#ifndef _SERVER_APP_H_
#define _SERVER_APP_H_

// LOCAL INCLUDES
#include "DnsPacket.h"
#include "Udp.h"

// SYSTEM INCLUDES
#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <queue>
#include <stdint.h>

namespace DnsFwd
{
    class ServerApp
    {
      public:
        // Lifecycle
        ServerApp() = delete;
        ServerApp(const std::string& a_Ip4, const std::string& a_Ip6,
                  uint16_t a_Port);
        ~ServerApp();
        ServerApp(const ServerApp&) = delete;
        ServerApp& operator=(const ServerApp&) = delete;

        // Statics
        static void Terminate(int a_Signal);

        // Members
        void Run(uint32_t a_Ip, uint16_t a_Port);

        void InsertToQ(DnsPacket&& a_Pkt);

      private:
        // Methods
        void Worker();

        // Attributes
        static std::atomic<bool> m_Terminate;

        uint16_t m_UpstreamPort = 0;
        uint32_t m_UpstreamIp = 0;

        DnsFwd::Udp::Server m_Server;
        // deque is selected so that older entries may be deleted easily.
        // queue doesnt support find().
        std::deque<uint16_t> m_TxQ;
        std::queue<Packet> m_PktQ;

        std::mutex m_QMut;
        std::condition_variable m_QCond;
    };
}  // namespace DnsFwd

#endif  // _SERVER_APP_H_

