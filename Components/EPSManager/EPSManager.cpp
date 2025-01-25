// ======================================================================
// \title  EPSManager.cpp
// \author jjhessing
// \brief  cpp file for EPSManager component implementation class
// ======================================================================

#include "Components/EPSManager/EPSManager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  EPSManager ::
    EPSManager(const char* const compName) :
      EPSManagerComponentBase(compName)
  {

  }

  EPSManager ::
    ~EPSManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void EPSManager ::
    returnHealth_handler(
        NATIVE_INT_TYPE portNum,
        F32& voltage,
        F32& current
    )
  {
    // TODO return
    //!!! This is code to sepslate the EPS. The code will only be run if VIRTUAL is defined
    // #ifdef VIRTUAL
    std::normal_distribution<float> currentDistribution(3,.5);
    std::normal_distribution<float> voltageDistribution(7,1);
    current = currentDistribution(generator);
    voltage = voltageDistribution(generator);

    //#endif

    this->tlmWrite_current(current);
    this->tlmWrite_voltage(voltage);
    
  }

  U32 EPSManager ::
    dataRequest_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    // const U32 needed_size = 1024;
    // Fw::Buffer writeBuffer = this->allocate_out(0, needed_size);      
    // Fw::Buffer readBuffer = this->allocate_out(0, needed_size);

    // if (readBuffer.getSize() < needed_size || writeBuffer.getSize() < needed_size) {
    //   this->deallocate_out(0, readBuffer);
    //   this->deallocate_out(0, writeBuffer);
    //   this->log_WARNING_LO_MemoryAllocationFailed();
    // }

    // U32 requestedData = 0x01;
    // Fw::SerializeBufferBase & sb = writeBuffer.getSerializeRepr();
    // sb.serialize(requestedData);
    // Fw::SerializeBufferBase & sb1 = readBuffer.getSerializeRepr();      

    // U8 reg = 0x68;
    // //this->log_ACTIVITY_LO_print(reg);

    // sb.resetSer();
    // sb.serialize(reg);
    // sb1.resetDeser();
    // sb1.resetSer();

    // this->checkStatus(this->i2cReadWrite_out(0, reg, writeBuffer, readBuffer));

    // this->epsData_out(0, readBuffer);

    // this->deallocate_out(0, readBuffer);
    // this->deallocate_out(0, writeBuffer);

    return value;
  }

  void EPSManager::checkStatus(Drv::I2cStatus i2cStatus) {
    switch (i2cStatus) {
      case Drv::I2cStatus::I2C_OK:
        this->log_ACTIVITY_HI_epsSuccess();
        break;

      case Drv::I2cStatus::I2C_ADDRESS_ERR:
        this->log_WARNING_HI_epsAddressFailure();
        break;

      case Drv::I2cStatus::I2C_WRITE_ERR:
        this->log_WARNING_HI_epsWriteError();
        break;

      case Drv::I2cStatus::I2C_READ_ERR:
        this->log_WARNING_HI_epsReadError();
        break;

      case Drv::I2cStatus::I2C_OPEN_ERR:
        this->log_WARNING_HI_epsOpenError();
        break;

      case Drv::I2cStatus::I2C_OTHER_ERR:
       this->log_WARNING_HI_epsOtherError();
       break;
      
      default:
        break;
    }
  }

}
