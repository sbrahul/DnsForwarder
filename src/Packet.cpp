// LOCAL INCLUDES
#include "Packet.h"

// SYSTEM INCLUDES
#include <cstring>

DnsFwd::Packet::Packet(uint8_t* a_Buffer, uint32_t a_Size,
                       const struct sockaddr_in& a_Saddr)
    : m_Size(a_Size),
      m_Buffer(new uint8_t[a_Size]),
      m_Saddr4(a_Saddr)
{
    // Copy the packet
    memcpy(m_Buffer.get(), a_Buffer, a_Size);
}

DnsFwd::Packet::Packet(uint8_t* a_Buffer, uint32_t a_Size,
                       const struct sockaddr_in6& a_Saddr)
    : m_Size(a_Size),
      m_Buffer(new uint8_t[a_Size]),
      m_Saddr6(a_Saddr)
{
    // Copy the packet
    memcpy(m_Buffer.get(), a_Buffer, a_Size);
}

DnsFwd::Packet::~Packet()
{
}

DnsFwd::Packet::Packet(Packet&& a_Pkt)
{
    std::swap(m_Buffer, a_Pkt.m_Buffer);
    std::swap(m_Size, a_Pkt.m_Size);
    // v6 saddr is bigger. so swapping that should be enough
    std::swap(m_Saddr6, a_Pkt.m_Saddr6);
}
