// ======================================================================
// \title  imuInterface.hpp
// \author devin
// \brief  hpp file for imuInterface component implementation class
// ======================================================================

#ifndef Components_imuInterface_HPP
#define Components_imuInterface_HPP

#include "Components/imuInterface/imuInterfaceComponentAc.hpp"
#include "Components/componentConfig/Constants.hpp"

namespace Components {

  class imuInterface :
    public imuInterfaceComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct imuInterface object
      imuInterface(
          const char* const compName //!< The component name
      );

      //! Destroy imuInterface object
      ~imuInterface();

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
