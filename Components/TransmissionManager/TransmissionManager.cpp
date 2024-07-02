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
    this->iter = 0;
  }

  TransmissionManager ::
    ~TransmissionManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void TransmissionManager ::
    beacon_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if(this->iter >= BEACON_INTERVAL){
      this->iter = 0;
      //Send out either initial beacon or standard beacon
      switch(this->beaconState_out(0,GASRATS::beacon::RETURN_STATE)) {
        case GASRATS::beacon::INITIAL:
          this->sendData_out(0,0x01);
          break;

        case GASRATS::beacon::STANDARD:
          this->sendData_out(0,0xFFFFFFFF);
          break;
        
        default:
          this->log_WARNING_LO_invalidBeaconState();
          this->beaconState_out(0,GASRATS::beacon::INITIAL);
          break;
      }
    }
    else {
      iter++;
    }
  }

  void TransmissionManager ::
    recvData_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    this->log_ACTIVITY_LO_success(value);
    return;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void TransmissionManager ::
    confirmConnection_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    this->beaconState_out(0,GASRATS::beacon::T::STANDARD);
    this->log_ACTIVITY_HI_beaconSet(GASRATS::beacon::T::STANDARD);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void TransmissionManager ::
    setBeacon_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        GASRATS::beacon state
    )
  {
    this->beaconState_out(0,state);
    this->log_ACTIVITY_HI_beaconSet(state);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void TransmissionManager ::
    sendTransToGround_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U32 data
    )
  {
    //this->sendData_out(0,data);
    this->log_ACTIVITY_LO_sending(data);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
