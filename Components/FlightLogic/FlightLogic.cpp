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
    //Assign initial values to component variables
    this->antennaState = GASRATS::deployed::UNDEPLOYED;
    this->cameraState = GASRATS::deployed::UNDEPLOYED;
    this->beaconState = GASRATS::beacon::INITIAL;
    this->failCount = 0;
    this->epsCurrent = 0;
    this->epsVoltage = 0;
    this->lowPower = true;
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

  void FlightLogic ::
    startup_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    //this->log_WARNING_LO_runningStartup(); //For debugging

    // Perform hardware checks
    this->epsHealth_out(0,epsVoltage, epsCurrent);

    //If battery is within okay conditions
    if(epsCurrent > CURRENT_MIN && epsVoltage > VOLTAGE_MIN) {
      lowPower = false;
      // If startup
      if(antennaState == GASRATS::deployed::UNDEPLOYED && 
      cameraState == GASRATS::deployed::UNDEPLOYED
      && beaconState == GASRATS::beacon::INITIAL) {
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

          // Transmit TODO
            // The transmission should pull the beacon state and handle sending the first transmission
            // as soon as confirmation of connection is sent. It can then change the beacon state itself
            // removing the need to handle the initial transmission in flight logic

          this->beaconState = GASRATS::beacon::STANDARD; // !! This is temporary to allow the code to move on from the startup phase
        
          //If everything has been completed successfully
          if (antennaState == GASRATS::deployed::UNDEPLOYED && 
          cameraState == GASRATS::deployed::UNDEPLOYED
          && beaconState == GASRATS::beacon::INITIAL)
          failCount = 0;
        }
        // Else keep trying startup for MAX_MIN_TILL_FAIL minutes
        else if(failCount < maxIter) {
          failCount++;
        }
        // Else restart TODO
      }
      //Else startup flags are good
      else {
        //Await commands
      }
    }
    //Else run low power mode
    else {
      lowPower = true;
      //TODO
        //Is there anything we need to do in low power mode? or do we just ignore commands or what?
    }
    this->tlmWrite_antennaState(this->antennaState);
    this->tlmWrite_cameraState(this->cameraState);
    this->tlmWrite_beaconState(this->beaconState);
  }

  void FlightLogic ::
    pingIn_handler(
        NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    //Respond to ping from health component
    this->pingOut_out(0,key);
  }

  GASRATS::beacon FlightLogic ::
    sendBeaconState_handler(NATIVE_INT_TYPE portNum)
  {
    return this->beaconState;
  }

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
}
