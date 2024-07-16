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
    U8 x_h = 0;
    U8 x_l = 0;
    U8 y_h = 0;
    U8 y_l = 0;
    U8 z_h = 0;
    U8 z_l = 0;
    Fw::SerializeBufferBase& sb = fwBuffer.getSerializeRepr();
    sb.setBuffLen(fwBuffer.getSize());  // Set available data for deserialization to the whole memory region

    sb.deserialize(x_h);
    sb.deserialize(x_l);
    sb.deserialize(y_h);
    sb.deserialize(y_l);
    sb.deserialize(z_h);
    sb.deserialize(z_l);
    // sb.deserialize(data);

    // // Convert data to meaningful values
    int16_t x = (x_l << 8) | x_h;
    int16_t y = (y_l << 8) | y_h;
    int16_t z = (z_l << 8) | z_h;

    this->log_ACTIVITY_HI_dataOut(x, y, z);
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
