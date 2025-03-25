// ======================================================================
// \title  CommunicationAdapter.hpp
// \author devins
// \brief  hpp file for CommunicationAdapter component implementation class
// ======================================================================

#ifndef Components_CommunicationAdapter_HPP
#define Components_CommunicationAdapter_HPP

#include "Components/CommunicationAdapter/CommunicationAdapterComponentAc.hpp"

namespace Components {

  class CommunicationAdapter :
    public CommunicationAdapterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct CommunicationAdapter object
      CommunicationAdapter(
          const char* const compName //!< The component name
      );

      //! Destroy CommunicationAdapter object
      ~CommunicationAdapter();

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

      //! Handler implementation for command TODO
      //!
      //! TODO
      void TODO_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

  };

}

#endif
