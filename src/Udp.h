#ifndef _UDP_H_
#define _UDP_H_

// LOCAL INCLUDES
#include "Packet.h"

// SYSTEM INCLUDES
#include <stdint.h>
#include <netinet/in.h>

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
        Server(const char* a_Ip, uint16_t a_Port);
        ~Server();

        // Members
        bool CreateAndBind();

        inline void SetTimeout(uint32_t a_Sec)
        {
            m_Timeout.tv_sec = a_Sec;
        }

        Packet Recv();

        void SendTo(const DnsFwd::Packet& a_Pkt, struct sockaddr_in6 a_Saddr);

        static constexpr uint32_t MAX_BUF_SIZE = 1500;

      private:
        int m_ServFd = -1;

        uint16_t m_ListenPort = 0;
        const char* m_ListenIp = nullptr;

        struct timeval m_Timeout = {0};

        // Large buffers shouldnt be on stack.
        std::unique_ptr<uint8_t[]> m_RecvBuffer;
    };
}


#endif // _UDP_H_
