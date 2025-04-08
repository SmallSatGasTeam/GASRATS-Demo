// ======================================================================
// \title  CommunicationAdapter.hpp
// \author devins
// \brief  hpp file for CommunicationAdapter component implementation class
// ======================================================================

#ifndef Components_CommunicationAdapter_HPP
#define Components_CommunicationAdapter_HPP

#include "Components/CommunicationAdapter/CommunicationAdapterComponentAc.hpp"
#include "../../fprime/Drv/LinuxI2cDriver/LinuxI2cDriver.hpp" // need this so it knows what the I2cStatus type is

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

      //! We need to have the first buffer send a success message to kick of the ComQueue
      bool isFirst = true;

      //! Address to write to the transciever
      const U8 ADDRESS = 0x22; // the range for the address is from 0x22 to 0x23 

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

    PRIVATE:

      // ----------------------------------------------------------------------
      // Helper Functions
      // ----------------------------------------------------------------------

      //! checkStatus
      //!
      //! Calls an event based on the status returned from the read/write operation
      Fw::Success checkStatus(Drv::I2cStatus i2cStatus);
  };

}

#endif
