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
    configureFrequency_cmdHandler(
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
  // ----------------------------------------------------------------------
  // User defined methods
  // ----------------------------------------------------------------------

  void UHFTransceiverManager::configureSettings() {
    // Fw::String str("Reading low power mode");
    // this->log_ACTIVITY_HI_somethingHappened(str);
    // Fw::Buffer readBuffer = getReadBuffer(this->READ_POWER_MODE, strlen(this->READ_POWER_MODE)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r = parseResponse(readBuffer);
    // this->log_ACTIVITY_HI_somethingHappened(r.fullResponse);
    // this->deallocate_out(0, readBuffer);

    // Fw::String str1("Turning on low power mode");
    // this->log_ACTIVITY_HI_somethingHappened(str1);

    // Fw::Buffer readBuffer1 = getReadBuffer(this->WRITE_POWER_MODE, strlen(this->WRITE_POWER_MODE)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r1 = parseResponse(readBuffer1);
    // this->log_ACTIVITY_HI_somethingHappened(r1.fullResponse);
    // this->deallocate_out(0, readBuffer1);

    // Fw::String str2("Reading low power mode");
    // this->log_ACTIVITY_HI_somethingHappened(str2);

    // Fw::Buffer readBuffer2 = getReadBuffer(this->READ_POWER_MODE, strlen(this->READ_POWER_MODE)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r2 = parseResponse(readBuffer2);
    // this->log_ACTIVITY_HI_somethingHappened(r2.fullResponse);
    // this->deallocate_out(0, readBuffer2);

    // Fw::String str3("Reading temp, which turns off low power mode");
    // this->log_ACTIVITY_HI_somethingHappened(str3);

    // Fw::Buffer readBuffer3 = getReadBuffer(this->READ_INTERNAL_TEMP_ASCII, strlen(this->READ_INTERNAL_TEMP_ASCII)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r3 = parseResponse(readBuffer3);
    // this->log_ACTIVITY_HI_somethingHappened(r3.fullResponse);
    // this->deallocate_out(0, readBuffer3);

    // Fw::String str4("Reading low power mode");
    // this->log_ACTIVITY_HI_somethingHappened(str4);

    // Fw::Buffer readBuffer4 = getReadBuffer(this->READ_POWER_MODE, strlen(this->READ_POWER_MODE)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r4 = parseResponse(readBuffer4);
    // this->log_ACTIVITY_HI_somethingHappened(r4.fullResponse);
    // this->deallocate_out(0, readBuffer4);

    // Fw::String str5(".....Now reading uptime........");
    // this->log_ACTIVITY_HI_somethingHappened(str5);
    // Fw::Buffer readBuffer5 = getReadBuffer(this->READ_UPTIME, strlen(this->READ_UPTIME)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r5 = parseResponse(readBuffer5);
    // this->log_ACTIVITY_HI_somethingHappened(r5.fullResponse);
    // this->deallocate_out(0, readBuffer5);

    // Fw::String str6(".....Now reading packets transmitted........");
    // this->log_ACTIVITY_HI_somethingHappened(str6);
    // Fw::Buffer readBuffer6 = getReadBuffer(this->READ_TRANSMITTED_PACKETS, strlen(this->READ_TRANSMITTED_PACKETS)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r6 = parseResponse(readBuffer6);
    // this->log_ACTIVITY_HI_somethingHappened(r6.fullResponse);
    // this->deallocate_out(0, readBuffer6);

    // Fw::String str7(".....Now reading packets received........");
    // this->log_ACTIVITY_HI_somethingHappened(str7);
    // Fw::Buffer readBuffer7 = getReadBuffer(this->READ_RECEIVED_PACKETS, strlen(this->READ_RECEIVED_PACKETS)+1, 64); // 18 -> writeSize, 17 -> readSize
    // Response r7 = parseResponse(readBuffer7);
    // this->log_ACTIVITY_HI_somethingHappened(r7.fullResponse);
    // this->deallocate_out(0, readBuffer7);

    Fw::String str8(".....Now reading status control word........");
    this->log_ACTIVITY_HI_somethingHappened(str8);
    Fw::Buffer readBuffer8 = getReadBuffer(this->READ_SCW, strlen(this->READ_SCW)+1, 64); // 18 -> writeSize, 17 -> readSize
    Response r8 = parseResponse(readBuffer8);
    this->log_ACTIVITY_HI_somethingHappened(r8.fullResponse);
    this->deallocate_out(0, readBuffer8);


    Fw::String str12(".....Now reading status control word........");
    this->log_ACTIVITY_HI_somethingHappened(str12);
    Fw::Buffer readBuffer12 = getReadBuffer(this->READ_SCW, strlen(this->READ_SCW)+1, 64); // 18 -> writeSize, 17 -> readSize
    Response r12 = parseResponse(readBuffer12);
    this->log_ACTIVITY_HI_somethingHappened(r12.fullResponse);
    this->deallocate_out(0, readBuffer12);


    Fw::String str9(".....Now writing pipe on status control word........");
    this->log_ACTIVITY_HI_somethingHappened(str9);
    Fw::Buffer readBuffer9 = getReadBuffer(this->WRITE_SCW_PIPE_ON, strlen(this->WRITE_SCW_PIPE_ON)+1, 64); // 18 -> writeSize, 17 -> readSize
    Response r9 = parseResponse(readBuffer9);
    this->log_ACTIVITY_HI_somethingHappened(r9.fullResponse);
    this->deallocate_out(0, readBuffer9);

    // When you go into pipe mode you can no longer communicate via I2C.

    Fw::String str10(".....Now reading status control word........");
    this->log_ACTIVITY_HI_somethingHappened(str10);
    Fw::Buffer readBuffer10 = getReadBuffer(this->READ_SCW, strlen(this->READ_SCW)+1, 64); // 18 -> writeSize, 17 -> readSize
    Response r10 = parseResponse(readBuffer10);
    this->log_ACTIVITY_HI_somethingHappened(r10.fullResponse);
    this->deallocate_out(0, readBuffer10);


    Fw::String str11(".....Now writing pipe off status control word........");
    this->log_ACTIVITY_HI_somethingHappened(str11);
    Fw::Buffer readBuffer11 = getReadBuffer(this->WRITE_SCW_PIPE_OFF, strlen(this->WRITE_SCW_PIPE_OFF)+1, 64); // 18 -> writeSize, 17 -> readSize
    Response r11 = parseResponse(readBuffer11);
    this->log_ACTIVITY_HI_somethingHappened(r11.fullResponse);
    this->deallocate_out(0, readBuffer11);


    Fw::String str13(".....Now reading status control word........");
    this->log_ACTIVITY_HI_somethingHappened(str13);
    Fw::Buffer readBuffer13 = getReadBuffer(this->READ_SCW, strlen(this->READ_SCW)+1, 64); // 18 -> writeSize, 17 -> readSize
    Response r13 = parseResponse(readBuffer13);
    this->log_ACTIVITY_HI_somethingHappened(r13.fullResponse);
    this->deallocate_out(0, readBuffer13);



    // U8* data = static_cast<U8*>(readBuffer8.getData());
    // data[5] = 1;


    // COMMAND: Read Temperature (Example READ command)
    // Fw::Buffer readBuffer2 = getReadBuffer(this->READ_INTERNAL_TEMP_ASCII, 18, 25); // 18 -> writeSize, 17 -> readSize
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
    for (int i = 0; i < (writeSize-1); i++) {
      dataBufferTemp[i] = static_cast<char>(command[i]);
    }
    dataBufferTemp[writeSize-1] = 0x0D; // Last character in command should always be a carriage return <CR>

    // Fw::String str(reinterpret_cast<const char*>(dataBufferTemp));

    // this->tlmWrite_response(str);

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
