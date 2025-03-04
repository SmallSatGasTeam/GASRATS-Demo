// ======================================================================
// \title  TranscieverManager.cpp
// \author devins
// \brief  cpp file for TranscieverManager component implementation class
// ======================================================================

#include "Components/TranscieverManager/TranscieverManager.hpp"
#include "FpConfig.hpp"

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

    Fw::Buffer writeBuffer = this->allocate_out(0, writeBufferSize);

    if (writeBuffer.getSize() < writeBufferSize) {
      this->deallocate_out(0, writeBuffer);
      // send out a WARNING LOW MEMORY ALLOCATION FAILED
    }

    this->i2cWrite_out(0, reg, writeBuffer);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
