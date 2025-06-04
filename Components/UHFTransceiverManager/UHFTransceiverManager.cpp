// ======================================================================
// \title  UHFTransceiverManager.cpp
// \author ubuntu
// \brief  cpp file for UHFTransceiverManager component implementation class
// ======================================================================

#include "Components/UHFTransceiverManager/UHFTransceiverManager.hpp"
#include <cmath>
#include <zlib.h>
#include <stdio.h>
#include <iomanip>

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
        U32 cmdSeq,
        F32 frequency
    )
  {
    // U8 Fint = computeFint(FREQUENCY);
    // std::array<char, 6>  Ffrac = computeFfrac(FREQUENCY, Fint);
    // std::string command = generateI2cCommand(Fint, Ffrac);
    // std::string crc = std::to_string(computeCRC32(command));
    // command.append(crc);
    // command.append("\r");

    // U8 temp[24];
    // for (int i = 0; i < command.size(); i++) {
    //   temp[i] = command[i];
    // }

    const char* cmd1 = "ES+W230150E90942 36F6ADAB\r";
    Fw::Buffer readBuffer = getReadBuffer(cmd1);
    U8 byte_write_1 = getWriteData(readBuffer);
    this->tlmWrite_readConfiguration1(byte_write_1);


    const char* cmd2 = "ES+R2301 CA8FBE89\r";
    Fw::Buffer readBuffer2 = getReadBuffer(cmd2);
    U8 byte_read_1 = getWriteData(readBuffer2);
    this->tlmWrite_readConfiguration2(byte_read_1);


    
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  U8 UHFTransceiverManager::computeFint(double freq_hz) {
      double step = (freq_hz * this->outdiv) / (npresc * f_x0);
      return std::floor(step);
  }

  std::array<char, 6> UHFTransceiverManager::computeFfrac(double freq_hz, U8 Fint) {
      double step = (freq_hz * this->outdiv) / (npresc * f_x0);
      U32 f_frac = ((step - Fint) * 524288) + 524288;

      f_frac &= 0xFFFFFF;
      std::array<char, 6> nibbles;

      // Pack as 3-byte hex string, LSB-first
      nibbles[1] = (f_frac & 0x0F);
      nibbles[0] = ((f_frac >> 4) & 0x0F);

      nibbles[3] = ((f_frac >> 8) & 0x0F);
      nibbles[2] = ((f_frac >> 12) & 0x0F);

      nibbles[5] = ((f_frac >> 16) & 0x0F);
      nibbles[4] = ((f_frac >> 20));

      for (char& nibble : nibbles) {
        if (nibble >= 10) {
            nibble = 'A' + (nibble - 10);
        }
      }
      return nibbles;

  }

  U32 UHFTransceiverManager::computeCRC32(std::string& input) {
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

  // char* UHFTransceiverManager::generateI2cCommand(U8 Fint, std::array<char, 6>  Ffrac) {
  //   char command[24];

  //   command[0] = 'E';
  //   command[1] = 'S';
  //   command[2] = '+';
  //   command[3] = 'W';
  //   command[4] = '2';    
  //   command[5] = '3';
  //   command[6] = '0';
  //   command[7] = '1';
    
  //   for (int i = 0; i < 6; i++) {
  //     command[i+8] = Ffrac[i];
  //   }

  //   command[14] = Fint;
  //   command << ' ';

  //   return command.str();
  // }

  U8 UHFTransceiverManager::getWriteData(Fw::Buffer readBuffer){
    //! Get data out of the buffer
    U8* data = reinterpret_cast<U8*>(readBuffer.getData());
    U8 byte_1 = data[0];
    U8 byte_2 = data[1];

    U8 byte_3 = data[2];
    U8 byte_4 = data[3];
    U8 byte_5 = data[4];
    U8 byte_6 = data[5];
    U8 byte_7 = data[6];
    U8 byte_8 = data[7];

    
    //! Deallocate read buffer
    this->deallocate_out(0, readBuffer);

    return byte_1;
  }

  Fw::Buffer UHFTransceiverManager::getReadBuffer(const char* command) {
    const U32 writeBuffer_size = 24; // Buffer for command number
    const U32 readBuffer_size = 24; // Buffer for storing the contents of what is returned

    Fw::Buffer writeBuffer = this->allocate_out(0, writeBuffer_size);
    Fw::Buffer readBuffer = this->allocate_out(0, readBuffer_size);


    // Check to see if buffers were allocated properly, if not deallocate the possible memory allocated, and log a warning.
    if (writeBuffer.getSize() < writeBuffer_size || readBuffer.getSize() < readBuffer_size) {
      this->deallocate_out(0, writeBuffer);
      this->deallocate_out(0, readBuffer);
      this->log_WARNING_LO_MemoryAllocationFailed();
    }
    

    Fw::SerializeBufferBase& sb = writeBuffer.getSerializeRepr();
    Fw::SerializeBufferBase& sb1 = readBuffer.getSerializeRepr();      

    U8 reg = this->ADDRESS; //!< I2c slave address for the device

    //! Prepare both BufferBases
    sb.resetSer();
    sb1.resetDeser();
    sb1.resetSer();
    


    // for (int i = 0; i < 24; i++) {
    //   sb.serialize(command[i]); //!< Serialize the command into the writeBuffer
    // }

    // Serialize each byte into the write buffer
    for (U32 i = 0; i < strlen(command); i++) {
      sb.serialize(static_cast<U8>(command[i]));
    }


    
    //! Perform i2c read-write, and check the status of it:
    //! i2c read-write takes the following parameters: 
    //! (1) port num  (2) address  (3) writeBuffer (command)  (4) readBuffer (buffer receiving sensor data)
    this->checkStatus(this->i2cReadWrite_out(0, reg, writeBuffer, readBuffer));

    // Deallocate the memory in the writeBuffer, we are done using it.
    this->deallocate_out(0, writeBuffer);
    
    // Return the data (read) buffer
    return readBuffer;
  }

  void UHFTransceiverManager::checkStatus(Drv::I2cStatus i2cStatus) {
    switch (i2cStatus) {
      case Drv::I2cStatus::I2C_OK:
        this->log_ACTIVITY_HI_UHFSuccess();
        break;

      case Drv::I2cStatus::I2C_ADDRESS_ERR:
        this->log_WARNING_HI_UHFAddressFailure();
        break;

      case Drv::I2cStatus::I2C_WRITE_ERR:
        this->log_WARNING_HI_UHFWriteError();
        break;

      case Drv::I2cStatus::I2C_READ_ERR:
        this->log_WARNING_HI_UHFReadError();
        break;

      case Drv::I2cStatus::I2C_OPEN_ERR:
        this->log_WARNING_HI_UHFOpenError();
        break;

      case Drv::I2cStatus::I2C_OTHER_ERR:
       this->log_WARNING_HI_UHFOtherError();
       break;
      
      default:
        break;
    }
  }

  

}
