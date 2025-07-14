// This is the Framer for the Endurosat UHF Transciever II

#ifndef SVC_ENDUROSAT_PROTOCOL_HPP
#define SVC_ENDUROSAT_PROTOCOL_HPP

#include <Svc/FramingProtocol/FramingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include <Components/UHFTransceiverManager/UHFTransceiverManager.hpp>
#include <Svc/Deframer/Deframer.hpp>


namespace Svc {
    extern Deframer DEFRAMER_OBJECT; //!< Global deframer object

    namespace EndurosatFrameHeader {
        // Put our different data that the transciever will need here
        
        //! Token type for the header which is the preamble and sync word
        typedef U64 TokenType;

        enum {
            //! Header size for the preamble in bytes
            SIZE_ENDUROSAT  = 6
        };

        //! The preamble which is 5 bytes long
        const TokenType PREAMBLE = static_cast<TokenType>(0xAAAAAAAAAA);

        //! The sync word which is one byte long
        const U8 SYNC_WORD = 0x7E; // i think this is an okay way to declare this
    }

    // Definitions for the F Prime frame header
    namespace FPrimeFrameHeader {

        //! Token type for F Prime frame header
        typedef U32 TokenType;

        enum {
            //! Header size for F Prime frame header
            SIZE_FP = sizeof(TokenType) * 2
        };

        //! The start word for F Prime framing
        const TokenType START_WORD = static_cast<TokenType>(0xdeadbeef);

    }

    //! \brief Implements the F Prime framing protocol
    class EndurosatFraming: public FramingProtocol {
        public:
            
            //! Constructor
            EndurosatFraming();

            //! \brief Sets up the UHFTransceiverManager to allow the protocol to send data through it
            //! \param manager: UHFTransceiverManager component instance
            void setupComp(Components::UHFTransceiverManager* manager);

            //! \brief This will frame the data into a radio packet
            //! \param data: pointer to a set of bytes to be framed
            //! \param size: size of data pointed to by `data`
            //! \param packet_type: type of data supplied for File downlink packets
            void frame(
                const U8* const data, //! The data
                const U32 size, //! The data size in bytes | THIS SHOULD NOT EXCEDE 16 SINCE THAT'S ALL THE TRANSCIEVER CAN HANDLE
                Fw::ComPacket::ComPacketType packet_type //! The packet type
            ) override;
            
            //! \brief This will frame the data into a radio packet for EndurosatFraming
            //! \param data: pointer to a set of bytes to be framed
            //! \param size: size of data pointed to by `data`
            //! \param packet_type: type of data supplied for File downlink packets
            void EndurosatFrame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type);

            //! \brief This will frame the data into a radio packet for fprime Framing
            //! \param data: pointer to a set of bytes to be framed
            //! \param size: size of data pointed to by `data`
            //! \param packet_type: type of data supplied for File downlink packets
            void FPrimeFrame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type);
            
            private:
                Components::UHFTransceiverManager* m_uhfManager = nullptr; //!< Pointer to the UHFTransceiverManager component instance
                // Deframer DEFRAMER_OBJECT;
                
    };

    //! \brief Implements the F Prime deframing protocol
    class EndurosatDeframing : public DeframingProtocol {
        public:

            //! Constructor
            EndurosatDeframing();

            //! \brief This will deframe the data into a radio packet for EndurosatDeframe
            //! \param data: pointer to a set of bytes to be framed
            //! \param needed: Number of bytes needed, this is updated by the caller
            DeframingProtocol::DeframingStatus EndurosatDeframe(Types::CircularBuffer& ring, U32& needed);

            //! \brief This will deframe the data into a radio packet for FPrimeDeframe
            //! \param data: pointer to a set of bytes to be framed
            //! \param needed: Number of bytes needed, this is updated by the caller
            DeframingProtocol::DeframingStatus FPrimeDeframe(Types::CircularBuffer& ring, U32& needed);

            //! \brief This will deframe packets 
            //! \param buffer: CircularBuffer to deframe
            //! \param needed: Number of bytes needed, this is updated by the caller
            //! \return Status
            DeframingStatus deframe(
                Types::CircularBuffer& buffer, 
                U32& needed //! Number of bytes needed, this is updated by the caller
            ) override;

            bool getUseFPrimeProtocol(); //!< Get whether to use F´ protocol

            //! \brief This'll validate the buffer via the validation protocol specified by the transciever's datasheet. (Endurosat CRC16 Checksum)
            //! \param buffer: CircularBuffer to validate
            //! \param size: The data size in bytes
            //! \return true if the buffer is valid, false otherwise
            bool EndurosatValidate(Types::CircularBuffer& ring, U32 size); //<- Validate the buffer using 

            //! \brief This'll validate the buffer using the validation protocol specified by Fprime. (Fprime CRC32 Checksum)
            //! \param buffer: CircularBuffer to validate
            //! \param size: The data size in bytes
            //! \return true if the buffer is valid, false otherwise
            bool FPrimevalidate(Types::CircularBuffer& ring, U32 size); 

            
        
    };
    
    

}

#endif   // SVC_ENDUROSAT_PROTOCOL_HPP

