// ======================================================================
// \title  CommunicationAdapter.cpp
// \author devins
// \brief  cpp file for CommunicationAdapter component implementation class
// ======================================================================

#include "Components/CommunicationAdapter/CommunicationAdapter.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  CommunicationAdapter ::
    CommunicationAdapter(const char* const compName) :
      CommunicationAdapterComponentBase(compName)
  {

  }

  CommunicationAdapter ::
    ~CommunicationAdapter()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  Drv::SendStatus CommunicationAdapter ::
    comDataIn_handler(
        FwIndexType portNum,
        Fw::Buffer& sendBuffer
    )
  {
    // This is where we directly send the data to the transciever
    // first we must check if we're the first one so we can start the com queue by sending a success to start it, see ssd.md for the ComQueue component
    Fw::Success success = Fw::Success::SUCCESS;
    if (this->isFirst) {
      this->comStatus_out(0, success);
      this->isFirst = false;
    }

    // now we can assume we're not the first, and just start dealing with the data
    // if everything's hooked up right i mean it should just already be framed at this point and we can send it over i2c to the transciever
    // our measure of 'Success' in this component is just if the transciever reccieved the data successfully with no errors
    Drv::I2cStatus status = this->i2cWrite_out(0, this->ADDRESS, sendBuffer);
    // it is this components responsibility to deallocate the buffer https://fprime.jpl.nasa.gov/latest/docs/reference/communication-adapter-interface/#ports
    this->deallocate_out(0, sendBuffer);

    // now return the success message to the framer to give to the communication queue
    success = this->checkStatus(status);
    this->comStatus_out(0, success);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void CommunicationAdapter ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }


  // ----------------------------------------------------------------------
  // Helper Functions
  // ----------------------------------------------------------------------

  Fw::Success CommunicationAdapter::checkStatus(Drv::I2cStatus i2cStatus) {
    Fw::Success success = Fw::Success::SUCCESS;
    Fw::Success failure = Fw::Success::FAILURE;
    switch (i2cStatus) {
      case Drv::I2cStatus::I2C_OK:
        this->log_ACTIVITY_HI_imuSuccess();
        return success; // only time we return success
        break;

      case Drv::I2cStatus::I2C_ADDRESS_ERR:
        this->log_WARNING_HI_imuAddressFailure();
        break;

      case Drv::I2cStatus::I2C_WRITE_ERR:
        this->log_WARNING_HI_imuWriteError();
        break;

      case Drv::I2cStatus::I2C_READ_ERR:
        this->log_WARNING_HI_imuReadError();
        break;

      case Drv::I2cStatus::I2C_OPEN_ERR:
        this->log_WARNING_HI_imuOpenError();
        break;

      case Drv::I2cStatus::I2C_OTHER_ERR:
        this->log_WARNING_HI_imuOtherError();
        break;
      
      default:
        break;  
    }
    return failure; // return a failure if any other thing happened
  }
}
