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
    dataRequest_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    // this will hold all the data from the imu, unfortuenately idk how to make an output port take in a Buffer type so it's just more fake data for now
    Fw::Buffer imuData;
    this->requestI2CData_out(0, 0011110, imuData);
    this->collector_out(0, 1234);
    this->log_ACTIVITY_HI_imuSuccess();
    
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
