// ======================================================================
// \title  TransmissionManager.cpp
// \author jjhessing
// \brief  cpp file for TransmissionManager component implementation class
// ======================================================================

#include "Components/TransmissionManager/TransmissionManager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TransmissionManager ::
    TransmissionManager(const char* const compName) :
      TransmissionManagerComponentBase(compName)
  {

  }

  TransmissionManager ::
    ~TransmissionManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void TransmissionManager ::
    recvData_handler(NATIVE_INT_TYPE portNum)
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void TransmissionManager ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
