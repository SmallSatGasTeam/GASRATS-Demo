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
      const U32 secondary_size = 6;
      Fw::Buffer imuData = this->allocate_out(0, needed_size);

      if (imuData.getSize() < needed_size) {
        this->deallocate_out(0, imuData);
        this->log_WARNING_LO_MemoryAllocationFailed();
      }

      Fw::Buffer autoIncrement = this->allocate_out(0, needed_size);

      if (autoIncrement.getSize() < needed_size) {
        this->deallocate_out(0, autoIncrement);
        this->log_WARNING_LO_MemoryAllocationFailed();
      }

      Fw::Buffer xyz = this->allocate_out(0, secondary_size);

      if (xyz.getSize() < secondary_size) {
        this->deallocate_out(0, xyz);
        this->log_WARNING_LO_MemoryAllocationFailed();
      }

      char reg[1] = {X_L | 0x80};
      Fw::SerializeBufferBase& sb = autoIncrement.getSerializeRepr();
      sb.resetSer();  // Return the serialization to the beginning of the memory region
      sb.serialize(reg);
       
      U8 x_h = 0;
      U8 x_l = 0;
      U8 y_h = 0;
      U8 y_l = 0;
      U8 z_h = 0;
      U8 z_l = 0;
      Fw::SerializeBufferBase& sb1 = xyz.getSerializeRepr();
      sb.resetSer();  // Return the serialization to the beginning of the memory region
      sb.serialize(x_h);
      sb.serialize(x_l);
      sb.serialize(y_h);
      sb.serialize(y_l);
      sb.serialize(z_h);
      sb.serialize(z_l);

      // then you just have to give the i2c driver the slave address and a buffer that it'll put data into
      // this->checkStatus(this->requestI2CData_out(0, this->X_L, imuData)); // this will update the buffer 'imuData' with the data from the slave device
      // this->checkStatus(this->requestI2CData_out(0, this->X_H, imuData));

      this->checkStatus(this->i2cWrite_out(0, ADDRESS, autoIncrement));

      this->checkStatus(this->requestI2CData_out(0, ADDRESS, xyz));

      this->checkStatus(this->requestI2CData_out(0,ADDRESS,imuData));
      // this->collector_out(0, 1234); // just a way to test if we're connected to the dataCollector

      this->gyroData_out(0, xyz);

      this->deallocate_out(0, imuData);
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
