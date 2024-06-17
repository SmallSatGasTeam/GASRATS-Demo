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

  };

}

#endif
