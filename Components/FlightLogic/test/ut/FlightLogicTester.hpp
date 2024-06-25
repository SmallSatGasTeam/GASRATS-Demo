// ======================================================================
// \title  FlightLogicTester.hpp
// \author jjhessing
// \brief  hpp file for FlightLogic component test harness implementation class
// ======================================================================

#ifndef Components_FlightLogicTester_HPP
#define Components_FlightLogicTester_HPP

#include "Components/FlightLogic/FlightLogicGTestBase.hpp"
#include "Components/FlightLogic/FlightLogic.hpp"
#include "Components/componentConfig/Constants.hpp"

namespace Components {

  class FlightLogicTester :
    public FlightLogicGTestBase
  {

    public:

      // ----------------------------------------------------------------------
      // Constants
      // ----------------------------------------------------------------------

      // Maximum size of histories storing events, telemetry, and port outputs
      static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = DEPLOY_WAIT_ITER + STARTUP_MAX_ITER + 30;

      // Instance ID supplied to the component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;

      // Queue depth supplied to the component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

    public:

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

      //! Construct object FlightLogicTester
      FlightLogicTester();

      //! Destroy object FlightLogicTester
      ~FlightLogicTester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      //! testTakePic
      //!
      //! Tests the takePic command
      void testTakePic();

      //! testResetFlags
      //!
      //! Tests the resetFlags command
      void testResetFlags();

      //! testSendTransmission
      //!
      //! Tests the sendTransmission command
      void testSendTransmission();

      //! testStartup
      //!
      //! Tests the startup port
      //! This test covers the requirement of waiting 30 minutes before deployment,
      //!   immediately deploying the antenna, deploying the camera, taking a
      //!   picture, and triggering the INITIAL beacon on startup
      void testStartup();

      //! testDetumble
      //!
      //! Tests that the startup function will prompt the microcontroller to detumble
      //!   if it hasn't detumbled yet
      void testDetumble();

      //! testLowPower
      //!
      //! Tests that the startup function will throw the low power flag if EPS voltage
      //!   or current drops below a certain point
      void testLowPower();

      //! testCameraFailure
      //!
      //! Tests that startup will throw a fatal error if the camera fails to deploy
      void testCameraFailure();

      //! testAntennaFailure
      //!
      //! Tests that startup will throw a fatal error if the antenna fails to deploy
      void testAntennaFailure();

      //! testHealthPing
      //!
      //! Tests that the component is pinging the health componenet
      void testHealthPing();

      //! testDataRequest
      //!
      //! Tests the dataRequest port
      void testDataRequest();

      //! testDataRequest
      //!
      //! Tests the recvTransmission port
      void testRecvTransmission();

      //! testSendBeaconState
      //!
      //! Tests the sendBeaconState port
      void testSendBeaconState();

    private:

      // -----------------
      // Helper functions
      // -----------------

      //! checkAllDefault
      //!
      //! Checks that all telemetry matches default values
      //!   index: The index in the history to be checked
      void checkAllDefault(int index);

      //! checkDeployTlm
      //!
      //! Checks that telemetry matches expected mid deploy values
      //!   index: The index in the history to be checked
      void checkDeployTlm(int index);

      //! checkDeployed
      //!
      //! Checks that telemetry matches expected post deploy values
      //!   index: The index in the history to be checked
      void checkDeployed(int index);

      //! runTillDeployed
      //!
      //! Runs startup until past iterations after deploy
      //!   start: Current iteration of startup
      //!   past: Number of iterations to continue past startup
      //!   checks: flag to decide whether or not to run checks each iteration
      int runTillDeployed(int start, int past, bool checks);

      //! runFor
      //!
      //! Runs startup for iter iterations
      //!   iter: number of iterations to run startup
      int runFor(int iter);

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler implementation for deployAntenna
      bool from_deployAntenna_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      );

      //! Handler implementation for deployCamera
      bool from_deployCamera_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      );

      //! Handler implementation for epsHealth
      void from_epsHealth_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& voltage,
          F32& current
      );

      //! Handler implementation for fakeData
      void from_fakeData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 value
      );

      //! Handler implementation for pingOut
      void from_pingOut_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 key //!< Value to return to pinger
      );

      //! Handler implementation for sendTrans
      U32 from_sendTrans_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 value
      );

      //! Handler implementation for takePic
      void from_takePic_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      );

    private:

      // ----------------------------------------------------------------------
      // Helper functions
      // ----------------------------------------------------------------------

      //! Connect ports
      void connectPorts();

      //! Initialize components
      void initComponents();

    private:

      // ----------------------------------------------------------------------
      // Member variables
      // ----------------------------------------------------------------------

      //! The component under test
      FlightLogic component;

      bool powerLow;

      bool antennaFail;
      bool cameraFail;

  };

}

#endif
