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
      // User defined functions / variables /
      // ----------------------------------------------------------------------
      U8 computeFint(double freq_hz);
      std::array<char, 6> computeFfrac(double freq_hz, U8 Fint);
      U32 computeCRC32(std::string& input);
      U32 getLength(const unsigned char* input);

      U8 getWriteData(Fw::Buffer readBuffer);

      char* generateI2cCommand(U8 Fint, std::array<char, 6> Ffrac);
      void checkStatus(Drv::I2cStatus);

      Fw::Buffer getReadBuffer(const char* command);

      // Change these values once we actually know what frequency we will be using
      float f_x0 = 26e6; // Crystal Oscillator Frequency
      float outdiv = 8;  // Output Divider
      float npresc = 2; // Prescaler Value

      float FREQUENCY = 430e6;

      U8 ADDRESS = 0x23; // Default but can be changed to 0x23

      

  };

}

#endif
