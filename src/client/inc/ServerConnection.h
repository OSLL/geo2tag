#ifndef __ServerConnection__h__
#define __ServerConnection__h__

#include <list>
#include "User.h"
#include "Channel.h"
#include "Position.h"

class ServerConnection
{
  User m_user; // Current user
  std::list<Channel> user_channels;// Channels tha user subscrubed
  std::list<Channel> m_channels; // All channels
  
  ServerConnection();

  static ServerConnection* current_con;

  void initInstance();

public:

  static void deInit();
 
  bool isLogged() const;

  /* Login as User.*/
  bool login(const User& t_user);


  /*Get Channels for User position. It should return channels, available
   * for this user at radius @radius@
   */
  std::list<Channel> getChannels(const Position& target, double radius);
 
  /*Subscribe User to channel*/
  void subscribe(const Channel& target);

  /* Get channels list, at which user already has been subscribed */
  std::list<Channel> getSubscription();
 
  static ServerConnection& getInstance();
};

#endif











