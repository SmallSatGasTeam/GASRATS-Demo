// ======================================================================
// \title  FlightLogic.cpp
// \author jjhessing
// \brief  cpp file for FlightLogic component implementation class
// ======================================================================

#include "Components/FlightLogic/FlightLogic.hpp"
#include <cstdlib>

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
    this->epsCurrent = 1;
    this->epsVoltage = 5;
    this->lowPower = true;
    this->waitCount = 0;
    this->detumbled = false;
  }

  FlightLogic ::
    ~FlightLogic()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  // U32 FlightLogic ::
  //   recvTransmission_handler(
  //       NATIVE_INT_TYPE portNum,
  //       U32 value
  //   )
  // {
  //   return value;
  // }

  U32 FlightLogic ::
    dataRequest_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    this->fakeData_out(0, 122333);
    return value;
  }

  void FlightLogic ::
    startup_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    //this->log_WARNING_LO_runningStartup(this->waitCount); //For debugging

    // Perform hardware checks
    this->epsHealth_out(0,epsVoltage, epsCurrent);

    this->fakeData_out(0, 12345);

    //If battery is within okay conditions
    if(epsCurrent > CURRENT_MIN && epsVoltage > VOLTAGE_MIN) {
      //If detumbled false, send a detumble message to the ADCS board
      if(!detumbled)  {
        this->log_ACTIVITY_LO_detumble();
        this->detumbled = true;
      }
      this->lowPower = false;
      // If startup
      if(antennaState == GASRATS::deployed::UNDEPLOYED || 
      cameraState == GASRATS::deployed::UNDEPLOYED
      || beaconState == GASRATS::beacon::INITIAL) {
        // If we've waited an adequate amount of time
        if(waitCount > DEPLOY_WAIT_ITER) {
          // deploy antenna
          if(antennaState == GASRATS::deployed::UNDEPLOYED) {
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
          }

          // Deploy camera
          if(cameraState == GASRATS::deployed::UNDEPLOYED) {
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
          }

          this->beaconState = GASRATS::beacon::STANDARD; // !! This is temporary to allow the code to move on from the startup phase
        
          //If everything has been completed successfully
          if (antennaState == GASRATS::deployed::DEPLOYED && 
          cameraState == GASRATS::deployed::DEPLOYED)
          this->failCount = 0;
          // Else keep trying startup for MAX_MIN_TILL_FAIL minutes
          else if(this->failCount < STARTUP_MAX_ITER) {
            this->failCount++;
          }
          //Else restart
          else {
            #ifndef VIRTUAL
              std::system("shutdown -r 1");
            #endif
            this->log_FATAL_rebooting();
          }
        }
        // Else increment
        else {
          this->waitCount++;
        }
      }
      //Else startup flags are good
      //Await commands (AKA, leave this thread and move on)
    }
    //Else run low power mode
    else {
      lowPower = true;
      detumbled = false;
    }

    this->tlmWrite_antennaState(this->antennaState);
    this->tlmWrite_cameraState(this->cameraState);
    this->tlmWrite_beaconState(this->beaconState);
    this->tlmWrite_detumbled(this->detumbled);
    this->tlmWrite_lowPower(this->lowPower);
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
    beaconState_handler(
      NATIVE_INT_TYPE portNum,
      const GASRATS::beacon& value
    )
  {
    if(value != GASRATS::beacon::RETURN_STATE) {
      this->beaconState = value;
    }
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
    this->takePic_out(0);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void FlightLogic ::
    resetFlags_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    this->antennaState = GASRATS::deployed::UNDEPLOYED;
    this->cameraState = GASRATS::deployed::UNDEPLOYED;
    this->beaconState = GASRATS::beacon::INITIAL;
    this->failCount = 0;
    this->epsCurrent = 1;
    this->epsVoltage = 5;
    this->lowPower = true;
    this->waitCount = 0;
    this->detumbled = false;
    this->tlmWrite_antennaState(this->antennaState);
    this->tlmWrite_cameraState(this->cameraState);
    this->tlmWrite_beaconState(this->beaconState);
    this->tlmWrite_detumbled(this->detumbled);
    this->tlmWrite_lowPower(this->lowPower);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }
}
