#ifndef _PACKET_H_
#define _PACKET_H_

// SYSTEM INCLUDES
#include <memory>
#include <netinet/in.h>
#include <stdint.h>

namespace DnsFwd
{
    class Packet
    {
      public:
        // LIFECYCLE
        Packet() = default;
        Packet(uint8_t* a_Buffer, uint32_t a_Size,
               const struct sockaddr_storage& a_Saddr);
        virtual ~Packet();

        Packet(const Packet&) = default;
        Packet& operator=(const Packet&) = default;

        Packet(Packet&& a_Pkt);
        void operator=(Packet&& a_Pkt);

        inline uint8_t* Get()
        {
            return m_Buffer.get();
        }

        inline const uint8_t* Get() const
        {
            return m_Buffer.get();
        }

        inline uint32_t Size() const
        {
            return m_Size;
        }

        inline bool IsEmpty() const
        {
            return (0 == m_Size);
        }

        inline struct sockaddr_storage GetSaddr() const
        {
            return m_Saddr;
        }

        inline sa_family_t GetFamily() const
        {
            return m_Saddr.ss_family;
        }

      protected:
        uint32_t m_Size = 0;
        // shared_ptr is used so that the packet can be passed around easily
        std::shared_ptr<uint8_t[]> m_Buffer;
        // Packet can be v4 or v6. So, use sockaddr_storage
        struct sockaddr_storage m_Saddr;
    };
}  // namespace DnsFwd
#endif  // _UDP_PACKET_H_

