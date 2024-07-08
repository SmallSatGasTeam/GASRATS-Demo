// ======================================================================
// \title  imuInterface.cpp
// \author devin
// \brief  cpp file for imuInterface component implementation class
// ======================================================================

#include "Components/imuInterface/imuInterface.hpp"
#include "FpConfig.hpp"

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
    collectorRequest_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    // this will hold all the data from the imu, unfortuenately idk how to make an output port take in a Buffer type so it's just more fake data for now
    Fw::Buffer imuData;
    Drv::I2cStatus i2cStatus;

    #ifndef VIRTUAL
      // the number '0011110' is the slave address for the Magnetorqer registers
      i2cStatus = this->requestI2CData_out(0, 0x6B, imuData); // this will update the buffer 'imuData' with the data from the slave device
      // this->collector_out(0, 1234); // just a way to test if we're connected to the dataCollector
      this->gyroData_out(0, imuData);
    #endif

    if (i2cStatus == Drv::I2cStatus::I2C_OK) {
      this->log_ACTIVITY_HI_imuSuccess(); // this just announces that the data has been recieved 
    } else if (i2cStatus == Drv::I2cStatus::I2C_ADDRESS_ERR) {
      this->log_WARNING_HI_imuFailure();
    } else {
      this->log_WARNING_HI_imuFailure();
    }

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

}
