// ======================================================================
// \title  UHFTransceiverManager.cpp
// \author ubuntu
// \brief  cpp file for UHFTransceiverManager component implementation class
// ======================================================================

#include "Components/UHFTransceiverManager/UHFTransceiverManager.hpp"
#include <cstring>
#include <unistd.h>
#include <string>
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
    Fw::String str("---uartRecv_handler was used---");
    this->log_ACTIVITY_HI_debuggingEvent(str);

    Response r = parseResponse(recvBuffer);
    // this->tlmWrite_response1(r.fullResponse);
    this->log_ACTIVITY_HI_debuggingEvent(r.fullResponse);
    // this->tlmWrite_recvStatus(recvStatus);
    logEvent(recvBuffer);
    if (recvBuffer.isValid()) {
      this->deallocate_out(0, recvBuffer);
    }
  }


  // ----------------------------------------------------------------------
  // User defined methods
  // ----------------------------------------------------------------------

  void UHFTransceiverManager::configureSettings() {
    // Sending I2C Command
    // ---------------------------------------------------------------------------------------

      Fw::String str1("---Reading source callsign---");
      this->log_ACTIVITY_HI_debuggingEvent(str1);
      Fw::Buffer readBuffer1 = sendI2cCommand(this->READ_SOURCE_CALLSIGN, strlen(this->READ_SOURCE_CALLSIGN)+1, 64); 
      Response r = parseResponse(readBuffer1);
      this->log_ACTIVITY_HI_debuggingEvent(r.fullResponse);
      logEvent(readBuffer1);
      if (readBuffer1.isValid()) {
        this->deallocate_out(0, readBuffer1);
      }

      Fw::String str2("---Reading destination callsign---");
      this->log_ACTIVITY_HI_debuggingEvent(str2);
      Fw::Buffer readBuffer2 = sendI2cCommand(this->READ_DESTINATION_CALLSIGN, strlen(this->READ_DESTINATION_CALLSIGN)+1, 64); 
      Response r2 = parseResponse(readBuffer2);
      this->log_ACTIVITY_HI_debuggingEvent(r2.fullResponse);
      logEvent(readBuffer2);
      if (readBuffer1.isValid()) {
        this->deallocate_out(0, readBuffer2);
      }

    // Fw::String str1("---Reading pipe period (should be 10 secs)---");
    // this->log_ACTIVITY_HI_debuggingEvent(str1);
    // Fw::Buffer readBuffer1 = sendI2cCommand(this->READ_PIPE_PERIOD, strlen(this->READ_PIPE_PERIOD)+1, 64); 
    // Response r = parseResponse(readBuffer1);
    // this->log_ACTIVITY_HI_debuggingEvent(r.fullResponse);
    // logEvent(readBuffer1);
    // if (readBuffer1.isValid()) {
    //   this->deallocate_out(0, readBuffer1);
    // }

    // Fw::String str4("---activating PIPE Mode---");
    // this->log_ACTIVITY_HI_debuggingEvent(str4);
    // Fw::Buffer readBuffer4 = sendI2cCommand(this->WRITE_SCW_PIPE_ON, strlen(this->WRITE_SCW_PIPE_ON)+1, 64); 
    // Response r4 = parseResponse(readBuffer4);
    // this->log_ACTIVITY_HI_debuggingEvent(r4.fullResponse);
    // logEvent(readBuffer4);
    // if (readBuffer1.isValid()) {
    //   this->deallocate_out(0, readBuffer4);
    // }

    // sleep (7);

    // Fw::String str5("---Reading temperature while in PIPE Mode---");
    // this->log_ACTIVITY_HI_debuggingEvent(str5);

    // sendUartCommand(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1); 
    // sleep(2);
    // sendUartCommand(this->ANTENNA_TEST_DATA1, strlen(this->ANTENNA_TEST_DATA1)+1); 
    // sleep(2);
    // sendUartCommand(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1); 
    // sleep(2); // Necessary so UART has time to use the receive port before we read again
    // sendUartCommand(this->ANTENNA_TEST_DATA2, strlen(this->ANTENNA_TEST_DATA2)+1); 
    // sleep(2); // Necessary so UART has time to use the receive port before we read again


    // sendUartCommand(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1); 
    // sleep(2); // Necessary so UART has time to use the receive port before we read again
    // sendUartCommand(this->ANTENNA_TEST_DATA3, strlen(this->ANTENNA_TEST_DATA3)+1); 
    // sleep(2); // Necessary so UART has time to use the receive port before we read again
    // sendUartCommand(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1); 
    // sleep(2); // Necessary so UART has time to use the receive port before we read again
    // sendUartCommand(this->ANTENNA_TEST_DATA4, strlen(this->ANTENNA_TEST_DATA4)+1); 
    // sleep(2); // Necessary so UART has time to use the receive port before we read again
    // sendUartCommand(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1); 






    // -----------------------------------------------------------------------------------------
      
    sleep(1); // Necessary so UART has time to use the receive port before we read again


    // NOTES FOR MONDAY!
    // All of the commands above work:
    //   If you try to run them again know it will read 11 secs for pipe period because I didn't change it back.
    //   Something to work on: Integrate Devin's framer because currently the UART responses were limited to 8 bytes (meaning some responses got cut off) run the code to see what I mean.
    //   Then try to see if you can turn on pipe mode and get a signal.



  }

  void UHFTransceiverManager::logEvent(Fw::Buffer buffer) {
    U32 size = buffer.getSize();
    std::string text = "Size of Buffer: " + std::to_string(size);
    Fw::String str(text.c_str());
    this->log_ACTIVITY_HI_debuggingEvent(str);
  }    

  UHFTransceiverManager::Response UHFTransceiverManager::parseResponse(Fw::Buffer readBuffer){
    U8* data = static_cast<U8*>(readBuffer.getData());
    U8 size = readBuffer.getSize(); //! Size of readBuffer
    Response r; //! Response struct for storing contents and length of readBuffer

    r.fullResponse = reinterpret_cast<char*>(readBuffer.getData()); // Convert contents of read buffer to char array
    r.size = size;
    // Logic to determine what data is found in the fullResponse.
    // Checking status
    if (data[0] != 79) {
      r.status = false;
    } else {
      r.status = true;
    }


    return r;
  }

  Fw::Buffer UHFTransceiverManager::sendI2cCommand(const char* command, U32 writeSize, U32 readSize) {
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
    
    // Prepare writeBuffer to be serialized
    Fw::SerializeBufferBase& sb = writeBuffer.getSerializeRepr();
    sb.setBuffLen(writeSize);
    sb.resetSer();
  
    // Serialize each byte into the write buffer
    U8 dataBufferTemp[writeSize];
    for (int i = 0; i < (writeSize-1); i++) {
      dataBufferTemp[i] = static_cast<char>(command[i]);
    }
    dataBufferTemp[writeSize-1] = 0x0D; // Last character in command should always be a carriage return <CR>
    sb.serialize(dataBufferTemp, writeSize, true);

    // Send the serialized command out over I2C and get the read buffer back
    this->checkI2cStatus(i2cWrite_out(0, this->ADDRESS, writeBuffer));
    this->checkI2cStatus(i2cRead_out(0, this->ADDRESS, readBuffer));
    
    // Debugging function that logs the size of write buffer
    logEvent(writeBuffer);

    // Deallocate the writeBuffer, we are done using it.
    if (writeBuffer.isValid()) {
      this->deallocate_out(0, writeBuffer);
    }
    
    // Return the data (read) buffer
    return readBuffer;
  }

  void UHFTransceiverManager::sendUartCommand(const char* command, U32 writeSize) {
    Fw::Buffer writeBuffer = this->allocate_out(0, writeSize);

    // Check to see if buffers were allocated properly, if not deallocate the possible memory allocated, and log a warning.
    if (writeBuffer.getSize() < writeSize) {
      this->log_WARNING_LO_MemoryAllocationFailed();
      if (writeBuffer.getSize() > 0) {
        this->deallocate_out(0, writeBuffer);
      }
    }
    
    // Prepare writeBuffer to be serialized
    Fw::SerializeBufferBase& sb = writeBuffer.getSerializeRepr();
    sb.setBuffLen(writeSize);
    sb.resetSer();
    

    // Serialize each byte into the write buffer
    U8 dataBufferTemp[writeSize];
    for (int i = 0; i < (writeSize-1); i++) {
      dataBufferTemp[i] = static_cast<char>(command[i]);
    }
    dataBufferTemp[writeSize-1] = 0x0D; // Last character in command should always be a carriage return <CR>
    sb.serialize(dataBufferTemp, writeSize, true);

    // Send serialized command out over UART
    this->uartSend_out(0, writeBuffer);
    
    // Debugging function that logs the size of write buffer
    logEvent(writeBuffer);

    // It appears that you do not have to deallocate UART Buffers?
    // If you do it fails
    
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
