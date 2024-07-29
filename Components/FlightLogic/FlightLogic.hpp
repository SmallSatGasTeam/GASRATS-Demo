// ======================================================================
// \title  FlightLogic.hpp
// \author jjhessing
// \brief  hpp file for FlightLogic component implementation class
// ======================================================================

#ifndef Components_FlightLogic_HPP
#define Components_FlightLogic_HPP

#include "Components/FlightLogic/FlightLogicComponentAc.hpp"
#include "Components/componentConfig/Constants.hpp"
#include <chrono>
#include <ratio>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iostream>

namespace Components {

  class FlightLogic :
    public FlightLogicComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct FlightLogic object
      FlightLogic(
          const char* const compName //!< The component name
      );

      //! Destroy FlightLogic object
      ~FlightLogic();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for dataRequest
      //!
      //! dataRequest: receives a ping from the data collector to send out data
      U32 dataRequest_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 value
      ) override;

      //! Handler implementation for startup
      //!
      //! recvTransmission: Flight Logic is pinged to know that a message has been received
      //! startup: runs the startup code
      void startup_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      //! Handler implementation for beaconState
      //!
      //! beaconState : sets the beacon if prompted, then returns the state of the beacon
      GASRATS::beacon beaconState_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const GASRATS::beacon& value
      ) override;

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for pingIn
      //!
      //! pingIn : receives health pings
      void pingIn_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 key //!< Value to return to pinger
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command takePic
      //!
      //! takePic: sends command to camera to take a picture
      void takePic_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

      //! Handler implementation for command resetFlags
      //!
      //! resetFlags: resets startup flags to original condition
      void resetFlags_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined internal interfaces
      // ----------------------------------------------------------------------

      //! Handler implementation for saveFlags
      //!
      //! Saves flags to a file
      void saveFlags_internalInterfaceHandler() override;

    PRIVATE:
      //! Private variable tracking antenna deployment
      //!
      //! GASRATS::deployed showing antenna deployment state
      GASRATS::deployed::T antennaState;

      //! Private variable tracking camera deployment
      //!
      //! GASRATS::beacon showing if initial beacon is active
      GASRATS::beacon::T beaconState;

      //! Private variable storing boot time
      //!
      //! time_point, set on construction of the Flight Logic component
      //! holds the time the system was last booted.
      std::chrono::system_clock::time_point bootTime;

      //! Private variable tracking camera deployment
      //!
      //! GASRATS::deployed showing camera deployment state
      GASRATS::deployed::T cameraState;

      //! Private variable storing detumble state
      //!
      //! bool, true when satellite has been detumbled
      bool detumbled;

      //! Private variable storing epsCurrent
      //!
      //! F32, is passed by reference to epsCurrent
      F32 epsCurrent;

      //! Private variable storing epsVoltage
      //!
      //! F32, is passed by reference to epsHealthManager
      F32 epsVoltage;
      //! Private variable storing times startup has failed
      //!
      //! U16
      U16 failCount;

      //! Private variable tracking low power mode
      //!
      //! bool, true when in low power mode
      bool lowPower;

      #ifdef VIRTUAL
        char filePath [22] = "../../files/flags.txt";
      #else
        char filePath [34] = "/home/gas/FSWDeployment/flags.txt";
      #endif
  };

}

#endif
