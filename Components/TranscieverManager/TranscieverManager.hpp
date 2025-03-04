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
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command STARTUP
      //!
      //! Startup the transciever
      void STARTUP_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

  };

}

#endif
