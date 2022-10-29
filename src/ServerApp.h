#ifndef _SERVER_APP_H_
#define _SERVER_APP_H_

// LOCAL INCLUDES
#include "Udp.h"

// SYSTEM INCLUDES
#include <stdint.h>
#include <deque>

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
        // Queue is selected so that older entries may be deleted easily
        std::deque<uint16_t> m_TxQ;
    };
}  // namespace DnsFwd

#endif // _SERVER_APP_H_
