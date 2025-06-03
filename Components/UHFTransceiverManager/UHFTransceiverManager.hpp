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

      // ----------------------------------------------------------------------
      // User defined functions / variables /
      // ----------------------------------------------------------------------
      int computeFint(double freq_hz);
      F32 computeFfrac(double freq_hz, double Fint);
      U32 computeCRC32(const std::string& input);

      void generateI2cCommand(int Fint, F32 Ffrac, U32 CRC32);

      // Change these values once we actually know what frequency we will be using
      float f_x0 = 26e6; // Crystal Oscillator Frequency
      float outdiv = 8;  // Output Divider
      float npresc = 2; // Prescaler Value

      

  };

}

#endif
