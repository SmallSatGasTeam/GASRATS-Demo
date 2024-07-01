// ======================================================================
// \title  DummyTranceiverDriver.cpp
// \author jjhessing
// \brief  cpp file for DummyTranceiverDriver component implementation class
// ======================================================================

#include "Components/DummyTranceiverDriver/DummyTranceiverDriver.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  DummyTranceiverDriver ::
    DummyTranceiverDriver(const char* const compName) :
      DummyTranceiverDriverComponentBase(compName)
  {

  }

  DummyTranceiverDriver ::
    ~DummyTranceiverDriver()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void DummyTranceiverDriver ::
    sendTransToGround_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    this->log_ACTIVITY_HI_receivingTransGround(value);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void DummyTranceiverDriver ::
    sendTransToSatellite_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U32 data
    )
  {
    this->log_ACTIVITY_HI_sendingTransGround(data);
    this->recvTransFromGround_out(0, data);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
