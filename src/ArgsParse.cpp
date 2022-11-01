// LOCAL INCLUDES
#include "ArgsParse.h"
#include "Utils.h"

// SYSTEM INCLUDES
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>

namespace DnsFwd::ArgsParse
{
    std::tuple<uint32_t, uint16_t> Parse(int a_Argc, char** a_Argv)
    {
        auto bad_ret = std::make_tuple(0, 0);
        // We need exactly 2 args
        if (a_Argc != 3)
        {
            std::cout << "Wrong number of args!\n";
            return bad_ret;
        }

        uint32_t ip = 0;
        // extract IP
        if (!DnsFwd::Utils::Ipv4ToNetwork(a_Argv[1], &ip))
        {
            PRINTER("Bad IP value\n");
            return bad_ret;
        }

        // extract the port number and check for valid values
        unsigned long port = strtoul(a_Argv[2], NULL, 10);
        if (!port || (port > 65535))
        {
            PRINTER("Invalid port number\n");
            return bad_ret;
        }
        uint16_t port_16 = port;

        return std::make_tuple(ip, port_16);
    }
}  // namespace DnsFwd::ArgsParse

