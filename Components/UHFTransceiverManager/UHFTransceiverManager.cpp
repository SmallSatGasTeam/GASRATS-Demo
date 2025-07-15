// ======================================================================
// \title  UHFTransceiverManager.cpp
// \author ubuntu
// \brief  cpp file for UHFTransceiverManager component implementation class
// ======================================================================

#include "Components/UHFTransceiverManager/UHFTransceiverManager.hpp"
#include <cstring>
#include <unistd.h>
#include <string>
#include "Fw/Types/BasicTypes.hpp"

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
    printf("UHFTransceiverManager::configureRFSettings_cmdHandler(opCode=%d, cmdSeq=%d)\n", opCode, cmdSeq);
    configureSettings();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void UHFTransceiverManager ::
    sendData_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        const Fw::CmdStringArg& data
    )
  {
    Fw::String str("---sendData_cmdHandler was used---");
    this->log_ACTIVITY_HI_debuggingEvent(str);
    
    const char* dataPtr = data.toChar();
    U32 size = strlen(dataPtr) + 1; // +1 for carriage return
    sendUartCommand(data.toChar(), size);

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



    Response r = parseBuffer(recvBuffer);
    this->log_ACTIVITY_HI_debuggingEvent(r.fullResponse);

    if (recvBuffer.isValid()) {
      this->deallocate_out(0, recvBuffer);
    }

  }

  void UHFTransceiverManager ::
    uartReady_handler(FwIndexType portNum)
  {
    Fw::String str("---uartReady_handler was used---");
    this->log_ACTIVITY_HI_debuggingEvent(str);

    Fw::Success radioSuccess = Fw::Success::SUCCESS;
    if (this->isConnected_comStatus_OutputPort(0) && this->m_reinitialize) {
        this->m_reinitialize = false;
        this->comStatus_out(0, radioSuccess);
    }
  }



  // ----------------------------------------------------------------------
  // User defined methods
  // ----------------------------------------------------------------------

  void UHFTransceiverManager::configureSettings() {
    sendCommand("---Reading source callsign---", this->READ_SOURCE_CALLSIGN, false);
    sendCommand("---Reading destination callsign---", this->READ_DESTINATION_CALLSIGN, false);
    sendCommand("---Reading status control word---", this->READ_SCW, false);
    sendCommand("---Turning on pipe mode---", this->WRITE_SCW_PIPE_ON, false);

    // ---------------------------------------------------------------------------------------
    // Testing how we would send data over the transceiver
    // ---------------------------------------------------------------------------------------
    char data[91] = "Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World!"; // data to send
    sendDataBuffer(data); // Send the buffer out over the framer interface
    // ---------------------------------------------------------------------------------------
    // END OF TESTING
    // ---------------------------------------------------------------------------------------
  }

  // This is just a reusable method to send a command, it logs the command being sent
  void UHFTransceiverManager::sendCommand(const char* message, const char* command, bool useUart) {
    // log an event
    Fw::String str(message);
    this->log_ACTIVITY_HI_debuggingEvent(str);

    // send the command

    if (useUart) {
      sendUartCommand(command, strlen(command)+1);
    } else {
      Fw::Buffer readBuffer = sendI2cCommand(command, strlen(command)+1, 64);
      Response r = parseBuffer(readBuffer);
      this->log_ACTIVITY_HI_debuggingEvent(r.fullResponse);
      if (readBuffer.isValid()) {
        this->deallocate_out(0, readBuffer);
      }
    }

  }

  void UHFTransceiverManager::sendDataBuffer(const char* data) {
    // Log event
    Fw::String str("---Sending Data Buffer---");
    this->log_ACTIVITY_HI_debuggingEvent(str);

    // Initalize writeBuffer and verify it can be sent, if not deallocate it
    Fw::Buffer writeBuffer = this->allocate_out(0, strlen(data)); // Allocate buffer of size: length of data
    if (writeBuffer.getSize() < strlen(data)) {
      if (writeBuffer.isValid()) {
        this->deallocate_out(0, writeBuffer);
      }
      this->log_WARNING_LO_MemoryAllocationFailed();
    }

    // Prepare buffer to receive serialized data
    Fw::SerializeBufferBase& sb = writeBuffer.getSerializeRepr();
    sb.resetSer();

    // Serialize data into buffer
    U8 dataBufferTemp[strlen(data)];
    for (int i = 0; i < (strlen(data)); i++) {
      dataBufferTemp[i] = static_cast<U8>(data[i]);
    }
    sb.serialize(dataBufferTemp, strlen(data), true);

    Fw::String str1("");
    str1.format("Data being sent: %s", dataBufferTemp);
    this->log_ACTIVITY_HI_debuggingEvent(str1);
    this->sendBuffer_out(0, writeBuffer);
    
    
    this->log_ACTIVITY_HI_transmitDataSuccess();
  }
  
  void UHFTransceiverManager::transmitData(Fw::Buffer sendBuffer) {
    if (!sendBuffer.isValid()) {
      Fw::String str("sendBuffer was invalid");
      this->log_ACTIVITY_HI_debuggingEvent(str);
    } 
    else {
      Drv::SendStatus status = this->uartSend_out(0, sendBuffer);
      // this->deallocate_out(0, sendBuffer);
      if (status.e != Drv::SendStatus::SEND_OK) {
        this->m_reinitialize = true; // Set the reinitialize flag to true

        if (this->isConnected_comStatus_OutputPort(0)) {
          Fw::Success radioFailure = Fw::Success::FAILURE; // Indicate failure
          this->comStatus_out(0, radioFailure); // Notify the connected port about the failure
        }
        this->log_WARNING_HI_UHFUartNotReady(); // Log the warning
      }
    }

  }

  void UHFTransceiverManager::deallocate_buffer(Fw::Buffer& buffer) {
    // !!! Debugging output to trace the deallocation process
    printf("Buffer size: %d\n", buffer.getSize());
    printf("Buffer data: \n");
    for (U32 i = 0; i < buffer.getSize(); i++) {
      printf("%02X ", static_cast<U8*>(buffer.getData())[i]);
    }
    printf("\n");
    // !!! End of Debugging output

    // Deallocate the buffer if it is valid
    if (buffer.isValid()) {
      this->deallocate_out(0, buffer);
    }
  }

  UHFTransceiverManager::Response UHFTransceiverManager::parseBuffer(Fw::Buffer buffer){
    char* data = reinterpret_cast<char*>(buffer.getData());
    U8 size = buffer.getSize(); //! Size of readBuffer

    Response r; //! Response struct for storing contents and length of readBuffer
    r.fullResponse = data; // Convert contents of read buffer to char array
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
  
    // Deallocate the writeBuffer, we are done using it.
    if (writeBuffer.isValid()) {
      this->deallocate_out(0, writeBuffer);
    }
    
    // Return the data (read) buffer
    return readBuffer;
  }

  void UHFTransceiverManager::sendUartCommand(const char* command, U32 writeSize) {
    // Check m_reinitialize flag to see if we need to reinitialize the component
    if (this->m_reinitialize) {
      this->log_WARNING_HI_UHFUartNotReady();
      // TODO: Research into how complex adding a priority queue would be.
      return;
    }
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
    Drv::SendStatus status = this->uartSend_out(0, writeBuffer);

    // Check the status of the send operation
    if (status.e != Drv::SendStatus::SEND_OK) {
      this->m_reinitialize = true; // Set the reinitialize flag to true

      if (this->isConnected_comStatus_OutputPort(0)) {
        Fw::Success radioFailure = Fw::Success::FAILURE; // Indicate failure
        this->comStatus_out(0, radioFailure); // Notify the connected port about the failure
      }
      this->log_WARNING_HI_UHFUartNotReady(); // Log the warning
    }
    
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
