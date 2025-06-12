// ======================================================================
// \title  UHFTransceiverManager.cpp
// \author ubuntu
// \brief  cpp file for UHFTransceiverManager component implementation class
// ======================================================================

#include "Components/UHFTransceiverManager/UHFTransceiverManager.hpp"
#include <cstring>

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
    configureSettings();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void UHFTransceiverManager::configureSettings() {
    // COMMAND: Configure radio frequency
    // Current this results in a successful write, but an error when reading. Its because the size for the readBuffer (3rd argument) is not valid therefore it will just keep reading until the buffer is full.
    // Need to find a way to allow this function to accomodate for variable read sizes. 
    Fw::Buffer readBuffer1 = getReadBuffer(this->READ_RADIO_FREQ, strlen(this->READ_RADIO_FREQ)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response radioFrequencyResponse = parseResponse(readBuffer1);
    // this->tlmWrite_response(radioFrequencyResponse.fullResponse);
    // this->deallocate_out(0, readBuffer1);

    // COMMAND: Read Temperature (Example READ command)
    // Fw::Buffer readBuffer2 = getReadBuffer(this->READ_INTERNAL_TEMP_ASCII, 18, 17); // 18 -> writeSize, 17 -> readSize
    // Response temperatureRead = parseResponse(readBuffer2);
    // this->tlmWrite_response(temperatureRead.fullResponse);
    // this->deallocate_out(0, readBuffer2);

  }

  Fw::String UHFTransceiverManager::responseToString(U8 command, Fw::Buffer readBuffer) {
    // This is a luxury, wait til other methods are finished.
    return NULL;
  } 

  UHFTransceiverManager::Response UHFTransceiverManager::parseResponse(Fw::Buffer readBuffer){
    U8* data = static_cast<U8*>(readBuffer.getData());
    U8 size = readBuffer.getSize(); //! Size of readBuffer
    Response r; //! Response struct for storing contents and length of readBuffer

    r.fullResponse = reinterpret_cast<char*>(readBuffer.getData()); // Convert contents of read buffer to char array

    // Logic to determine what data is found in the fullResponse.
    // Checking status
    if (data[0] != 79) {
      r.status = false;
    } else {
      r.status = true;
    }


    return r;
  }

  Fw::Buffer UHFTransceiverManager::getReadBuffer(const char* command, U32 writeSize, U32 readSize) {
    Fw::Buffer writeBuffer = this->allocate_out(0, writeSize);
    Fw::Buffer readBuffer = this->allocate_out(0, readSize);

    // Check to see if buffers were allocated properly, if not deallocate the possible memory allocated, and log a warning.
    if (writeBuffer.getSize() < writeSize || readBuffer.getSize() < readSize) {
      this->deallocate_out(0, writeBuffer);
      this->deallocate_out(0, readBuffer);
      this->log_WARNING_LO_MemoryAllocationFailed();
    }
    

    Fw::SerializeBufferBase& sb = writeBuffer.getSerializeRepr();
    sb.setBuffLen(writeSize);


    //! Prepare BufferBase
    sb.resetSer();
    
    U8 dataBufferTemp[writeSize];

    // Serialize each byte into the write buffer
    for (int i = 0; i < writeSize; i++) {
      dataBufferTemp[i] = static_cast<char>(command[i]);
    }
    dataBufferTemp[writeSize] = 0x0D; // Last character in command should always be a carriage return <CR>

    sb.serialize(dataBufferTemp, writeSize, true);

    this->checkI2cStatus(i2cWrite_out(0, this->ADDRESS, writeBuffer));
    this->checkI2cStatus(i2cRead_out(0, this->ADDRESS, readBuffer));

    // Deallocate the writeBuffer, we are done using it.
    this->deallocate_out(0, writeBuffer);
    
    // Return the data (read) buffer
    return readBuffer;
  }

  void UHFTransceiverManager::checkI2cStatus(Drv::I2cStatus i2cStatus) {
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
