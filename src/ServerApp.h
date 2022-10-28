#ifndef _SERVER_APP_H_
#define _SERVER_APP_H_

// LOCAL INCLUDES
#include "Udp.h"

// SYSTEM INCLUDES
#include <stdint.h>

namespace DnsFwd
{
    class ServerApp
    {
      public:
        // Lifecycle
        ServerApp() = delete;
        ServerApp(const char* ip, uint16_t port);
        ~ServerApp();

        // Statics
        static void Terminate(int signal);

        // Members
        void Run(uint32_t ip, uint16_t port);

      private:
        // Methods

        // Attributes
        static bool m_Terminate;

        uint16_t m_UpstreamPort = 0;
        uint32_t m_UpstreamIp = 0;

        DnsFwd::Udp::Server m_Server;
    };
}  // namespace DnsFwd

#endif // _SERVER_APP_H_
