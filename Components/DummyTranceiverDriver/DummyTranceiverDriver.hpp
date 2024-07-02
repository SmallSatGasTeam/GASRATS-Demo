// ======================================================================
// \title  DummyTranceiverDriver.hpp
// \author jjhessing
// \brief  hpp file for DummyTranceiverDriver component implementation class
// ======================================================================

#ifndef Components_DummyTranceiverDriver_HPP
#define Components_DummyTranceiverDriver_HPP

#include "Components/DummyTranceiverDriver/DummyTranceiverDriverComponentAc.hpp"

namespace Components {

  class DummyTranceiverDriver :
    public DummyTranceiverDriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct DummyTranceiverDriver object
      DummyTranceiverDriver(
          const char* const compName //!< The component name
      );

      //! Destroy DummyTranceiverDriver object
      ~DummyTranceiverDriver();

    PRIVATE:
    

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for sendTransToGround
      void sendTransToGround_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 value //!< The serialization buffer
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command sendTransToSatellite
      void sendTransToSatellite_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U32 data
      ) override;

  };

}

#endif
