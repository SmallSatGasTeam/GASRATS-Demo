// ======================================================================
// \title  FlightLogicTester.cpp
// \author jjhessing
// \brief  cpp file for FlightLogic component test harness implementation class
// ======================================================================

#include "FlightLogicTester.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  FlightLogicTester ::
    FlightLogicTester() :
      FlightLogicGTestBase("FlightLogicTester", FlightLogicTester::MAX_HISTORY_SIZE),
      component("FlightLogic")
  {
    this->initComponents();
    this->connectPorts();
    this->powerLow = false;
    this->antennaFail = false;
    this->cameraFail = false;
  }

  FlightLogicTester ::
    ~FlightLogicTester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  //! testTakePic
  //!
  //! Tests the takePic command
  void FlightLogicTester ::
    testTakePic()
  {
    //Run the takePic command and dispatch it
    this->sendCmd_takePic(0,0);
    this->component.doDispatch();

    //Check command response was received correctly
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(
      0,
      Components::FlightLogic::OPCODE_TAKEPIC,
      0,
      Fw::CmdResponse::OK
    );
  }

  //! testResetFlags
  //!
  //! Tests the resetFlags command
  void FlightLogicTester ::
    testResetFlags() {
      int iter = 0; //Used to measure the size of tlm history
      
      //Run resetFlags command
      this->sendCmd_resetFlags(0,0);
      iter++;

      //Check command response
      ASSERT_CMD_RESPONSE_SIZE(1);
      ASSERT_CMD_RESPONSE(
        0,
        Components::FlightLogic::OPCODE_RESETFLAGS,
        0,
        Fw::CmdResponse::OK
      );

      //Check that telemetry has been correctly reset
      ASSERT_TLM_SIZE(5);
      this->checkAllDefault(0);

      //Run system until deployed to change tlm
      iter += this->runTillDeployed(iter, 2, false);

      //Check tlm has been updated
      ASSERT_TLM_SIZE(5*iter);
      this->checkDeployed(iter-1);

      //Reset tlm with command resetflags
      this->sendCmd_resetFlags(0,1);

      //Check command response
      ASSERT_CMD_RESPONSE_SIZE(2);
      ASSERT_CMD_RESPONSE(
        1,
        Components::FlightLogic::OPCODE_RESETFLAGS,
        1,
        Fw::CmdResponse::OK
      );
      iter++;

      //Check tlm was reset
      ASSERT_TLM_SIZE(5*iter);
      this->checkAllDefault(iter-1);
    }

  //! testStartup
  //!
  //! Tests the startup port
  //! This test covers the requirement of waiting 30 minutes before
  //! deployment, immediately deploying the antenna, deploying the camera,
  //! taking a picture, and triggering the INITIAL beacon on startup
  void FlightLogicTester::testStartup() {
    int iter = 0; //Used to track tlm history size

    //run startup once
    this->invoke_to_startup(0,0);
    iter++;

    //Check tlm
    ASSERT_TLM_SIZE(5);
    this->checkDeployTlm(0);

    //Run startup until deployment
    iter += this->runTillDeployed(iter,1, true);

    //Check the tlm
    ASSERT_TLM_SIZE(5*(iter));
    this->checkDeployed(iter-1);

  }

  //! testDetumble
  //!
  //! Tests that the startup function will prompt the microcontroller to detumble
  //!   if it hasn't detumbled yet
  void FlightLogicTester::testDetumble() {
    //Run startup port
    this->invoke_to_startup(0,0);

    //Check if detumbled was triggered
    ASSERT_TLM_detumbled_SIZE(1);
    ASSERT_TLM_detumbled(0,true);

    //Reset flags to test again
    this->sendCmd_resetFlags(0,0);

    //Check detumbled tlm was reset
    ASSERT_TLM_detumbled_SIZE(2);
    ASSERT_TLM_detumbled(1,false);

    //Run startup
    this->invoke_to_startup(0,1);

    //Check detumbled switched to true
    ASSERT_TLM_detumbled_SIZE(3);
    ASSERT_TLM_detumbled(2,true);
  }

  //! testLowPower
  //!
  //! Tests that the startup function will throw the low power flag if EPS voltage
  //!   or current drops below a certain point
  void FlightLogicTester::testLowPower() {
    //Check that lowPower was not triggered
    this->invoke_to_startup(0,0);
    ASSERT_TLM_lowPower_SIZE(1);
    ASSERT_TLM_lowPower(0,false);

    //Have test EPSManager port return low power values
    this->powerLow = true;

    //Check that this triggers low power mode
    this->invoke_to_startup(0,1);
    ASSERT_TLM_lowPower_SIZE(2);
    ASSERT_TLM_lowPower(1,true);
  }

  //! testAntennaFailure
  //!
  //! Tests that startup will throw a fatal error if the antenna fails to deploy
  void FlightLogicTester::testAntennaFailure(){
    int size = 1; //Used to track tlm history size

    //Set test port to return antenna failure
    antennaFail = true;

    //Check that antenna doesn't deploy
    size += runTillDeployed(0,1,false);
    ASSERT_TLM_antennaState_SIZE(size);
    ASSERT_TLM_antennaState(size-1,GASRATS::deployed::UNDEPLOYED);
    ASSERT_EVENTS_deployFailure_SIZE(1);
    
    //Check for system failure
    size += runFor(STARTUP_MAX_ITER);
    ASSERT_EVENTS_rebooting_SIZE(1);
  }


  //! testCameraFailure
  //!
  //! Tests that startup will throw a fatal error if the camera fails to deploy
  void FlightLogicTester::testCameraFailure(){
    int size = 1; //Tracks size of tlm history

    //Set test port to report camera deployment failure
    cameraFail = true;

    //Check that camera doesn't deploy
    size += runTillDeployed(0,1,false);
    ASSERT_TLM_cameraState_SIZE(size);
    ASSERT_TLM_cameraState(size-1,GASRATS::deployed::UNDEPLOYED);
    ASSERT_EVENTS_deployFailure_SIZE(1);
    
    //Check that failed deployment causes system failure
    size += runFor(STARTUP_MAX_ITER);
    ASSERT_EVENTS_rebooting_SIZE(1);
  }

  //! testHealthPing
  //!
  //! Tests that the component is pinging the health componenet
  void FlightLogicTester::testHealthPing() {
    //Run pingIn input port
    this->invoke_to_pingIn(0,87);
    this->component.doDispatch();

    //Check output port runs
    this->pushFromPortEntry_pingOut(87);
    ASSERT_from_pingOut_SIZE(1); 
  }

  //! testDataRequest
  //!
  //! Tests the dataRequest port
  void FlightLogicTester::testDataRequest() {
    //Run dataRequest input port
    this->invoke_to_dataRequest(0,72);
    //Check fakeData output port
    this->pushFromPortEntry_fakeData(72);
    ASSERT_from_fakeData_SIZE(1);
  }

  //! testSendBeaconState
  //! 
  //! Tests the sendBeaconState port
  void FlightLogicTester::testBeaconState() {
    //Check sendBeaconState returns correct value
    ASSERT_EQ(GASRATS::beacon::OFF, this->invoke_to_beaconState(0,GASRATS::beacon::RETURN_STATE));
    this->invoke_to_startup(0,0);
    ASSERT_TLM_beaconState_SIZE(1);
    ASSERT_TLM_beaconState(0, GASRATS::beacon::OFF);

    ASSERT_EQ(GASRATS::beacon::INITIAL, this->invoke_to_beaconState(0,GASRATS::beacon::INITIAL));
    this->invoke_to_startup(0,0);
    ASSERT_TLM_beaconState_SIZE(2);
    ASSERT_TLM_beaconState(1, GASRATS::beacon::INITIAL);

    ASSERT_EQ(GASRATS::beacon::STANDARD, this->invoke_to_beaconState(0,GASRATS::beacon::STANDARD));
    this->invoke_to_startup(0,0);
    ASSERT_TLM_beaconState_SIZE(3);
    ASSERT_TLM_beaconState(2, GASRATS::beacon::STANDARD);

    ASSERT_EQ(GASRATS::beacon::ERROR, this->invoke_to_beaconState(0,GASRATS::beacon::ERROR));
    this->invoke_to_startup(0,0);
    ASSERT_TLM_beaconState_SIZE(4);
    ASSERT_TLM_beaconState(3, GASRATS::beacon::ERROR);
  }

  //! testBeacon
  //! 
  //! Tests the beacon is off until after the delay. Then checks that beacon is INITIAL until
  //! confirmConnection makes it STANDARD
  void FlightLogicTester::testBeacon(){
    U32 hist = 1;

    hist += this->runTillDeployed(0, 5, false);
    ASSERT_TLM_beaconState_SIZE(hist);
    ASSERT_TLM_beaconState(hist-1, GASRATS::beacon::INITIAL);

    this->invoke_to_beaconState(0, GASRATS::beacon::STANDARD);
    this->invoke_to_startup(0,0);
    hist++;
    ASSERT_TLM_beaconState_SIZE(hist);
    ASSERT_TLM_beaconState(hist-1, GASRATS::beacon::STANDARD);

  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  bool FlightLogicTester ::
    from_deployAntenna_handler(NATIVE_INT_TYPE portNum)
  {
    return !antennaFail;
  }

  bool FlightLogicTester ::
    from_deployCamera_handler(NATIVE_INT_TYPE portNum)
  {
    return !cameraFail;
  }

  void FlightLogicTester ::
    from_epsHealth_handler(
        NATIVE_INT_TYPE portNum,
        F32& voltage,
        F32& current
    )
  {
    //If in low power mode send failing power values
    if(powerLow) {
      voltage = 1;
      current = .05;
    }
    //Otherwise return normal values
    else {
      voltage = 5;
      current = 1;
    }
  }

  void FlightLogicTester ::
    from_fakeData_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    //std::cout << value << std::endl;
    return;
  }

  void FlightLogicTester ::
    from_pingOut_handler(
        NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    //std::cout << "Key: " << key << std::endl;
    return;
  }

  U32 FlightLogicTester ::
    from_sendTrans_handler(
        NATIVE_INT_TYPE portNum,
        U32 value
    )
  {
    return value;
  }

  void FlightLogicTester ::
    from_takePic_handler(NATIVE_INT_TYPE portNum)
  {
    return;
  }

  // ----------------------------
  // Helper functions
  // ----------------------------

  //! checkAllDefault
  //!
  //! Checks that all telemetry matches default values
  //!   index: The index in the history to be checked
  void FlightLogicTester::checkAllDefault(int index) {
    ASSERT_TLM_antennaState_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_beaconState_SIZE(index+1);
    ASSERT_TLM_beaconState(index,GASRATS::beacon::T::OFF);
    
    ASSERT_TLM_cameraState_SIZE(index+1);
    ASSERT_TLM_cameraState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_detumbled_SIZE(index+1);
    ASSERT_TLM_detumbled(index,false);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);
  }

  //! checkDeployTlm
  //!
  //! Checks that telemetry matches expected mid deploy values
  //!   index: The index in the history to be checked
  void FlightLogicTester::checkDeployTlm(int index) {
    ASSERT_TLM_antennaState_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_beaconState_SIZE(index+1);
    ASSERT_TLM_beaconState(index,GASRATS::beacon::T::OFF);
    
    ASSERT_TLM_cameraState_SIZE(index+1);
    ASSERT_TLM_cameraState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_detumbled_SIZE(index+1);
    ASSERT_TLM_detumbled(index,true);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_lowPower(index,false);
  }

  //! checkDeployed
  //!
  //! Checks that telemetry matches expected post deploy values
  //!   index: The index in the history to be checked
  void FlightLogicTester::checkDeployed(int index) {
    ASSERT_TLM_antennaState_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::DEPLOYED);

    ASSERT_TLM_beaconState_SIZE(index+1);
    ASSERT_TLM_beaconState(index,GASRATS::beacon::T::INITIAL);
    
    ASSERT_TLM_cameraState_SIZE(index+1);
    ASSERT_TLM_cameraState(index,GASRATS::deployed::T::DEPLOYED);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::DEPLOYED);

    ASSERT_TLM_detumbled_SIZE(index+1);
    ASSERT_TLM_detumbled(index,true);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_lowPower(index,false);
  }

  //! runTillDeployed
  //!
  //! Runs startup until past iterations after deploy
  //!   start: Current iteration of startup
  //!   past: Number of iterations to continue past startup
  //!   checks: flag to decide whether or not to run checks each iteration
  int FlightLogicTester::runTillDeployed(int start, int past, bool checks) {
    for(int i = start; i <= DEPLOY_WAIT_ITER+past; i++) {
      this->invoke_to_startup(0,i);
      if (checks){
        if(start != DEPLOY_WAIT_ITER+1) {
          this->checkDeployTlm(i);
          start++;
        }
        else {
          this->checkDeployed(i);
        }
      }
    }
    return DEPLOY_WAIT_ITER + past;
  }

  //! runFor
  //!
  //! Runs startup for iter iterations
  //!   iter: number of iterations to run startup
  int FlightLogicTester::runFor(int iter) {
    for(int i = 0; i < iter; i++) {
      this->invoke_to_startup(0,i);
    }
    return iter;
  }

}
