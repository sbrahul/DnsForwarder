// LOCAL INCLUDES
#include "Udp.h"
#include "Utils.h"

// SYSTEM INCLUDES
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

DnsFwd::Udp::Server::Server(const char* a_Ip, uint16_t a_Port)
    : m_ListenPort(a_Port),
      m_ListenIp(a_Ip)
{
}

DnsFwd::Udp::Server::~Server()
{
    if (-1 != m_ServFd)
    {
        close(m_ServFd);
    }
}

bool
DnsFwd::Udp::Server::CreateAndBind()
{
//    uint32_t ip = 0;
//    if (!DnsFwd::Utils::Ipv4ToNetwork(m_ListenIp, &ip))
//    {
//        return false;
//    }

    struct in6_addr ip6 = {0};
    if (!DnsFwd::Utils::Ipv6ToNetwork(m_ListenIp, &ip6))
    {
        return false;
    }

    m_ServFd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (-1 == m_ServFd)
    {
        perror("Failed to create server socket");
        return false;
    }

    // dual stack is supported by default on Linux. Set again just in case its
    // switch off manually
    int off = 0;
    setsockopt(m_ServFd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off));

    struct sockaddr_in6 saddr;
    memset(&saddr, 0, sizeof saddr);

    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = ip6;
    saddr.sin6_port = htons(m_ListenPort);

    if (0 != bind(m_ServFd, (struct sockaddr*) &saddr, sizeof saddr))
    {
        perror("Bind failed for server port");
        return false;
    }

	return true;
}

uint32_t DnsFwd::Udp::Server::Recv(uint8_t* a_Buffer, uint32_t a_Size, struct sockaddr_in6* a_Saddr)
{
    // not performing sanity checks on args
    socklen_t saddr_size = sizeof(decltype(*a_Saddr));
    memset(a_Buffer, 0, a_Size);
    memset(a_Saddr, 0, saddr_size);
    int bytes = recvfrom(m_ServFd, a_Buffer, a_Size, 0,
                         (struct sockaddr*) a_Saddr, &saddr_size);
    std::cout << "Returned from recvfrom\n";
    if (bytes < 1)
    {
        // If interrupted, no need to print error
	    if (EINTR != errno)
        {
            perror("Failed to receive data");
        }
        return 0;
    }

    return bytes;
}

