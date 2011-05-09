#ifndef TRACKER_H
#define TRACKER_H

#include <QString>

namespace
{
  // global app guid
  static const QString APP_MAGIC("ebe2a955-14fa-4acf-899e-9f9d51b38476");
  // 57 sec
  static const int UPDATE_INTERVAL = (1000 * 57);
}


struct Settings
{
  bool initialized;
  QString channel;
  QString key;
  QString user;
  QString passw;
  QString auth_token;

  Settings() : initialized(false){}
};
// TRACKER_H
#endif
