// ======================================================================
// \title  CommunicationAdapter.cpp
// \author devins
// \brief  cpp file for CommunicationAdapter component implementation class
// ======================================================================

#include "Components/CommunicationAdapter/CommunicationAdapter.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  CommunicationAdapter ::
    CommunicationAdapter(const char* const compName) :
      CommunicationAdapterComponentBase(compName)
  {

  }

  CommunicationAdapter ::
    ~CommunicationAdapter()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  Drv::SendStatus CommunicationAdapter ::
    comDataIn_handler(
        FwIndexType portNum,
        Fw::Buffer& sendBuffer
    )
  {
    // TODO return
    // need to deallocate the given buffer
    this->deallocate_out(0, sendBuffer);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void CommunicationAdapter ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
