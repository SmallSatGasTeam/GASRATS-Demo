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
      void testTakePic();

      void testResetFlags();

      void testSendTransmission();

      void testStartup();

      void testDetumble();

      void testLowPower();

      void testCameraFailure();

      void testAntennaFailure();

      void testHealthPing();

      void testDataRequest();

      void testRecvTransmission();

      void testSendBeaconState();

    private:

      // -----------------
      // Helper functions
      // -----------------

      void checkAllDefault(int index);

      void checkDeployTlm(int index);

      void checkDeployed(int index);

      int runTillDeployed(int start, int past, bool checks);

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
