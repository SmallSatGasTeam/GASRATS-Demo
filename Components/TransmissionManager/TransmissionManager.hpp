// ======================================================================
// \title  TransmissionManager.hpp
// \author jjhessing
// \brief  hpp file for TransmissionManager component implementation class
// ======================================================================

#ifndef Components_TransmissionManager_HPP
#define Components_TransmissionManager_HPP

#include "Components/TransmissionManager/TransmissionManagerComponentAc.hpp"
#include "Components/componentConfig/Constants.hpp"
#include <cstdlib>

namespace Components {

  class TransmissionManager :
    public TransmissionManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TransmissionManager object
      TransmissionManager(
          const char* const compName //!< The component name
      );

      //! Destroy TransmissionManager object
      ~TransmissionManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for beacon
      //!
      //! Port that calls the beacon
      void beacon_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;
      
      //! Handler implementation for recvData
      //!
      //! Port used to receive data from the ground
      void recvData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 value
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command confirmConnection
      //!
      //! Confirms connection with the ground
      void confirmConnection_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

      //! Handler implementation for command setBeacon
      //!
      //! Command used to set the beacon to a specific state. Only use to set to INITIAL or STANDARD
      void setBeacon_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          GASRATS::beacon state
      ) override;

      //! Handler implementation for command sendTransToGround
      //!
      //! Sends data from the satellite to ground
      void sendTransToGround_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U32 data
      ) override;

    private:

      // ----------------------------------------------------------------------
      // Class variables
      // ----------------------------------------------------------------------

      //! The Scotsman
      //!
      //! For some reason, it's in the FSW requirements to have the initial beacon
      //! send the Scotsman. So here it is for easy use later.
      // Fw::String scotsman = "Show me a Scotsman, who doesn't love the thistle. Show me an Englishman, who doesn't love the rose. Show me a true-blooded Aggie from Utah, who doesn't love the spot . . . Where the sagebrush grows!";

  };

}

#endif
