// ======================================================================
// \title  DataCollector.cpp
// \author devin
// \brief  cpp file for DataCollector component implementation class
// ======================================================================

#include "Components/DataCollector/DataCollector.hpp"
#include "FpConfig.hpp"
#include "Components/componentConfig/Constants.hpp"
#include <iostream>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  DataCollector ::
    DataCollector(const char* const compName) :
      DataCollectorComponentBase(compName)
  {
    this->iter = 0;
  }

  DataCollector ::
    ~DataCollector()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void DataCollector ::
    imuIncoming_handler(
        NATIVE_INT_TYPE portNum,
        Fw::Buffer& fwBuffer
    )
  {
    U32 my_data = 0;
    U8  my_byte = 0;
    Fw::SerializeBufferBase& sb = fwBuffer.getSerializeRepr();
    sb.setBuffLen(fwBuffer.getSize());  // Set available data for deserialization to the whole memory region
    sb.deserialize(my_data);
    sb.deserialize(my_byte);

    this->log_ACTIVITY_HI_dataOut(my_byte);
  }

  void DataCollector ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    this->ping_out(0, 123);
    this->ping_out(1, 123);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined serial input ports
  // ----------------------------------------------------------------------

  void DataCollector ::
    aggregate_handler(
        NATIVE_INT_TYPE portNum,
        Fw::SerializeBufferBase& buffer
    )
  {
    if(iter < MAX_BACKGROUND_MESSAGES) {
      this->log_ACTIVITY_HI_dataSuccess();
      this->iter++;
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void DataCollector ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
