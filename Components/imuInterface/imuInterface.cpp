// ======================================================================
// \title  imuInterface.cpp
// \author devin
// \brief  cpp file for imuInterface component implementation class
// ======================================================================

#include "Components/imuInterface/imuInterface.hpp"
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

  }

  imuInterface ::
    ~imuInterface()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

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

      Fw::SerializeBufferBase & sb = imuConfig.getSerializeRepr();
      Fw::SerializeBufferBase & sb1 = imuData.getSerializeRepr();      

      U8 reg = this->X_L | 0x80;
      //this->log_ACTIVITY_LO_print(reg);

      sb.resetSer();
      sb.serialize(reg);
      sb1.resetDeser();
      sb1.resetSer();

      this->checkStatus(this->i2cWrite_out(0, this->ADDRESS, imuConfig));
      this->checkStatus(this->requestI2CData_out(0,this->ADDRESS,imuData));

      // sb1.setBuffLen(imuData.getSize());

      // U8 val;

      // sb1.deserialize(val);
      // this->log_ACTIVITY_LO_print(val);

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
