// ======================================================================
// \title  DataCollector.hpp
// \author devin
// \brief  hpp file for DataCollector component implementation class
// ======================================================================

#ifndef Components_DataCollector_HPP
#define Components_DataCollector_HPP

#include "Components/DataCollector/DataCollectorComponentAc.hpp"

namespace Components {

  class DataCollector :
    public DataCollectorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct DataCollector object
      DataCollector(
          const char* const compName //!< The component name
      );

      //! Destroy DataCollector object
      ~DataCollector();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined serial input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for aggregate
      //!
      //! Recieving data from multiple channels
      void aggregate_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          Fw::SerializeBufferBase& buffer //!< The serialization buffer
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
