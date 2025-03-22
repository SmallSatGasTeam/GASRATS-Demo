// ======================================================================
// \title  TranscieverManager.hpp
// \author devins
// \brief  hpp file for TranscieverManager component implementation class
// ======================================================================

#ifndef Components_TranscieverManager_HPP
#define Components_TranscieverManager_HPP

#include "Components/TranscieverManager/TranscieverManagerComponentAc.hpp"

namespace Components {

  class TranscieverManager :
    public TranscieverManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TranscieverManager object
      TranscieverManager(
          const char* const compName //!< The component name
      );

      //! Destroy TranscieverManager object
      ~TranscieverManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for comDataIn
      //!
      //! Port for recieving Fw:Buffer objects for outgoing transmission
      Drv::SendStatus comDataIn_handler(
          FwIndexType portNum, //!< The port number
          Fw::Buffer& sendBuffer //!< Data to send
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command STARTUP
      //!
      //! Startup the transciever
      void STARTUP_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

      //! Sends an event based on the returned status from an i2c read or write operation
      void checkStatus(Drv::I2cStatus status);

      int calls;

  };

}

#endif
