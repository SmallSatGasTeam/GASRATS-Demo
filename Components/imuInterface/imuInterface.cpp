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

  void imuInterface ::startup_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    const U32 needed_size = 1024;
    Fw::Buffer imuConfig = this->allocate_out(0, needed_size);  
    
    U32 turnAllOn = 0x0F; // should start the gyro in normal state, with all axis enabled

    Fw::SerializeBufferBase & config = imuConfig.getSerializeRepr();
    config.resetDeser();
    config.resetSer();
    config.serialize(turnAllOn);

    this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuConfig));
  }


  U32 imuInterface ::
    dataRequest_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    #ifndef VIRTUAL
      const U32 needed_size = 1024;
      Fw::Buffer imuConfig = this->allocate_out(0, needed_size);  
      Fw::Buffer imuData = this->allocate_out(0, needed_size);

      if (imuData.getSize() < needed_size || imuConfig.getSize() < needed_size) {
        this->deallocate_out(0, imuData);
        this->deallocate_out(0, imuConfig);
        this->log_WARNING_LO_MemoryAllocationFailed();
      }

      Fw::SerializeBufferBase & data = imuData.getSerializeRepr();      
      data.resetDeser();
      data.resetSer();

      this->checkStatus(this->requestI2CData_out(0,this->ADDRESS, imuData));

      this->gyroData_out(0, imuData);

      this->deallocate_out(0, imuData);
      this->deallocate_out(0,imuConfig);
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
