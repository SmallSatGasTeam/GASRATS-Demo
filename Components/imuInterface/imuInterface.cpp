// ======================================================================
// \title  imuInterface.cpp
// \author devin
// \brief  cpp file for imuInterface component implementation class
// ======================================================================

#include "Components/imuInterface/imuInterface.hpp"
#include "Components/componentConfig/Constants.hpp"
#include "FpConfig.hpp"
#include "fprime/Drv/LinuxI2cDriver/LinuxI2cDriver.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  imuInterface ::
    imuInterface(const char* const compName) :
      imuInterfaceComponentBase(compName)
  {
    this->calls = 0;
  }

  imuInterface ::
    ~imuInterface()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void imuInterface ::startup() {
    //Allocate 2 buffers each of size 2 bytes
    const U32 bufferSize = 2;
    Fw::Buffer imuConfig = this->allocate_out(0, bufferSize);  
    Fw::Buffer imuTwo = this->allocate_out(0, bufferSize);

    //Check that the buffers were properly allocated
    if (imuConfig.getSize() < bufferSize || imuTwo.getSize() < bufferSize) {
        this->deallocate_out(0, imuConfig);
        this->deallocate_out(0, imuTwo);
        this->log_WARNING_LO_MemoryAllocationFailed();
        return;
      } 
  
    //Prepare imuConfig to be loaded with values
    Fw::SerializeBufferBase & config = imuConfig.getSerializeRepr();
    U8 sampleRate = 0x20;
    config.resetDeser();
    config.resetSer();
    //Put the address for register CTRL1 and the value ALL_ON in the buffer (each is 1 byte)
    config.serialize(this->CTRL1);
    config.serialize(this->ALL_ON);

    //Prepare imuTwo to be loaded with values
    config = imuTwo.getSerializeRepr();
    config.resetDeser();
    config.resetSer();
    //Put the address for register CTRL4 and the value to set the sample reate on the buffer (each is 1 byte)
    config.serialize(this->CTRL4);
    config.serialize(sampleRate);

    //Write the values to the imu and check if it was written successfully
    #ifndef VIRTUAL
    this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuConfig));
    this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuTwo));
    #endif

    //Deallocate the buffers to avoid a memory leak
    this->deallocate_out(0,imuConfig);
    this->deallocate_out(0,imuTwo);
  }


  U32 imuInterface ::
    dataRequest_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    #ifndef VIRTUAL
      U8 startAddress = this->X_L | 0x80; // ORing with 0x80 to read multiple bytes

      //Set sizes of buffers
      const U32 writeSize = 1;
      const U32 readSize = 6;

      //Allocate buffers
      Fw::Buffer imuConfigSTAddress = this->allocate_out(0, writeSize);
      Fw::Buffer imuData = this->allocate_out(0, readSize);
      
      //Check buffers were allocated properly
      if (imuData.getSize() < readSize || imuConfigSTAddress.getSize() < writeSize) {
        this->deallocate_out(0, imuData);
        this->deallocate_out(0, imuConfigSTAddress);
        this->log_WARNING_LO_MemoryAllocationFailed();
        return value;
      } 
      
      //Prepare imuConfigSTAddress to have data
      Fw::SerializeBufferBase & data = imuConfigSTAddress.getSerializeRepr();      
      data.resetDeser();
      data.resetSer();
      data.serialize(startAddress); //Add the start address to the buffer

      //Write the startAddress and then read the next 6 bytes from the imu
      this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuConfigSTAddress));
      this->checkStatus(this->requestI2CData_out(0, this->ADDRESS, imuData));

      //Output the imuData
      this->gyroData_out(0, imuData);

      //Deallocate the buffers
      this->deallocate_out(0, imuData);
      this->deallocate_out(0, imuConfigSTAddress);
    #endif


    return value;
  }

  // ----------------------------------------------------------------------
  // Helper Functions
  // ----------------------------------------------------------------------

  void imuInterface::checkStatus(Drv::I2cStatus i2cStatus) {
    if(this->calls < MAX_BACKGROUND_MESSAGES) {
      this->calls++;
      switch (i2cStatus) {
        case Drv::I2cStatus::I2C_OK:
          this->log_ACTIVITY_HI_imuSuccess();
          break;

        case Drv::I2cStatus::I2C_ADDRESS_ERR:
          this->log_WARNING_HI_imuAddressFailure();
          break;

        case Drv::I2cStatus::I2C_WRITE_ERR:
          this->log_WARNING_HI_imuWriteError();
          break;

        case Drv::I2cStatus::I2C_READ_ERR:
          this->log_WARNING_HI_imuReadError();
          break;

        case Drv::I2cStatus::I2C_OPEN_ERR:
          this->log_WARNING_HI_imuOpenError();
          break;

        case Drv::I2cStatus::I2C_OTHER_ERR:
        this->log_WARNING_HI_imuOtherError();
        break;
        
        default:
          break;
      }
    }
  }
}
