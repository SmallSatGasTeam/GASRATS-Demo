// ======================================================================
// \title  UHFTransceiverManager.hpp
// \author ubuntu
// \brief  hpp file for UHFTransceiverManager component implementation class
// ======================================================================

#ifndef Components_UHFTransceiverManager_HPP
#define Components_UHFTransceiverManager_HPP


#include "Components/UHFTransceiverManager/UHFTransceiverManagerComponentAc.hpp"


namespace Components {

  class UHFTransceiverManager :
    public UHFTransceiverManagerComponentBase
  {
    public:

      // ----------------------------------------------------------------------
      // Constructor / Destructor
      // ----------------------------------------------------------------------

      UHFTransceiverManager(const char* const compName);
      ~UHFTransceiverManager();

      // ----------------------------------------------------------------------
      // Public Methods
      // ----------------------------------------------------------------------

      //! Send a UART buffer
      void transmitData(Fw::Buffer);

      //! Deallocate buffer (typically used by the framer)
      void deallocate_buffer(Fw::Buffer& buffer);

    private:

      // ----------------------------------------------------------------------
      // Command Handlers
      // ----------------------------------------------------------------------

      // \brief configureRFSettings_cmdHandler : This command handler configures the RF settings, ensuring that the frequency, beacon transmission period, and other relevant settings are configured. 
      void configureRFSettings_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) override;
      
      void sendData_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Fw::CmdStringArg& data) override;

      // ----------------------------------------------------------------------
      // Port Handlers
      // ----------------------------------------------------------------------

      void uartRecv_handler(FwIndexType portNum, Fw::Buffer& recvBuffer, const Drv::RecvStatus& recvStatus) override;
      void uartReady_handler(FwIndexType portNum) override;
      // ----------------------------------------------------------------------
      // Response Structure
      // ----------------------------------------------------------------------

      struct Response {
          Fw::String fullResponse;
          bool status;
          U32 size;
      };

      // ----------------------------------------------------------------------
      // Internal Methods
      // ----------------------------------------------------------------------

      //! Send buffer over framer
      void sendDataBuffer(const char* data);

      //! Parse a received buffer into a Response struct
      Response parseBuffer(Fw::Buffer readBuffer);

      //! Log I2C transaction status
      void checkI2cStatus(Drv::I2cStatus status);

      //! Send a command via UART or I2C
      void sendCommand(const char* message, const char* command, bool useUart);

      //! Send I2C command and return read buffer
      Fw::Buffer sendI2cCommand(const char* command, U32 writeSize, U32 readSize);

      //! Format a response buffer and command as a telemetry string
      Fw::String responseToString(U8 command, Fw::Buffer readBuffer);

      //! Apply default transceiver settings on startup
      void configureSettings();

      //! Send raw UART command
      void sendUartCommand(const char* command, U32 writeSize);

      

      // ----------------------------------------------------------------------
      // Configuration Constants
      // ----------------------------------------------------------------------

      U8 ADDRESS = 0x23;

      // UART Write Commands
      const char* WRITE_RADIO_FREQ                    = "ES+W230150E90942 F778726B";
      const char* WRITE_BEACON_TRANSMISSION_PERIOD    = "ES+W23070000003C FDEF1764";
      const char* WRITE_POWER_MODE                    = "ES+W23F4 0AA27131";
      const char* WRITE_SCW_PIPE_ON                   = "ES+W23003033 e5b18b2f";
      const char* WRITE_PIPE_PERIOD                   = "ES+W23060000000A 2fb731c8";

      // UART Read Commands
      const char* READ_RADIO_FREQ                     = "ES+R2301 CB4DD4BE";
      const char* READ_UPTIME                         = "ES+R2302 52448504";
      const char* READ_TRANSMITTED_PACKETS            = "ES+R2303 2543B592";
      const char* READ_RECEIVED_PACKETS               = "ES+R2304 BB272031";
      const char* READ_BEACON_TRANSMISSION_PERIOD     = "ES+R2307 22E716B";
      const char* READ_INTERNAL_TEMP_ASCII            = "ES+R230A 9b48a582";
      const char* READ_SCW                            = "ES+R2300 bc4ae428";
      const char* READ_PIPE_PERIOD                    = "ES+R2306 5529411d";
      const char* READ_POWER_MODE                     = "ES+R23F4 C242FE41";
      const char* READ_SOURCE_CALLSIGN                = "ES+R23F6 2c4c9f6d";
      const char* READ_DESTINATION_CALLSIGN           = "ES+R23F5 b545ced7";

      // ----------------------------------------------------------------------
      // Internal State
      // ----------------------------------------------------------------------

      bool m_reinitialize = false;  // Flag to trigger reinitialization

  };

} // namespace Components

#endif // COMPONENTS_UHFTRANSCEIVERMANAGER_HPP