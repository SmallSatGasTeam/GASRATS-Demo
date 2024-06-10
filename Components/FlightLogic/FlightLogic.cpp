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
    this->antennaState = GASRATS::deployed::UNDEPLOYED;
    this->cameraState = GASRATS::deployed::UNDEPLOYED;
    this->initialState = GASRATS::deployed::UNDEPLOYED;
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
    this->deployAntenna_out(0);
    this->deployCamera_out(0);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }


  void FlightLogic ::
    startup_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    //this->log_WARNING_LO_runningStartup();
    // Perform software checks

    // Perform hardware checks

    // If startup
    if(antennaState == GASRATS::deployed::UNDEPLOYED && 
    cameraState == GASRATS::deployed::UNDEPLOYED
    && initialState == GASRATS::deployed::UNDEPLOYED) {
      // Check EPS conditions TODO
      // If conditions are okay TODO
      if(true) {
        // deploy antenna
        if(deployAntenna_out(0)) {
          this->antennaState = GASRATS::deployed::DEPLOYED;
        }
        // If failed, give a second try
        else if (deployAntenna_out(0)) {
          this->antennaState = GASRATS::deployed::DEPLOYED;
        }
        // If failed twice, throw an error
        else {
          this->log_WARNING_HI_deployFailure("Antenna");
        }

        // Set beacon state to initial beacon TODO
        this->initialState = GASRATS::deployed::DEPLOYED;

        // Deploy camera
        if(deployCamera_out(0)) {
          this->cameraState = GASRATS::deployed::DEPLOYED;
          // Take photo
          this->takePic_out(0);
        }
        //If failed try again
        else if(deployCamera_out(0)) {
          this->cameraState = GASRATS::deployed::DEPLOYED;
        }
        // else throw failure
        else {
          this->log_WARNING_HI_deployFailure("Camera");
        }
        // Wait for ground connection TODO

        // Transmit TODO
      }
      // Else TODO

        // Wait for good conditions, or restart

        // Else

        // If startup flags are good

        // Await commands

        // Ping the watchdog

        // Check if battery voltage is high enough

        // Else

        // Re-run startup
    }
    this->tlmWrite_antennaState(this->antennaState);
    this->tlmWrite_cameraState(this->cameraState);
    this->tlmWrite_initialState(this->initialState);
  }
}
