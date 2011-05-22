#include "DaemonManager.h"

#include "SymbianDaemon.h"
#include "MaemoDaemon.h"

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
