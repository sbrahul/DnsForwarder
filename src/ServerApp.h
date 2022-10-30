#ifndef _SERVER_APP_H_
#define _SERVER_APP_H_

// LOCAL INCLUDES
#include "Udp.h"

// SYSTEM INCLUDES
#include <deque>
#include <stdint.h>

namespace DnsFwd
{
    class ServerApp
    {
      public:
        // Lifecycle
        ServerApp() = delete;
        ServerApp(const char* a_Ip, uint16_t a_Port);
        ~ServerApp();

        // Statics
        static void Terminate(int a_Signal);

        // Members
        void Run(uint32_t a_Ip, uint16_t a_Port);

      private:
        // Methods

        // Attributes
        static bool m_Terminate;

        DnsFwd::Udp::Server m_Server;
        // Queue is selected so that older entries may be deleted easily
        std::deque<uint16_t> m_TxQ;
    };
}  // namespace DnsFwd

#endif  // _SERVER_APP_H_
