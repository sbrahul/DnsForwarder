#ifndef _UDP_H_
#define _UDP_H_

// LOCAL INCLUDES
#include "Packet.h"

// SYSTEM INCLUDES
#include <functional>
#include <netinet/in.h>
#include <stdint.h>
#include <string>

namespace DnsFwd::Udp
{
    DnsFwd::Packet SendAndReceive(const DnsFwd::Packet& a_Pkt, uint32_t a_Ip,
                                  uint16_t a_Port);

    // Udp Server to send and receive packets
    class Server
    {
        // disable copy and move
        Server() = delete;
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
        Server(const Server&&) = delete;
        Server& operator=(const Server&&) = delete;

      public:
        // Lifecycle
        Server(const std::string& a_Ip4, const std::string& a_Ip6,
               uint16_t a_Port);
        ~Server();

        // Members
        bool CreateAndBind();

        inline void SetTimeout(uint32_t a_Sec)
        {
            m_Timeout.tv_sec = a_Sec;
        }

        void Recv(std::function<void(DnsFwd::Packet)> a_Inserter);

        void SendTo(const DnsFwd::Packet& a_Pkt,
                    struct sockaddr_storage a_Saddr);

        inline void Terminate()
        {
            m_Terminate = true;
        }

        static constexpr uint32_t MAX_BUF_SIZE = 1500;

      private:
        bool m_Terminate = false;

        int m_ServFd4 = -1;
        int m_ServFd6 = -1;

        uint16_t m_ListenPort = 0;
        std::string m_ListenIp4;
        std::string m_ListenIp6;

        struct timeval m_Timeout = {0};

        // Large buffers shouldnt be on stack.
        std::unique_ptr<uint8_t[]> m_RecvBuffer;
    };
}  // namespace DnsFwd::Udp

#endif  // _UDP_H_

