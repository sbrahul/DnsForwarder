#ifndef _SIGNAL_HND_H_
#define _SIGNAL_HND_H_

namespace DnsFwd::SignalHnd
{
    bool Register(int a_Sig, void (*a_Fn)(int));
}

#endif  // _SIGNAL_HND_H_

