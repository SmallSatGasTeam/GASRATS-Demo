// ======================================================================
// \title  FlightLogic.cpp
// \author jjhessing
// \brief  cpp file for FlightLogic component implementation class
// ======================================================================

#include "Components/FlightLogic/FlightLogic.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  FlightLogic ::
    FlightLogic(const char* const compName) :
      FlightLogicComponentBase(compName)
  {
    //Try to open the file
    std::ifstream file;
    file.open(filePath, std::ios::in);
    if(file.is_open()) {
      U64 temp;

      //Assign antenna state
      file >> temp;
      std::cout << temp << std::endl;
      switch (temp) {
      case 0:
        this->antennaState = GASRATS::deployed::DEPLOYED;
        break;
      
      default:
        this->antennaState = GASRATS::deployed::UNDEPLOYED;
        break;
      }

      //Assign beacon state
      file >> temp;
      std::cout << temp << std::endl;
      switch(temp) {
        case 0:
          this->beaconState = GASRATS::beacon::RETURN_STATE;
          break;
        case 1:
          this->beaconState = GASRATS::beacon::INITIAL;
          break;
        case 2:
          this->beaconState = GASRATS::beacon::STANDARD;
          break;
        case 3:
          this->beaconState = GASRATS::beacon::ERROR;
          break;
        default:
          this->beaconState = GASRATS::beacon::OFF;
      }

      //Assign boot time
      file >> temp;
      std::cout << temp << std::endl;
      this->bootTime = std::chrono::system_clock::now();

      //Assign camera state
      file >> temp;
      std::cout << temp << std::endl;
      switch (temp) {
      case 0:
        this->cameraState = GASRATS::deployed::DEPLOYED;
        break;
      
      default:
        this->cameraState = GASRATS::deployed::UNDEPLOYED;
        break;
      }

      //Assign detumbled
      file >> detumbled;
      std::cout << temp << std::endl;

      //Assign low power
      file >> lowPower;
      std::cout << temp << std::endl;
      file.close();
    }
    //Else assign initial values to component variables and throw a warning
    else {
      this->antennaState = GASRATS::deployed::UNDEPLOYED;
      this->cameraState = GASRATS::deployed::UNDEPLOYED;
      this->beaconState = GASRATS::beacon::OFF;
      this->failCount = 0;
      this->epsCurrent = 1;
      this->epsVoltage = 5;
      this->lowPower = true;
      this->detumbled = false;
      this->bootTime = std::chrono::system_clock::now();
      this->log_WARNING_HI_fileFailed(filePath);
    }
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
    //Variable declarations
    std::chrono::duration<int,std::ratio<1>> waitTime(static_cast<int>(DEPLOY_WAIT_ITER));
    bool changed = false;

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
        if(std::chrono::system_clock::now() >= (this->bootTime + waitTime)) {
          // deploy antenna
          if(antennaState == GASRATS::deployed::UNDEPLOYED) {
            if(deployAntenna_out(0)) {
              this->antennaState = GASRATS::deployed::DEPLOYED;
              changed = true;
            }
            // If failed, give a second try
            else if (deployAntenna_out(0)) {
              this->antennaState = GASRATS::deployed::DEPLOYED;
              changed = true;
            }
            // If failed twice, throw an error
            else {
              this->log_WARNING_HI_deployFailure("Antenna");
            }
          }

          //Turn on the initial beacon
          this->beaconState = GASRATS::beacon::INITIAL;

          // Deploy camera
          if(cameraState == GASRATS::deployed::UNDEPLOYED) {
            if(deployCamera_out(0)) {
              this->cameraState = GASRATS::deployed::DEPLOYED;
              changed = true;
              // Take photo
              this->takePic_out(0);
            }
            //If failed try again
            else if(deployCamera_out(0)) {
              this->cameraState = GASRATS::deployed::DEPLOYED;
              changed = true;
            }
            // else throw failure
            else {
              this->log_WARNING_HI_deployFailure("Camera");
            }
          }

          //If a state has changed write it to the file
          if(changed) {
            //this->saveFlags_internalInterfaceInvoke();
          }
        
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
        // Else do nothing and try again next iteration
      }
      //Else startup flags are good
      //Await commands (AKA, leave this thread and move on)
    }
    //Else run low power mode
    else {
      lowPower = true;
      detumbled = false;
      this->beaconState = GASRATS::beacon::ERROR;
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
      this->saveFlags_internalInterfaceInvoke();
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
    this->beaconState = GASRATS::beacon::OFF;
    this->failCount = 0;
    this->epsCurrent = 1;
    this->epsVoltage = 5;
    this->lowPower = true;
    this->detumbled = false;
    this->tlmWrite_antennaState(this->antennaState);
    this->tlmWrite_cameraState(this->cameraState);
    this->tlmWrite_beaconState(this->beaconState);
    this->tlmWrite_detumbled(this->detumbled);
    this->tlmWrite_lowPower(this->lowPower);

    this->saveFlags_internalInterfaceInvoke();

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined internal interfaces
  // ----------------------------------------------------------------------

  void FlightLogic ::
    saveFlags_internalInterfaceHandler()
  {
    //Open the file
    std::ofstream file;
    file.open(this->filePath, std::ios::out | std::ios::trunc);
    if(!file.is_open()) {
      this->log_WARNING_HI_fileFailed(this->filePath);
      return;
    }
    
    file << this->antennaState << std::endl;
    file << this->beaconState << std::endl;
    file << this->bootTime.time_since_epoch().count() << std::endl;
    file << this->cameraState << std::endl;
    file << this->detumbled << std::endl;
    file << this->lowPower << std::endl;

    file.close();
  }
}