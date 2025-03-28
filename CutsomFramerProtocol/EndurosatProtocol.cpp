// This is the actual implementation that'll abide by how things should be setup for the transciever

#include "EndurosatProtocol.hpp"
#include "FpConfig.hpp"

namespace Svc {
    EndurosatFraming::EndurosatFraming(): FramingProtocol() {}

    EndurosatDeframing::EndurosatDeframing(): DeframingProtocol() {}

    void EndurosatFraming::frame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        FW_ASSERT(data != nullptr);
        
        // Preamble 
        
    }
} // namespace Svc
