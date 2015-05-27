#pragma once

namespace libusbemu
{
  namespace failguard
  {
    const bool Check();

    void WaitDecision();

    const bool Abort();
  }
}
