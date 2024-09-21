// ======================================================================
// \title  imuInterface.cpp
// \author devin
// \brief  cpp file for imuInterface component implementation class
// ======================================================================

#include "Components/imuInterface/imuInterface.hpp"
#include "Components/componentConfig/Constants.hpp"
#include "FpConfig.hpp"
#include "fprime/Drv/LinuxI2cDriver/LinuxI2cDriver.hpp"
#include <iostream>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  imuInterface ::
    imuInterface(const char* const compName) :
      imuInterfaceComponentBase(compName)
  {
    this->calls = 0;
    this->bootTime = this->getTime();
  }

  imuInterface ::
    ~imuInterface()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void imuInterface::startup_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Fw::Buffer imuConfig = this->allocate_out(0, 2);  
    Fw::Buffer imuTwo = this->allocate_out(0, 2);
  
    Fw::SerializeBufferBase & config = imuConfig.getSerializeRepr();
    U8 sampleRate = 0x20;
    // config.resetDeser();
    config.resetSer();
    config.serialize(this->CTRL1);
    config.serialize(this->ALL_ON);

    config = imuTwo.getSerializeRepr();
    config.resetDeser();
    config.resetSer();
    config.serialize(this->CTRL4);
    config.serialize(sampleRate);

    this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuConfig));
    this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuTwo));
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

      const U32 writeSize = 1;
      const U32 readSize = 6;
      Fw::Buffer imuConfigSTAddress = this->allocate_out(0, writeSize);
      Fw::Buffer imuData = this->allocate_out(0, readSize);
      
      if (imuData.getSize() < readSize || imuConfigSTAddress.getSize() < writeSize) {
        this->deallocate_out(0, imuData);
        this->deallocate_out(0, imuConfigSTAddress);
        this->log_WARNING_LO_MemoryAllocationFailed();
      } 
      
      Fw::SerializeBufferBase & data = imuConfigSTAddress.getSerializeRepr();      
      data.resetDeser();
      data.resetSer();
      data.serialize(startAddress);

      this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuConfigSTAddress));
      this->checkStatus(this->requestI2CData_out(0, this->ADDRESS, imuData));

      //this->checkStatus(this->i2cWriteRead_out(0, this->ADDRESS, imuConfigSTAddress, imuData));

      this->gyroData_out(0, imuData);

      this->deallocate_out(0, imuData);
      this->deallocate_out(0, imuConfigSTAddress);
    #endif


    return value;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void imuInterface ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
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

  void imuInterface::setTime() {
    this->bootTime = this->getTime();
  }
}
