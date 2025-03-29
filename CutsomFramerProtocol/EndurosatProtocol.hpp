// This is the Framer for the Endurosat UHF Transciever II

#ifndef SVC_ENDUROSAT_PROTOCOL_HPP
#define SVC_ENDUROSAT_PROTOCOL_HPP

#include <Svc/FramingProtocol/FramingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>

namespace Svc {

    namespace EndFrameHeader {
        // Put our different data that the transciever will need here
        
        //! Token type for the header which is the preamble and sync word
        typedef U64 TokenType;

        enum {
            //! Header size for the preabmble in bytes
            SIZE = 6
        };

        //! The preamble which is 5 bytes long
        const TokenType PREAMBLE = static_cast<TokenType>(0xAAAAAAAAAA);

        //! The sync word which is one byte long
        const U8 SYNC_WORD = 0x7E; // i think this is an okay way to declare this
    }

    //! \brief Implements the F Prime framing protocol
    class EndurosatFraming: public FramingProtocol {
        public:
            
            //! Constructor
            EndurosatFraming();

            //! Implementing the frame method
            void frame(
                const U8* const data, //! The data
                const U32 size, //! The data size in bytes | THIS SHOULD NOT EXCEDE 16 SINCE THAT'S ALL THE TRANSCIEVER CAN HANDLE
                Fw::ComPacket::ComPacketType packet_type //! The packet type
            ) override;
    };

    //! \brief Implements the F Prime deframing protocol
    class EndurosatDeframing : public DeframingProtocol {
        public:

            //! Constructor
            EndurosatDeframing();

            //! This'll validate the buffer via the validation protocol specified by the transciever's datasheet
            bool validate(
                Types:CircularBuffer& buffer, 
                U32 size //! The data size in bytes
            );

            //! Implements the deframe method
            //! \return Status
            DeframingStatus deframe(
                Types::CircularBuffer& buffer, 
                U32& needed //! Number of bytes needed, this is updated by the caller
            ) override;
    };
}
#endif   // SVC_ENDUROSAT_PROTOCOL_HPP