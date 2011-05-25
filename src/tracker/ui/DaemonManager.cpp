#include "DaemonManager.h"

#ifdef NO_DAEMON
#include "SymbianDaemon.h"
#else
#include "MaemoDaemon.h"
#endif

DaemonManager::DaemonManager()
{

}


Daemon& DaemonManager::getInstance()
{
  #ifdef NO_DAEMON
  static SymbianDaemon daemon;
  #else
  static MaemoDaemon daemon;
  #endif
  return daemon;
}
