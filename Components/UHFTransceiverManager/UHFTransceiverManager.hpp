// ======================================================================
// \title  UHFTransceiverManager.hpp
// \author ubuntu
// \brief  hpp file for UHFTransceiverManager component implementation class
// ======================================================================

#ifndef Components_UHFTransceiverManager_HPP
#define Components_UHFTransceiverManager_HPP


#include "Components/UHFTransceiverManager/UHFTransceiverManagerComponentAc.hpp"
#include "string"
#include "sstream"
#include <array>


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
          U32 cmdSeq, //!< The command sequence number
          F32 frequency
      ) override;

      // ----------------------------------------------------------------------
      // User defined functions 
      // ----------------------------------------------------------------------

      U8* getWriteData(Fw::Buffer readBuffer);

      void checkI2cStatus(Drv::I2cStatus);

      Fw::Buffer getReadBuffer(const char* command);



      // ----------------------------------------------------------------------
      // User defined variables 
      // ----------------------------------------------------------------------

      // Preset
      float FREQ_X0 = 26e6; // 26e6 : Crystal Oscillator Frequency
      float OUTDIV = 8;  // 8 : Output Divider
      float NPRESC = 2; // 2 : Prescaler Value

      // Change these values once we actually know what frequency we will be using
      float FREQUENCY = 430e6; // 430e6 : Chosen radio frequency

      U8 ADDRESS = 0x23; // Default but can be changed to 0x23

      const char* WRITE_RADIO_FREQ = "ES+W230150E90942 F778726B\r"; // Command to configure radio frequency to 495 MHz (calculated from datasheet example)
      const char* WRITE_BEACON_TRANSMISSION_PERIOD = "ES+W23070000003C FDEF1764\r"; // Command to configure beacon tranmission period to 60 seconds
      const char* WRITE_POWER_MODE = "ES+W23F4 AA27131\r"; // Command that turns on low power mode

      // READ Commands
      const char* READ_RADIO_FREQ = "ES+R2301 CB4DD4BE\r"; // Command to read radio freqency configuration (example from datasheet)
      const char* READ_UPTIME = "ES+R2302 52448504\r"; // Command to read the device uptime in seconds
      const char* READ_TRANSMITTED_PACKETS = "ES+R2303 2543B592\r"; // Command to read number of transmitted packets
      const char* READ_RECEIVED_PACKETS = "ES+R2304 BB272031\r"; // Command to read number of received packets
      const char* READ_BEACON_TRANSMISSION_PERIOD = "ES+R2307 22E716B\r"; // Command to read beacon transmission period in seconds. 
      const char* READ_INTERNAL_TEMP_ASCII = "ES+R230A 9B48A582\r"; // Command to read internal temperature measurement


      const char* READ_POWER_MODE = "ES+R23F4 C242FE41\r"; // Command that reads the power mode of the device (low power mode)


      // There is also a command on page 87 to read current antenna status which may be useful.



      

  };

}

#endif
