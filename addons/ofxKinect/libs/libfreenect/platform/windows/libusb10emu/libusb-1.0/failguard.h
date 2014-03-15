#ifndef LIBUSBEMU_FAIL_GUARD_H
#define LIBUSBEMU_FAIL_GUARD_H

namespace libusbemu
{
  namespace failguard
  {
    const bool Check();

    void WaitDecision();

    const bool Abort();
  }
}

#endif//LIBUSBEMU_FAIL_GUARD_H
