// LOCAL INCLUDES
#include "DnsPacket.h"

// SYSTEM INCLUDES


DnsFwd::DnsPacket::DnsPacket(Packet&& a_Pkt)
    : Packet(std::move(a_Pkt))
{
}


DnsFwd::DnsPacket::~DnsPacket()
{
}

uint16_t DnsFwd::DnsPacket::GetTxId() const
{
    // First 2 bytes are Tx Id
    return *(uint16_t*) m_Buffer.get();
}

