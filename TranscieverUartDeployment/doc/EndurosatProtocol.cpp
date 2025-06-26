#include "EndurosatProtocol.hpp"
#include "FpConfig.hpp"
#include "stdlib.h"
#include "stdio.h"
#include <cstring>
#include <vector> // Don't forget to include vector!
#include <cstdint> // For fixed-width integer types
#include <limits>  // For std::numeric_limits

#include "DeframingProtocolInterface.hpp"
#include "FramingProtocolInterface.hpp"

namespace Svc {
    EndurosatFraming::EndurosatFraming(): FramingProtocol() {}

    EndurosatDeframing::EndurosatDeframing(): DeframingProtocol() {}

    void EndurosatFraming::setup(FramingProtocolInterface& interface) {
        FW_ASSERT(m_interface == nullptr);
        m_interface = &interface;
    }

    void EndurosatDeframing::setup(DeframingProtocolInterface& interface) {
        FW_ASSERT(m_interface == nullptr);
         m_interface = &interface;
    };

    // Your CRC calculation looks solid! Let's keep it.

    // way to calculate CRC given all bytes at once
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

    // // way to calculate CRC given one byte at a time
    // U16 calculate_crc16(U16 crc, U8 byte) {
    //     crc ^= static_cast<U16>(byte) << 8;
    //     for (int j = 0; j < 8; ++j) {
    //         if (crc & 0x8000) {
    //             crc = (crc << 1) ^ 0x1021;
    //         } else {
    //             crc <<= 1;
    //         }
    //     }
    //     return crc;
    // }

    void EndurosatFraming::setupComp(Components::UHFTransceiverManager* manager) {
        this->m_uhfManager = manager;
    }
    
    void EndurosatFraming::frame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        FW_ASSERT(data != nullptr);

        // Let's build the AX.25 frame using a std::vector
        std::vector<U8> ax25_frame;

        // Preamble for AX.25
        std::vector<U8> preamble_ax25 = {0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E};
        ax25_frame.insert(ax25_frame.end(), preamble_ax25.begin(), preamble_ax25.end());

        // Starting flag
        ax25_frame.push_back(0x7E);

        // Destination Address
        U8 dest_callsign[6] = {'C' << 1, 'Q' << 1, ' ' << 1, ' ' << 1, ' ' << 1, ' ' << 1}; // Using 'CQ' as a common destination callsign
        U8 dest_ssid = 0xE0;
        ax25_frame.insert(ax25_frame.end(), dest_callsign, dest_callsign + 6);
        ax25_frame.push_back(dest_ssid);

        // Source Address
        U8 src_callsign[6] = {'N' << 1, '7' << 1, 'G' << 1, 'A' << 1, 'S' << 1, ' ' << 1};        
        U8 src_ssid = 0xE1;
        ax25_frame.insert(ax25_frame.end(), src_callsign, src_callsign + 6);
        ax25_frame.push_back(src_ssid);

        // Control Field (UI Frame)
        ax25_frame.push_back(0x03);

        // Protocol Identifier (No Layer 3 Protocol)
        ax25_frame.push_back(0xF0);

        // Information Field (Your Data)
        ax25_frame.insert(ax25_frame.end(), data, data + size);

        // FCS (CRC-16) over the AX.25 frame (excluding preamble and flags)
        U16 crc_ax25 = calculate_crc16(ax25_frame.data() + 9, static_cast<U32>(ax25_frame.size() - 9)); // Start after preamble and first flag
        ax25_frame.push_back(static_cast<U8>(crc_ax25 & 0xFF));             // LSB
        ax25_frame.push_back(static_cast<U8>((crc_ax25 >> 8) & 0xFF));      // MSB

        // Ending flag
        ax25_frame.push_back(0x7E);

        // Postamble
        std::vector<U8> postamble = {0x7E, 0x7E, 0x7E};
        ax25_frame.insert(ax25_frame.end(), postamble.begin(), postamble.end());

        // Overall Radio Packet Construction
        std::vector<U8> radio_packet;

        // Preamble
        std::vector<U8> preamble_for_frame = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
        radio_packet.insert(radio_packet.end(), preamble_for_frame.begin(), preamble_for_frame.end());

        // Sync Word
        radio_packet.push_back(0x7E);

        // Data Field 1 (Length of Data Field 2 = AX.25 Frame)
        radio_packet.push_back(static_cast<U8>(ax25_frame.size())); // Cast to U8

        // Data Field 2 (AX.25 Frame)
        radio_packet.insert(radio_packet.end(), ax25_frame.begin(), ax25_frame.end());

        // CRC16 for Radio Packet (Data Field 1 + Data Field 2)
        // CRC over the length byte and the AX.25 frame
        U16 crc_radio = calculate_crc16(radio_packet.data() + 6, static_cast<U32>(radio_packet.size() - 6));
        radio_packet.push_back(static_cast<U8>(crc_radio & 0xFF));
        radio_packet.push_back(static_cast<U8>((crc_radio >> 8) & 0xFF));
        
        // Now, you'll likely need to pass this 'radio_packet' (which is a std::vector<U8>)
        // to your transceiver's sending function. How you do that depends on your specific hardware interface.
        // You can access the underlying data array using radio_packet.data() and its size using radio_packet.size().

        // For example, if you have a function like 'transceiver_send(const U8* data, U32 length)':
        // transceiver_send(radio_packet.data(), static_cast<U32>(radio_packet.size()));
        
        // That is what this does:
        if (this->m_uhfManager) {
            const char* packet = reinterpret_cast<char*>(radio_packet.data());
            U32 packet_size = radio_packet.size();
            this->m_uhfManager->transmitData(packet, size);
        } else {
            printf("[ERROR] UHFTransceiverManager not set in EndurosatFraming\n");
        }
    }

    bool EndurosatDeframing::validate(Types::CircularBuffer& ring, U32 size) {
        if (ring.get_allocated_size() < size + 2) { // Need at least the data + 2 CRC bytes
            return false;
        }

        Fw::Buffer buffer = m_interface->allocate(size+2); // added
        ring.peek(buffer.getData(), size+2, 6); // Peek at the data field 1 and 2, plus the CRC //added

        // U8* data = nullptr; // old lines of code
        // ring.peek(data, size + 2, 6); // Peek at the data field 1 and 2, plus the CRC

        U8* data = static_cast<U8*>(buffer.getData()); // added 

        printf("[DEBUG]  Ring Buffer: %s \n", data);        

        // Calculate CRC over Data Field 1 and Data Field 2
        U16 expected_crc = calculate_crc16(data, size-2);

        // Extract the CRC from the packet
        U16 given_crcMSB = (static_cast<U16>(data[size]) << 8) | data[size + 1];
        U16 given_crcLSB = data[size] | (static_cast<U16>(data[size + 1]) << 8);

        printf("[DEBUG]  Expected CRC16: %x  Given CRC16 (MSB): %x  Given CRC16 (LSB) %x\n", expected_crc, given_crcMSB, given_crcLSB);

        return given_crcMSB == expected_crc;
        // return true;
    }


    DeframingProtocol::DeframingStatus EndurosatDeframing::deframe(Types::CircularBuffer& ring, U32& needed) {
        FW_ASSERT(m_interface != nullptr);

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

        // check that it's bigger than we expected

        if (total_packet_size > ring.get_capacity()) {
            // size is too large so we don't give a 'needed' since it would overflow
            return DeframingProtocol::DEFRAMING_INVALID_SIZE;
        } else if (ring.get_allocated_size() < needed) { // check if the ring is big enough
            return DeframingProtocol::DEFRAMING_MORE_NEEDED;
        }

        // once we know we have the amount of data we'd expect, we can check if it's valid and use the data

        // check the checksum
        printf("[DEBUG]  Validate: %d \n",this->validate(ring, total_packet_size - 2));
        if (!this->validate(ring, total_packet_size - 2)) { // doing -2 so we don't iterate over the crc16 in the packet
            return DeframingProtocol::DEFRAMING_INVALID_CHECKSUM;
        } 

        Fw::Buffer buffer = m_interface->allocate(total_packet_size);
        // ig sometimes this may return buffers larger than requested so we need to account for that
        FW_ASSERT(buffer.getSize() >= total_packet_size);
        buffer.setSize(total_packet_size);
        Fw::SerializeStatus status = ring.peek(buffer.getData(), total_packet_size, EndFrameHeader::SIZE);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        m_interface->route(buffer); // really not sure what this is doing
        return DeframingProtocol::DEFRAMING_STATUS_SUCCESS;
    }
} 
