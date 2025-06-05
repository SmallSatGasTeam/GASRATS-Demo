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
    // Configure Radio Frequency
    Fw::Buffer readBuffer = getReadBuffer(this->WRITE_RADIO_FREQ);
    U8* byte_write = getWriteData(readBuffer);
    this->tlmWrite_readConfiguration1(byte_write[0]);

    // Read Radio Frequency Configuration
    Fw::Buffer readBuffer2 = getReadBuffer(this->READ_RADIO_FREQ);
    U8* byte_read1= getWriteData(readBuffer2);
    this->tlmWrite_readConfiguration2(byte_read1[0]);

    // Read Internal Temp
    Fw::Buffer readBuffer3 = getReadBuffer(this->READ_INTERNAL_TEMP);
    U8* byte_read2= getWriteData(readBuffer3);
    this->tlmWrite_temperature((byte_read2[4]*100 + byte_read2[5]*10 + byte_read2[6]));
     
    // Read power mode should be in normal
    Fw::Buffer readBuffer4 = getReadBuffer(this->READ_POWER_MODE);
    U8* byte_read3= getWriteData(readBuffer4);
    this->tlmWrite_powerMode(byte_read3[4]);
    
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }



  U8* UHFTransceiverManager::getWriteData(Fw::Buffer readBuffer){
    //! Get data out of the buffer
    U8* data = reinterpret_cast<U8*>(readBuffer.getData());
    int size = readBuffer.getSize();

    //! Deallocate read buffer
    this->deallocate_out(0, readBuffer);

    // CAUTION: This set of data may contain different sizes for different commands
    return data;
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
    this->checkI2cStatus(this->i2cReadWrite_out(0, reg, writeBuffer, readBuffer));

    // Deallocate the memory in the writeBuffer, we are done using it.
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
