#ifndef __ServerConnection__h__
#define __ServerConnection__h__

#include <list>
#include "Channel.h"
#include "Position.h"

class ServerConnection
{

  std::list<Channel> user_channels;// Channels tha user subscrubed

  std::list<Channel> m_channels; // All channels
  
  ServerConnection();

  static ServerConnection* current_con;

  static void initInstance();

public:

  bool isLogged() const;

  /* Login as User.*/
  bool login(const std::string& login,const std::string& password);
  /*Get Channels for User position. It should return channels, available
   * for this user at radius @radius@  */
  std::list<Channel> getChannels(const Position& target, double radius);
 
  /*Subscribe User to channel*/
  void subscribe(const Channel& target);

  /* Get channels list, at which user already has been subscribed */
  std::list<Channel> getSubscription();
 
  static ServerConnection& getInstance();
};

#endif











