// ======================================================================
// \title  UHFTransceiverManager.cpp
// \author ubuntu
// \brief  cpp file for UHFTransceiverManager component implementation class
// ======================================================================

#include "Components/UHFTransceiverManager/UHFTransceiverManager.hpp"
#include <cstring>
#include <unistd.h>

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
    configureRFSettings_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    configureSettings();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void UHFTransceiverManager ::
    transmitData_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        const Fw::CmdStringArg& data
    )
  {
    

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void UHFTransceiverManager ::
    uartRecv_handler(
        FwIndexType portNum,
        Fw::Buffer& recvBuffer,
        const Drv::RecvStatus& recvStatus
    )
  {
    Response r = parseResponse(recvBuffer);
    this->tlmWrite_response1(r.fullResponse);
    this->tlmWrite_recvStatus(recvStatus);
    // if (recvBuffer.getSize() > 0) {
    //   this->deallocate_out(0, recvBuffer);
    // }
    // TODO
  }


  // ----------------------------------------------------------------------
  // User defined methods
  // ----------------------------------------------------------------------

  void UHFTransceiverManager::configureSettings() {
    Fw::String str("---Now reading pipe mode configuration (should be 5)---");
    this->log_ACTIVITY_HI_somethingHappened(str);
    Fw::Buffer readBuffer1 = getReadBuffer(this->READ_SCW, strlen(this->READ_SCW)+1, 64, false); // 18 -> writeSize, 17 -> readSize
    Response r = parseResponse(readBuffer1);
    this->log_ACTIVITY_HI_somethingHappened(r.fullResponse);
    if (readBuffer1.getSize() > 0) {
      this->deallocate_out(0, readBuffer1);
    }

    // ---------------------------------------------------------------------------------------
    sleep(10);

    Fw::String str5("---Test UART command---");
    this->log_ACTIVITY_HI_somethingHappened(str5);
    Fw::Buffer readBuffer2 = getReadBuffer(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1, 64, true); // 18 -> writeSize, 17 -> readSize
    // if (readBuffer2.getSize() > 0) {
    //   this->deallocate_out(0, readBuffer2);
    // }
    
    // -----------------------------------------------------------------------------------------
    

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

  Fw::Buffer UHFTransceiverManager::getReadBuffer(const char* command, U32 writeSize, U32 readSize, bool UARTMODE) {
    Fw::Buffer writeBuffer = this->allocate_out(0, writeSize);
    Fw::Buffer readBuffer = this->allocate_out(0, readSize);

    // Check to see if buffers were allocated properly, if not deallocate the possible memory allocated, and log a warning.
    if (writeBuffer.getSize() < writeSize || readBuffer.getSize() < readSize) {
      if (writeBuffer.getSize() > 0) {
        this->deallocate_out(0, writeBuffer);
      }
      if (readBuffer.getSize() > 0) {
        this->deallocate_out(0, readBuffer);
      }
      this->log_WARNING_LO_MemoryAllocationFailed();
      return Fw::Buffer(); // return a default buffer early to avoid continuing
    }
    

    Fw::SerializeBufferBase& sb = writeBuffer.getSerializeRepr();
    sb.setBuffLen(writeSize);


    //! Prepare BufferBase
    sb.resetSer();
    
    U8 dataBufferTemp[writeSize];

    // Serialize each byte into the write buffer
    for (int i = 0; i < (writeSize-1); i++) {
      dataBufferTemp[i] = static_cast<char>(command[i]);
    }
    dataBufferTemp[writeSize-1] = 0x0D; // Last character in command should always be a carriage return <CR>

    // Fw::String str(reinterpret_cast<const char*>(dataBufferTemp));

    // this->tlmWrite_response(str);

    sb.serialize(dataBufferTemp, writeSize, true);

    if (UARTMODE) {
      this->uartSend_out(0, writeBuffer);


    } else {
      this->checkI2cStatus(i2cWrite_out(0, this->ADDRESS, writeBuffer));
      this->checkI2cStatus(i2cRead_out(0, this->ADDRESS, readBuffer));
    }
    
    // Deallocate the writeBuffer, we are done using it.
    // if (writeBuffer.getSize() > 0) {
    //   this->deallocate_out(0, writeBuffer);
    // }
    
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
