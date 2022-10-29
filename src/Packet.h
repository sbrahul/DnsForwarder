#ifndef _PACKET_H_
#define _PACKET_H_

// SYSTEM INCLUDES
#include <stdint.h>
#include <netinet/in.h>
#include <memory>

namespace DnsFwd
{
    class Packet
    {
      public:
        // LIFECYCLE
        Packet() {};
        Packet(uint8_t* a_Buffer, uint32_t a_Size,
               const struct sockaddr_in& a_Saddr);
        Packet(uint8_t* a_Buffer, uint32_t a_Size,
               const struct sockaddr_in6& a_Saddr);
        virtual ~Packet();

        Packet(Packet&& a_Pkt);

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
            // convert to bool
            return (0 == m_Size);
        }

        inline struct sockaddr_in6 GetSaddr6() const
        {
            return m_Saddr6;
        }

      protected:
        uint32_t m_Size = 0;
        std::shared_ptr<uint8_t[]> m_Buffer;
        union
        {
            struct sockaddr_in m_Saddr4;
            struct sockaddr_in6 m_Saddr6;
        };
    };
}
#endif // _UDP_PACKET_H_

