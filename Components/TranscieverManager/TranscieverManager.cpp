// ======================================================================
// \title  TranscieverManager.cpp
// \author devins
// \brief  cpp file for TranscieverManager component implementation class
// ======================================================================

#include "Components/TranscieverManager/TranscieverManager.hpp"
#include "FpConfig.hpp"
#include "fprime/Drv/LinuxI2cDriver/LinuxI2cDriver.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TranscieverManager ::
    TranscieverManager(const char* const compName) :
      TranscieverManagerComponentBase(compName)
  {

  }

  TranscieverManager ::
    ~TranscieverManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void TranscieverManager ::
    STARTUP_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    U8 reg = 0x68; // this needs to change

    const U32 writeBufferSize = 1;

    Fw::Buffer transcieverConfig = this->allocate_out(0, writeBufferSize);

    if (transcieverConfig.getSize() < writeBufferSize) {
      this->deallocate_out(0, transcieverConfig);
      this->log_WARNING_LO_MemoryAllocationFailed();
      // send out a WARNING LOW MEMORY ALLOCATION FAILED
      return;
    }

    Fw::SerializeBufferBase & config = transcieverConfig.getSerializeRepr();

    // this should be done before sending serialized data
    config.resetDeser();
    config.resetSer();

    // send the address for the register we need
    

    this->i2cWrite_out(0, reg, transcieverConfig);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

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
