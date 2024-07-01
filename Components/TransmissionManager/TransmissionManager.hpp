// ======================================================================
// \title  TransmissionManager.hpp
// \author jjhessing
// \brief  hpp file for TransmissionManager component implementation class
// ======================================================================

#ifndef Components_TransmissionManager_HPP
#define Components_TransmissionManager_HPP

#include "Components/TransmissionManager/TransmissionManagerComponentAc.hpp"

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

      //! Handler implementation for recvData
      //!
      //! This is synchronous because it can return a value if it has successfully received data
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

  };

}

#endif
