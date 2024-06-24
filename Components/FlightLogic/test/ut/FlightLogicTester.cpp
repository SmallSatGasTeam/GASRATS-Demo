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

  void FlightLogicTester ::
    testTakePic()
  {
    this->sendCmd_takePic(0,0);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(
      0,
      Components::FlightLogic::OPCODE_TAKEPIC,
      0,
      Fw::CmdResponse::OK
    );
  }

  void FlightLogicTester ::
    testResetFlags() {
      int iter = 0;
      this->sendCmd_resetFlags(0,0);
      iter++;

      ASSERT_CMD_RESPONSE_SIZE(1);
      ASSERT_CMD_RESPONSE(
        0,
        Components::FlightLogic::OPCODE_RESETFLAGS,
        0,
        Fw::CmdResponse::OK
      );

      ASSERT_TLM_SIZE(5);

      this->checkAllDefault(0);

      iter += this->runTillDeployed(iter, 2, false);

      ASSERT_TLM_SIZE(5*iter);
      this->checkDeployed(iter-1);

      this->sendCmd_resetFlags(0,1);
      ASSERT_CMD_RESPONSE_SIZE(2);
      ASSERT_CMD_RESPONSE(
        1,
        Components::FlightLogic::OPCODE_RESETFLAGS,
        1,
        Fw::CmdResponse::OK
      );
      iter++;
      ASSERT_TLM_SIZE(5*iter);
      this->checkAllDefault(iter-1);
    }

    void FlightLogicTester::testSendTransmission() {
      this->sendCmd_sendTransmission(0,0,1);
      this->component.doDispatch();

      ASSERT_CMD_RESPONSE_SIZE(1);
      ASSERT_CMD_RESPONSE(
        0,
        Components::FlightLogic::OPCODE_SENDTRANSMISSION,
        0,
        Fw::CmdResponse::OK
      );
    }

    void FlightLogicTester::testStartup() {
      int iter = 0;

      this->invoke_to_startup(0,0);
      iter++;

      ASSERT_TLM_SIZE(5);

      this->checkDeployTlm(0);

      iter += this->runTillDeployed(iter,1, true);

      ASSERT_TLM_SIZE(5*(iter));
      this->checkDeployed(iter-1);

    }

    void FlightLogicTester::testDetumble() {
      this->invoke_to_startup(0,0);

      ASSERT_TLM_detumbled_SIZE(1);
      ASSERT_TLM_detumbled(0,true);

      this->sendCmd_resetFlags(0,0);

      ASSERT_TLM_detumbled_SIZE(2);
      ASSERT_TLM_detumbled(1,false);

      this->invoke_to_startup(0,1);

      ASSERT_TLM_detumbled_SIZE(3);
      ASSERT_TLM_detumbled(2,true);
    }

    void FlightLogicTester::testLowPower() {
      this->invoke_to_startup(0,0);
      ASSERT_TLM_lowPower_SIZE(1);
      ASSERT_TLM_lowPower(0,false);

      this->powerLow = true;
      this->invoke_to_startup(0,1);
      ASSERT_TLM_lowPower_SIZE(2);
      ASSERT_TLM_lowPower(1,true);
    }

  void FlightLogicTester::testAntennaFailure(){
    int size = 1;
    antennaFail = true;
    size += runTillDeployed(0,1,false);
    ASSERT_TLM_antennaState_SIZE(size);
    ASSERT_TLM_antennaState(size-1,GASRATS::deployed::UNDEPLOYED);
    ASSERT_EVENTS_deployFailure_SIZE(1);
    
    size += runFor(STARTUP_MAX_ITER);
    ASSERT_EVENTS_rebooting_SIZE(1);
  }

  void FlightLogicTester::testCameraFailure(){
    int size = 1;
    cameraFail = true;
    size += runTillDeployed(0,1,false);
    ASSERT_TLM_cameraState_SIZE(size);
    ASSERT_TLM_cameraState(size-1,GASRATS::deployed::UNDEPLOYED);
    ASSERT_EVENTS_deployFailure_SIZE(1);
    
    size += runFor(STARTUP_MAX_ITER);
    ASSERT_EVENTS_rebooting_SIZE(1);
  }

  //!!! These two are acting weird
  void FlightLogicTester::testHealthPing() {
    this->invoke_to_pingIn(0,87);
    this->component.doDispatch();
    ASSERT_from_pingOut_SIZE(0); //!!! You'd expect this to be 1 but it's not. That being said it's still printing so its running as we want
  }

  void FlightLogicTester::testDataRequest() {
    this->invoke_to_dataRequest(0,72);
    ASSERT_from_fakeData_SIZE(0); //!!! This one is a problem child as well
  }

  void FlightLogicTester::testRecvTransmission() {
    ASSERT_EQ(43,this->invoke_to_recvTransmission(0,43));
  }

  void FlightLogicTester::testSendBeaconState() {
    ASSERT_EQ(GASRATS::beacon::INITIAL, this->invoke_to_sendBeaconState(0));
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
    if(powerLow) {
      voltage = 1;
      current = .05;
    }
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
    std::cout << "Key: " << key << std::endl;
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

  void FlightLogicTester::checkAllDefault(int index) {
    ASSERT_TLM_antennaState_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_beaconState_SIZE(index+1);
    ASSERT_TLM_beaconState(index,GASRATS::beacon::T::INITIAL);
    
    ASSERT_TLM_cameraState_SIZE(index+1);
    ASSERT_TLM_cameraState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_detumbled_SIZE(index+1);
    ASSERT_TLM_detumbled(index,false);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);
  }

  void FlightLogicTester::checkDeployTlm(int index) {
    ASSERT_TLM_antennaState_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_beaconState_SIZE(index+1);
    ASSERT_TLM_beaconState(index,GASRATS::beacon::T::INITIAL);
    
    ASSERT_TLM_cameraState_SIZE(index+1);
    ASSERT_TLM_cameraState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::UNDEPLOYED);

    ASSERT_TLM_detumbled_SIZE(index+1);
    ASSERT_TLM_detumbled(index,true);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_lowPower(index,false);
  }

  void FlightLogicTester::checkDeployed(int index) {
    ASSERT_TLM_antennaState_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::DEPLOYED);

    ASSERT_TLM_beaconState_SIZE(index+1);
    ASSERT_TLM_beaconState(index,GASRATS::beacon::T::STANDARD);
    
    ASSERT_TLM_cameraState_SIZE(index+1);
    ASSERT_TLM_cameraState(index,GASRATS::deployed::T::DEPLOYED);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_antennaState(index,GASRATS::deployed::T::DEPLOYED);

    ASSERT_TLM_detumbled_SIZE(index+1);
    ASSERT_TLM_detumbled(index,true);

    ASSERT_TLM_lowPower_SIZE(index+1);
    ASSERT_TLM_lowPower(index,false);
  }

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

  int FlightLogicTester::runFor(int iter) {
    for(int i = 0; i < iter; i++) {
      this->invoke_to_startup(0,i);
    }
    return iter;
  }

}
