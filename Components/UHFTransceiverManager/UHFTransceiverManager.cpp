// ======================================================================
// \title  UHFTransceiverManager.cpp
// \author ubuntu
// \brief  cpp file for UHFTransceiverManager component implementation class
// ======================================================================

#include "Components/UHFTransceiverManager/UHFTransceiverManager.hpp"
#include <cmath>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  UHFTransceiverManager ::
    UHFTransceiverManager(const char* const compName) :
      UHFTransceiverManagerComponentBase(compName)
  {

  }

  UHFTransceiverManager ::
    ~UHFTransceiverManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void UHFTransceiverManager ::
    configureFrequency_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  int UHFTransceiverManager::computeFint(double freq_hz) {
      double step = (freq_hz * this->outdiv) / (npresc * f_x0);
      return std::floor(step);
  }

  F32 UHFTransceiverManager::computeFfrac(double freq_hz, double Fint) {
      double step = (freq_hz * this->outdiv) / (npresc * f_x0);
      return ((step - Fint) * 524288) + 524288;
  }

  U32 UHFTransceiverManager::computeCRC32(const std::string& input) {
    U32 crc = 0xFFFFFFFF;
    for (unsigned char c : input) {
        crc ^= static_cast<U32>(c);
        for (int i = 0; i < 8; ++i) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320; // reflected poly
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
  }

  

}
