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
  
  ServerConnection(){
      m_user=User::NONE;
  }

  static ServerConnection* current_con;

  /*Return TRUE, if current connection has been initialized by some User*/
  void initInstance(){//There will be params with such server is startingprivate -Vera Kononova 9/15/09 2:09 PM 
      current_con= new ServerConnection;
  }

public:
static void deInit(){
    delete current_con;
  }
 
  bool isLogged() const{
      return m_user==User::NONE;  
  }

  /* Login as User.*/
  bool login(const User& t_user){
      m_user=User::NONE;
      if (t_user.m_name==t_user.m_pass){
          m_user=t_user;
          user_channels.clear();//New user - new channels
      }
      return isLogged();
  }


  /*Get Channels for User position. It should return channels, available
   * for this user at radius @radius@
   */
  std::list<Channel> getChannels(const Position& target, double radius){
      std::list<Channel> channels,subs=m_channels; 
      for(std::list<Channel>::const_iterator i=subs.begin();i!=subs.end();i++)//Search channels with coordinates in radius @radius@
          if ((*i).distance(target)<=radius*radius) channels.push_back(*i);
      return channels;
  }

 
  /*Subscribe User to channel*/
  void subscribe(const Channel& target){ 
  user_channels.push_back(target);
  }

  /* Get channels list, at which user already has been subscribed */
  std::list<Channel> getSubscription(){
      return user_channels;
  }
 
  static ServerConnection& getInstance(){
    if(!current_con)
       initInstance();
    return *current_con;
}
};

#endif











