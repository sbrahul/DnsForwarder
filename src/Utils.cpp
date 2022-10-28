// LOCAL INCLUDES
#include "Utils.h"

// SYSTEM INCLUDES
#include <iostream>
#include <arpa/inet.h>

bool DnsFwd::Utils::Ipv4ToNetwork(const char *ip, uint32_t* ipNetwork)
{
    if (nullptr == ip)
    {
        return false;
    }

    // extract IP
    if (inet_pton(AF_INET, ip, ipNetwork) != 1)
    {
        perror("Ip conversion failed");
        return false;
    }

    return true;
}

bool DnsFwd::Utils::Ipv6ToNetwork(const char *ip, struct in6_addr* ipNetwork)
{
    if (nullptr == ip)
    {
        return false;
    }

    // extract IP
    if (inet_pton(AF_INET6, ip, ipNetwork) != 1)
    {
        perror("Ip6 conversion failed");
        return false;
    }

    return true;
}
