// LOCAL INCLUDES
#include "ArgsParse.h"

// SYSTEM INCLUDES
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>

namespace DnsFwd::ArgsParse
{
    std::tuple<uint32_t, uint16_t> Parse(int argc, char** argv)
    {
        auto bad_ret = std::make_tuple(0, 0);
        // We need exactly 2 args
        if (argc != 3)
        {
            std::cerr << "Wrong number of args!\n";
            return bad_ret;
        }

        uint32_t ip = 0;
        // extract IP
        if (inet_pton(AF_INET, argv[1], &ip) != 1)
        {
            perror("Unable to parse IPv4 address");
            return bad_ret;
        }

        // extract the port number and check for valid values
        unsigned long port = strtoul(argv[2], NULL, 10);
        std::cout << "Port: " << port << "\n";
        if (!port || (port > 65535))
        {
            std::cerr << "Invalid port number\n";
            return bad_ret;
        }
        uint16_t port_16 = port;

        return std::make_tuple(ip, port_16);
    }
}  // namespace DnsFwd::ArgsParse

