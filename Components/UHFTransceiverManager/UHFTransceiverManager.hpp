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
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct UHFTransceiverManager object
      UHFTransceiverManager(
          const char* const compName //!< The component name
      );

      //! Destroy UHFTransceiverManager object
      ~UHFTransceiverManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command configureFrequency
      void configureFrequency_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
          
      ) override;

      //! Handler implementation for command transmitData
      void transmitData_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          const Fw::CmdStringArg& data
      ) override;

      // ----------------------------------------------------------------------
      // User defined functions 
      // ----------------------------------------------------------------------

      //! struct Response
      //!
      //! This holds a response from an I2C transaction and a status: OK or ERR
      struct Response {
          Fw::String fullResponse; // Full Response string that was returned
          bool status; // First few characters of response dictate the status
      };

      //! parseResponse
      //!
      //! Method that takes in a readBuffer and parses response by filling in all found values in response. Refer to `struct Response`
      UHFTransceiverManager::Response parseResponse(Fw::Buffer readBuffer);

      void checkI2cStatus(Drv::I2cStatus);

      //! getReadBuffer
      //!
      //! Method that takes in a command and returns the response of that command
      Fw::Buffer getReadBuffer(const char* command, U32 writeSize, U32 readSize);

      //! responseToString
      //!
      //! Method that takes in a command and a readBuffer, formats the readBuffer and command in a string for telemetry
      Fw::String responseToString(U8 command, Fw::Buffer readBuffer);

      //! configureSettings
      //!
      //! Master method that configures all settings for transceiver. Should be called on startup.
      void configureSettings();

      // ----------------------------------------------------------------------
      // User defined variables 
      // ----------------------------------------------------------------------


      // Change these values once we actually know what frequency we will be using
      float FREQUENCY = 430e6; // 430e6 : Chosen radio frequency

      U8 ADDRESS = 0x23; // Address -> 0x23 


      const char* WRITE_RADIO_FREQ = "ES+W230150E90942 F778726B"; // Command to configure radio frequency to 437 MHz (calculated from datasheet example)
      const char* WRITE_BEACON_TRANSMISSION_PERIOD = "ES+W23070000003C FDEF1764"; // Command to configure beacon tranmission period to 60 seconds
      const char* WRITE_POWER_MODE = "ES+W23F4 0AA27131"; // Command that turns on low power mode
      const char* WRITE_SCW_PIPE_ON = "ES+W23003323 feec0437"; // Command that activates pipe mode for SCW
      const char* WRITE_SCW_PIPE_OFF = "ES+W23003303 ccda66b5"; // Command that activates pipe mode for SCW

      // READ Commands
      const char* READ_RADIO_FREQ = "ES+R2301 CB4DD4BE"; // Command to read radio freqency configuration (example from datasheet)
      const char* READ_UPTIME = "ES+R2302 52448504"; // Command to read the device uptime in seconds
      const char* READ_TRANSMITTED_PACKETS = "ES+R2303 2543B592"; // Command to read number of transmitted packets
      const char* READ_RECEIVED_PACKETS = "ES+R2304 BB272031"; // Command to read number of received packets
      const char* READ_BEACON_TRANSMISSION_PERIOD = "ES+R2307 22E716B"; // Command to read beacon transmission period in seconds. 
      const char* READ_INTERNAL_TEMP_ASCII = "ES+R230A 9b48a582"; // Command to read internal temperature measurement
      const char* READ_SCW = "ES+R2300 bc4ae428"; // Command that reads status control word

      const char* READ_POWER_MODE = "ES+R23F4 C242FE41"; // Command that reads the power mode of the device (low power mode)


      // There is also a command on page 87 to read current antenna status which may be useful.


      
      

  };

}

#endif
