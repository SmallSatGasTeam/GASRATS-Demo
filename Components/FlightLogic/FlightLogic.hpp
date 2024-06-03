// ======================================================================
// \title  FlightLogic.hpp
// \author jjhessing
// \brief  hpp file for FlightLogic component implementation class
// ======================================================================

#ifndef Components_FlightLogic_HPP
#define Components_FlightLogic_HPP

#include "Components/FlightLogic/FlightLogicComponentAc.hpp"

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

      //! Handler implementation for recvTransmission
      //!
      //! recvTransmission: Flight Logic is pinged to know that a message has been received
      void recvTransmission_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command TODO
      //!
      //! TODO
      void TODO_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

      //! Handler implementation for command takePic
      //!
      //! takePic: sends command to camera to take a picture
      void takePic_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

      //! Handler implementation for command sendTransmission
      //!
      //! sendTransmission: commands system to transmit for x miliseconds
      void sendTransmission_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U32 duration
      ) override;

      //! Handler implementation for command resetFlags
      //!
      //! resetFlags: resets startup flags to original condition
      void resetFlags_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

  };

}

#endif
