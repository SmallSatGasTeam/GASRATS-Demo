// This is the Framer for the Endurosat UHF Transciever II

#ifndef SVC_ENDUROSAT_PROTOCOL_HPP
#define SVC_ENDUROSAT_PROTOCOL_HPP

#include <Svc/FramingProtocol/FramingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include <Components/UHFTransceiverManager/UHFTransceiverManager.hpp>

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

            //! \brief Sets up the UHFTransceiverManager to allow the protocol to send data through it
            //! \param manager: UHFTransceiverManager component instance
            void setupComp(Components::UHFTransceiverManager* manager);

            //! \brief setup function called to supply the interface used for allocation and sending
            //! \param interface: interface implementation, normally FramerComponentImpl
            void setup(FramingProtocolInterface& interface);

            //! Implementing the frame method
            void frame(
                const U8* const data, //! The data
                const U32 size, //! The data size in bytes | THIS SHOULD NOT EXCEDE 16 SINCE THAT'S ALL THE TRANSCIEVER CAN HANDLE
                Fw::ComPacket::ComPacketType packet_type //! The packet type
            ) override;

            private:
                Components::UHFTransceiverManager* m_uhfManager = nullptr; //!< Pointer to the UHFTransceiverManager component instance
                FramingProtocolInterface* m_interface; //!< Pointer to the FramingProtocolInterface implementation
    };

    //! \brief Implements the F Prime deframing protocol
    class EndurosatDeframing : public DeframingProtocol {
        public:

            //! Constructor
            EndurosatDeframing();


            //! This'll validate the buffer via the validation protocol specified by the transciever's datasheet
            bool validate(
                Types::CircularBuffer& buffer, 
                U32 size //! The data size in bytes
            );

            //! Implements the deframe method
            //! \return Status
            DeframingStatus deframe(
                Types::CircularBuffer& buffer, 
                U32& needed //! Number of bytes needed, this is updated by the caller
            ) override;

            //! \brief Setup the deframing protocol with the deframing interface
            //! \param interface: Deframing interface implementation, normally DeframerComponentImpl
            void setup(
                DeframingProtocolInterface& interface /*!< Deframing interface */
            );
        PROTECTED:
            DeframingProtocolInterface* m_interface; //1<!< Pointer to the DeframingProtocolInterface implementation
    };

    

}

#endif   // SVC_ENDUROSAT_PROTOCOL_HPP

