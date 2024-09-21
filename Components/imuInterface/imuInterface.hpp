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

      //! Handler implementation for startup
      //!
      //! startup: runs the startup code
      void startup_handler(
          NATIVE_INT_TYPE portNum,  //!< The port number
          NATIVE_UINT_TYPE context  //!< The call order
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
      void checkStatus(Drv::I2cStatus status);

      //! Private variable storing boot time
      //!
      //! time_point, set on construction of the Flight Logic component
      //! holds the time the system was last booted.
      Fw::Time bootTime;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Helper Variables
      // ----------------------------------------------------------------------
      const U32 ADDRESS = 0x6B;
      const U8 CTRL1 = 0x20;
      const U8 CTRL4 = 0x23;
      const U8 X_L = 0x28;
      const U8 X_H = 0x29;
      const U8 Y_L = 0x2A;
      const U8 Y_H = 0x2B;
      const U8 Z_L = 0x2C;
      const U8 Z_H = 0x2D;
      const U8 ALL_ON = 0x0F;

      int calls;

  public:
    void setTime();
  };

}

#endif
