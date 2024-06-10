// ======================================================================
// \title  FlightLogic.cpp
// \author jjhessing
// \brief  cpp file for FlightLogic component implementation class
// ======================================================================

#include "Components/FlightLogic/FlightLogic.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FlightLogic ::
    FlightLogic(const char* const compName) :
      FlightLogicComponentBase(compName)
  {

  }

  FlightLogic ::
    ~FlightLogic()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  // void FlightLogic ::
  //   recvTransmission_handler(NATIVE_INT_TYPE portNum)
  // {
  //   // TODO
  // }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void FlightLogic ::
    takePic_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->takePic_out(0);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void FlightLogic ::
    sendTransmission_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U32 duration
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void FlightLogic ::
    resetFlags_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void FlightLogic ::
    startupCommand_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    this->startup_internalInterfaceInvoke();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void FlightLogic ::
    startup_internalInterfaceHandler()
  {
    this->deployAntenna_out(0);
    this->deployCamera_out(0);
  }

}
