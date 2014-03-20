#include "failguard.h"
#include "libusbemu_threads.h"

int ThreadFailGuardProc(void* params);

namespace libusbemu
{

namespace failguard
{

static volatile bool boTriggered (false);
static QuickEvent hReaction;
static volatile int nDecision (0);

const bool Check()
{
  if (failguard::boTriggered)
    return(true);

  // CTRL + ALT pressed?
  if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_MENU) & 0x8000))
  {
    // only one thread is allowed to activate the guard
    static QuickMutex mutexFailGuard;
    if (mutexFailGuard.TryEnter())
    {
      if (!failguard::boTriggered)
      {
        failguard::hReaction.Reset();
        failguard::boTriggered = true;
        new QuickThread(ThreadFailGuardProc, NULL, true);
      }
      mutexFailGuard.Leave();
    }
  }

  return(failguard::boTriggered);
}

void WaitDecision()
{
  failguard::hReaction.Wait();
}

const bool Abort()
{
  return(-1 == nDecision);
}

}

}

using namespace libusbemu::failguard;

int ThreadFailGuardProc(void* params)
{
  int user_option =
  MessageBoxA(GetDesktopWindow(),
              "The libusb_handle_events() fail guard of libusbemu was reached!\n"
              "This was caused by pressing and holding the [CTRL] + [ALT] keys.\n"
              "If it was unintentional, click Cancel to resume normal execution;\n"
              "otherwise, click OK to effectively terminate the thread (note that\n"
              "the host program might run abnormally after such termination).",
              "WARNING: libusbemu thread fail guard reached!", MB_ICONWARNING | MB_OKCANCEL);

  if (IDOK == user_option)
    nDecision = -1;
  else
    boTriggered = false;

  hReaction.Signal();

  return(0);
}
