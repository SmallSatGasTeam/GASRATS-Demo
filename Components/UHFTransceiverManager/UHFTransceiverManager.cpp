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


    // COMMAND: Read Temperature (Example READ command)
    Fw::Buffer readBuffer = getReadBuffer(this->READ_INTERNAL_TEMP_ASCII, 18, 17); // 18 -> writeSize, 17 -> readSize
    Response temperatureRead = getWriteData(readBuffer);
    this->tlmWrite_response(temperatureRead.data);
    this->deallocate_out(0, readBuffer);

  }

  Fw::String UHFTransceiverManager::responseToString(U8 command, Fw::Buffer readBuffer) {
    // This is a luxury, wait til other methods are finished.
    return NULL;
  } 

  bool UHFTransceiverManager::checkResponse(Response response) {
    // check first 2 characters of response for OK

    const char *data = response.data.toChar();
    
    char charArray[response.length + 1];
    strcpy(charArray, data);

    
    if ((charArray[0] == 79) && (charArray[1] = 75)) {
      return true;
    }
    return false;
  }

  UHFTransceiverManager::Response UHFTransceiverManager::getWriteData(Fw::Buffer readBuffer){
    U8 size = readBuffer.getSize(); //! Size of readBuffer
    Response r; //! Response struct for storing contents and length of readBuffer

    r.data = reinterpret_cast<char*>(readBuffer.getData()); // Convert contents of read buffer to char array
    r.length = size;

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
    
    U8 dataBufferTemp[18];

    // Serialize each byte into the write buffer
    for (int i = 0; i < readSize; i++) {
      dataBufferTemp[i] = static_cast<char>(command[i]);
    }
    dataBufferTemp[readSize] = 0x0D; // Last character in command should always be a carriage return <CR>

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
