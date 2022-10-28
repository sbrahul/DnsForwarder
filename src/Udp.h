#ifndef _UDP_H_
#define _UDP_H_

// SYSTEM INCLUDES
#include <stdint.h>
#include <netinet/in.h>

namespace DnsFwd::Udp
{
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
            m_Timeout = a_Sec;
        }

        uint32_t Recv(uint8_t* a_Buffer, uint32_t a_Size, struct sockaddr_in6* a_Saddr);

      private:
        int m_ServFd = -1;
        uint32_t m_Timeout = 0;

        uint16_t m_ListenPort = 0;
        const char* m_ListenIp = nullptr;
    };
}


#endif // _UDP_H_
