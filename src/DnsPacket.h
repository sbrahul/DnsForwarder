#ifndef _DNS_PACKET_H_
#define _DNS_PACKET_H_

// LOCAL INCLUDES
#include "Packet.h"

namespace DnsFwd
{
    class DnsPacket : public Packet
    {
      public:
        // LIFECYCLE
        DnsPacket(Packet&& a_Pkt);
        virtual ~DnsPacket();

        uint16_t GetTxId() const;
    };
}  // namespace DnsFwd

#endif  // _UDP_PACKET_H_
