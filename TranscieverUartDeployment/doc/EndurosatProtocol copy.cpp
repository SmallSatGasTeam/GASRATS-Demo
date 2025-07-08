#include "EndurosatProtocol.hpp"
#include "FpConfig.hpp"
#include "stdlib.h"
#include "stdio.h"

#include "Utils/Hash/Hash.hpp"

// #include "DeframingProtocolInterface.hpp"
#include "FramingProtocolInterface.hpp"


namespace Svc {

    // -------------------------------------------------------------------------
    // Construction / Setup
    // -------------------------------------------------------------------------

    Deframer DEFRAMER_OBJECT("Deframer");

    void setUseFPrimeProtocol(bool useFPrimeProtocol) { 
        DEFRAMER_OBJECT.setUseFPrimeProtocol(useFPrimeProtocol);
    }; //!< Set whether to use F´ protocol

    bool getUseFprimeProtocol() { 
        return DEFRAMER_OBJECT.getUseFPrimeProtocol(); 
    }; //!< Get whether to use F´ protocol
        
    EndurosatFraming::EndurosatFraming(): FramingProtocol() {}

    EndurosatDeframing::EndurosatDeframing(): DeframingProtocol() {}
    
    void EndurosatFraming::setupComp(Components::UHFTransceiverManager* manager) {
        this->m_uhfManager = manager;
    }

    // -------------------------------------------------------------------------
    // Helper functions
    // -------------------------------------------------------------------------

    // computes a 16-bit cyclic redundancy check (CRC-16) for a given data buffer. It takes a pointer to the data (data) and its size (size) as input, iteratively applying the CRC-16-CCITT algorithm to return the computed checksum.
    U16 calculate_crc16(const U8* data, U32 size) {
        U16 crc = 0xFFFF;
        for (U32 i = 0; i < size; ++i) {
            crc ^= data[i] << 8;
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x8000) {
                    crc = (crc << 1) ^ 0x1021;
                } else {
                    crc <<= 1;
                }
            }
        }
        return crc;
    };

    bool EndurosatDeframing::validate(Types::CircularBuffer& ring, U32 size) {
        // !!! Debugging output to trace the validation process
        // printf("[DEBUG]  Validating ring buffer of size %d\n", size);
        // !!! End of Debugging output
        if (ring.get_allocated_size() < size) { 
            return false;
        }
        
        U8 data[size]; 
        ring.peek(data, size, 6); // Gets all data after the header

        // printf("->  [DEBUG]  Ring Buffer: ");
        // for (U32 i = 0; i < size-2; i++) {
        //     printf(" %02X", data[i]);
        // }
        // printf("\n->  [DEBUG]  CRC16: ");
        // for (U32 i = size-2; i < size; i++) {
        //     printf(" %02X", data[i]);
        // }
        // printf("\n");        
        // Calculate CRC over Data Field 1 and Data Field 2
        U16 expected_crc = calculate_crc16(data, size-2);

        U8 crc_low = data[size];
        U8 crc_high = data[size + 1];
        U16 given_crc = static_cast<U16>(crc_high) << 8 | crc_low;
        // printf("->  [DEBUG]  Expected CRC16: %x  Given CRC16: %x\n", expected_crc, given_crc);
        return given_crc == expected_crc;
    }

    // Might need to have a main method that calls either EndurosatFraming::frame or Fprime::frame, 
    // because there are communication packets that fprime creates that might only work with fprime framing. 

    // -------------------------------------------------------------------------
    // Framing and Deframing Protocol Implementation
    // -------------------------------------------------------------------------

    void EndurosatFraming::frame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        // // Based on what the packet type is, we can call the appropriate framing function
        printf("[DEBUG]  Framing with packet type %d\n", packet_type);
        if (packet_type == Fw::ComPacket::FW_PACKET_UNKNOWN || packet_type == Fw::ComPacket::FW_PACKET_COMMAND || 
            packet_type == Fw::ComPacket::FW_PACKET_TELEM || packet_type == Fw::ComPacket::FW_PACKET_LOG ||
            packet_type == Fw::ComPacket::FW_PACKET_IDLE) {
            // For these packet types, we use the F
            printf("[DEBUG]  Framing with FPrimeFrame\n");
            setUseFPrimeProtocol(true); // Set the flag to use FPrime framing
            FW_ASSERT(getUseFprimeProtocol(), true);
            FPrimeFrame(data, size, packet_type);
        } else {
            printf("[DEBUG]  Framing with EndurosatFrame\n");
            setUseFPrimeProtocol(false); // Set the flag to use FPrime framing
            FW_ASSERT(getUseFprimeProtocol(), false);
            EndurosatFrame(data, size, packet_type);
        }

    }

    void EndurosatFraming::EndurosatFrame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        // !!! Debugging output to trace the framing process
        printf("\n");
        printf("Frame Report: \n");
        printf("->  [DEBUG] Framing data of size %d with packet type %d\n", size, packet_type);
        // printf("->  [DEBUG] Data: ");
        // for (U32 i = 0; i < size; ++i) {
        //     printf(" %02X", data[i]);
        // }
        // printf("\n");
        // !!! End of Debugging output

        // Check if the data pointer and interface are valid
        FW_ASSERT(data != nullptr);
        FW_ASSERT(m_interface != nullptr);

        // Estimate the total size of the radio packet
        const U32 preamble_ax25_len = 8;
        const U32 start_flag_len = 1;
        const U32 addr_len = 14; // 7 dest + 7 source (callsign(6) + ssid(1))
        const U32 ctrl_pid_len = 2;
        const U32 FCS_ax25_len = 2;
        const U32 end_flag_len = 1;
        const U32 postamble_len = 3;

        const U32 ax25_len = preamble_ax25_len + start_flag_len + addr_len + ctrl_pid_len + size + FCS_ax25_len + end_flag_len + postamble_len;

        const U32 radio_preamble_len = 5;
        const U32 sync_word_len = 1;
        const U32 length_byte_len = 1;
        const U32 crc16_len = 2;

        const U32 total_len = radio_preamble_len + sync_word_len + length_byte_len + ax25_len + crc16_len;
        
        // !!! Debugging output to trace the framing process
        printf("->  [DEBUG] Total packet length: %d bytes\n", total_len);
        printf("->  [DEBUG] AX.25 frame length: %d bytes\n", ax25_len);
        // !!! End of Debugging output

        // Allocate buffer for the radio packet
        Fw::Buffer buffer = m_interface->allocate(total_len); 
        if (buffer.getSize() < total_len) {
            printf("[ERROR] Buffer size %d does not match total packet length %d\n", buffer.getSize(), total_len);
            m_uhfManager->deallocate_buffer(buffer); // Deallocate the buffer if it was allocated
            return; // Handle error appropriately
        }

        Fw::SerializeBufferBase& serializer = buffer.getSerializeRepr(); //< Get the serializer representation from the buffer
        Fw::SerializeStatus status; //< Status for serialization operations, used to check serialization success

        // --- Radio preamble ---
        for (int i = 0; i < 5; ++i) {
            status = serializer.serialize(static_cast<U8>(0xAA));
            FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        }

        // --- Sync word ---
        status = serializer.serialize(static_cast<U8>(0x7E));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // --- Length byte (AX.25 frame length) ---
        status = serializer.serialize(static_cast<U8>(ax25_len));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // === Begin AX.25 Frame ===

        // AX.25 preamble
        for (int i = 0; i < 8; ++i) {
            status = serializer.serialize(static_cast<U8>(0x7E));
            FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        }

        // AX.25 start flag
        status = serializer.serialize(static_cast<U8>(0x7E));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Destination callsign ("CQ    ") and SSID
        const U8 dest_callsign[6] = {'C' << 1, 'Q' << 1, ' ' << 1, ' ' << 1, ' ' << 1, ' ' << 1};
        const U8 dest_ssid = 0xE0;

        status = serializer.serialize(dest_callsign, 6, true);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        status = serializer.serialize(dest_ssid);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Source callsign ("N7GAS ") and SSID
        const U8 src_callsign[6] = {'N' << 1, '7' << 1, 'G' << 1, 'A' << 1, 'S' << 1, ' ' << 1};
        const U8 src_ssid = 0xE1;

        status = serializer.serialize(src_callsign, 6, true);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        status = serializer.serialize(src_ssid);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Control + PID
        status = serializer.serialize(static_cast<U8>(0x03));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        status = serializer.serialize(static_cast<U8>(0xF0));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Info field (your payload)
        status = serializer.serialize(data, size, true); // raw data
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Compute CRC over AX.25 data (after preamble and start flag)
        const U8* ax25_start = buffer.getData() + radio_preamble_len + sync_word_len + length_byte_len + preamble_ax25_len + 1; // <- Points to the start of the AX.25 frame data
        const U32 ax25_crc_len = addr_len + ctrl_pid_len + size; // <- Tells us how many bytes to include in the CRC calculation 


        U16 crc_ax25 = calculate_crc16(ax25_start, ax25_crc_len); // <- CRC16 calculation over the AX.25 frame data
        // Serialize the CRC into the AX.25 frame
        status = serializer.serialize(static_cast<U8>(crc_ax25 & 0xFF)); 
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        status = serializer.serialize(static_cast<U8>((crc_ax25 >> 8) & 0xFF));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // End flag
        status = serializer.serialize(static_cast<U8>(0x7E));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Postamble
        for (int i = 0; i < 3; ++i) {
            status = serializer.serialize(static_cast<U8>(0x7E));
            FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        }

        // Compute radio packet CRC (starts at length byte, spans AX.25 frame)
        const U8* radio_crc_start = buffer.getData() + radio_preamble_len + sync_word_len; //<- Points to the start of the radio packet data, which includes the length byte and the AX.25 frame
        const U32 radio_crc_len = length_byte_len + ax25_len; //<- Tells us how many bytes to include in the CRC calculation for the radio packet

        U16 crc_radio = calculate_crc16(radio_crc_start, radio_crc_len); //<- CRC16 calculation over the radio packet data
        status = serializer.serialize(static_cast<U8>(crc_radio & 0xFF));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        status = serializer.serialize(static_cast<U8>((crc_radio >> 8) & 0xFF));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Set actual size (tells the buffer how much data is actually in it)
        buffer.setSize(total_len);

        // Send the buffer to the interface
        m_interface->send(buffer);
        // !!! Debugging output to trace the framing process
        printf("->  [DEBUG] Framing complete. Total size: %d bytes\n", total_len);
    }

    void EndurosatFraming::FPrimeFrame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        printf("[DEBUG]   Packet type: %d\n", packet_type);
        FW_ASSERT(data != nullptr);
        FW_ASSERT(m_interface != nullptr);
        // Use of I32 size is explicit as ComPacketType will be specifically serialized as an I32
        FPrimeFrameHeader::TokenType real_data_size =
            size + ((packet_type != Fw::ComPacket::FW_PACKET_UNKNOWN) ?
            static_cast<Svc::FPrimeFrameHeader::TokenType>(sizeof(I32)) :
            0);
        FPrimeFrameHeader::TokenType total = real_data_size + FPrimeFrameHeader::SIZE_FP + HASH_DIGEST_LENGTH;
        Fw::Buffer buffer = m_interface->allocate(total);
        Fw::SerializeBufferBase& serializer = buffer.getSerializeRepr();
        Utils::HashBuffer hash;

        // Serialize data
        Fw::SerializeStatus status;
        status = serializer.serialize(FPrimeFrameHeader::START_WORD);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        status = serializer.serialize(real_data_size);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Serialize packet type if supplied, otherwise it *must* be present in the data
        if (packet_type != Fw::ComPacket::FW_PACKET_UNKNOWN) {
            status = serializer.serialize(static_cast<I32>(packet_type)); // I32 used for enum storage
            FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        }

        status = serializer.serialize(data, size, true);  // Serialize without length
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        // Calculate and add transmission hash
        Utils::Hash::hash(buffer.getData(), static_cast<NATIVE_INT_TYPE>(total - HASH_DIGEST_LENGTH), hash);
        status = serializer.serialize(hash.getBuffAddr(), HASH_DIGEST_LENGTH, true);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

        buffer.setSize(total);

        m_interface->send(buffer);
    }

    DeframingProtocol::DeframingStatus EndurosatDeframing::deframe(Types::CircularBuffer& ring, U32& needed) {
        if (DEFRAMER_OBJECT.getUseFPrimeProtocol()) {
            // If we are using the FPrime protocol, call the FPrime deframe method
            return this->FPrimeDeframe(ring, needed);
        } else {
            // Otherwise, use the Endurosat deframe method
            return this->EndurosatDeframe(ring, needed);
        }
    }

    DeframingProtocol::DeframingStatus EndurosatDeframing::EndurosatDeframe(Types::CircularBuffer& ring, U32& needed) {
        // !!! Debugging output to trace the deframing process
        printf("\n");
        printf("Deframe Report: \n");
        // printf("->  [DEBUG] Deframing from ring buffer of size %d\n", ring.get_allocated_size());
        // !!! End of Debugging output
        FW_ASSERT(this->m_interface != nullptr);

        // just go through the ring and read all the data 
        // need to keep checking the size of the ring to see if it's the size we're expecting as defined by data field 1

        // first we need to check if it even has data field 1, which holds the size of data field 2 which is the only part of the packet that varies in size
        if (ring.get_allocated_size() < 6) {
            needed = 6;
            return DeframingProtocol::DEFRAMING_MORE_NEEDED;
        }
        
        // then once we know the size after we get data field 1, we need to wait until the size = 7 + size of data field 2 + 2
        U8 size_field_1; // size of data field 1 defined by data field 1
        ring.peek(size_field_1, 6);

        const U8 maxU8 = std::numeric_limits<U8>::max();
        if (size_field_1 > maxU8) {
            // too big, 'needed' would overflow
            return DeframingProtocol::DEFRAMING_INVALID_SIZE;
        }

        // check the total fram size, both 'needed' and 'total_packet_size' are the same thing
        needed = size_field_1 + 9;
        U8 total_packet_size = size_field_1 + 9; // add the size of the other fixed sized sections

        
        // !!! Debugging output to trace the peeked data
        // printf("->  [DEBUG]  Size Field 1: %d, Total Packet Size: %d\n", size_field_1, total_packet_size);
        // U8 debugPeek[total_packet_size];
        // ring.peek(debugPeek, total_packet_size, 0);
        // printf("->  [DEBUG]  Peeked data from ring buffer (%d bytes):", total_packet_size+2);
        // for (U32 i = 0; i < sizeof(debugPeek) + 2; i++) {
        //     printf(" %02X", debugPeek[i]);
        // }
        // printf("\n");
        // !!! End of Debugging output

        // check that it's bigger than we expected
        if (total_packet_size > ring.get_capacity()) {
            // size is too large so we don't give a 'needed' since it would overflow
            return DeframingProtocol::DEFRAMING_INVALID_SIZE;
        } else if (ring.get_allocated_size() < needed) { // check if the ring is big enough
            return DeframingProtocol::DEFRAMING_MORE_NEEDED;
        }

        // once we know we have the amount of data we'd expect, we can check if it's valid and use the data
        // check the checksum

        if (!this->validate(ring, total_packet_size )) { 
            return DeframingProtocol::DEFRAMING_INVALID_CHECKSUM;
        } 

        Fw::Buffer buffer = m_interface->allocate(total_packet_size);
        // ig sometimes this may return buffers larger than requested so we need to account for that
        FW_ASSERT(buffer.getSize() >= total_packet_size);
        buffer.setSize(total_packet_size);
        Fw::SerializeStatus status = ring.peek(buffer.getData(), total_packet_size, EndurosatFrameHeader::SIZE_ENDUROSAT);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        m_interface->route(buffer); // really not sure what this is doing

        return DeframingProtocol::DEFRAMING_STATUS_SUCCESS;
    }

    DeframingProtocol::DeframingStatus EndurosatDeframing::FPrimeDeframe(Types::CircularBuffer& ring, U32& needed) {
        FPrimeFrameHeader::TokenType start = 0;
        FPrimeFrameHeader::TokenType size = 0;
        FW_ASSERT(m_interface != nullptr);
        // Check for header or ask for more data
        if (ring.get_allocated_size() < FPrimeFrameHeader::SIZE_FP) {
            needed = FPrimeFrameHeader::SIZE_FP;
            return DeframingProtocol::DEFRAMING_MORE_NEEDED;
        }
        // Read start value from header
        Fw::SerializeStatus status = ring.peek(start, 0);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        if (start != FPrimeFrameHeader::START_WORD) {
            // Start word must be valid
            return DeframingProtocol::DEFRAMING_INVALID_FORMAT;
        }
        // Read size from header
        status = ring.peek(size, sizeof(FPrimeFrameHeader::TokenType));
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        const U32 maxU32 = std::numeric_limits<U32>::max();
        if (size > maxU32 - (FPrimeFrameHeader::SIZE_FP + HASH_DIGEST_LENGTH)) {
            // Size is too large to process: needed would overflow
            return DeframingProtocol::DEFRAMING_INVALID_SIZE;
        }
        needed = (FPrimeFrameHeader::SIZE_FP + size + HASH_DIGEST_LENGTH);
        // Check frame size
        const U32 frameSize = size + FPrimeFrameHeader::SIZE_FP + HASH_DIGEST_LENGTH;
        if (frameSize > ring.get_capacity()) {
            // Frame size is too large for ring buffer
            return DeframingProtocol::DEFRAMING_INVALID_SIZE;
        }
        // Check for enough data to deserialize everything;
        // otherwise break and wait for more.
        else if (ring.get_allocated_size() < needed) {
            return DeframingProtocol::DEFRAMING_MORE_NEEDED;
        }
        // Check the checksum
        if (not this->validate(ring, needed - HASH_DIGEST_LENGTH)) {
            return DeframingProtocol::DEFRAMING_INVALID_CHECKSUM;
        }
        Fw::Buffer buffer = m_interface->allocate(size);
        // Some allocators may return buffers larger than requested.
        // That causes issues in routing; adjust size.
        FW_ASSERT(buffer.getSize() >= size);
        buffer.setSize(size);
        status = ring.peek(buffer.getData(), size, FPrimeFrameHeader::SIZE_FP);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        m_interface->route(buffer);
        return DeframingProtocol::DEFRAMING_STATUS_SUCCESS;
    }
    

    

} 
