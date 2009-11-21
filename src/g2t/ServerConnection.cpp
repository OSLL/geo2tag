#include "ServerConnection.h"

ServerConnection::ServerConnection(){
    m_user=User::NONE;
  }
  ///////////////////////////////////////////
 //If you remove "inline" it will not work//
///////////////////////////////////////////

inline void ServerConnection::initInstance(){//There will be params with such server is starting
    current_con= new ServerConnection;
  }

inline void ServerConnection::deInit(){
    delete current_con;
    current_con=0;
  }

bool ServerConnection::isLogged() const{
    return m_user.isNone();
  }

bool ServerConnection::login(const User& t_user){
    m_user=User::NONE;
    if (t_user.getName()==t_user.getPass()){
        m_user=t_user;
        user_channels.clear();//New user - new channels
    }
    return isLogged();
  }

std::list<Channel> ServerConnection::getChannels(const Position& target, double radius){
     std::list<Channel> channels,subs=m_channels; 
     for(std::list<Channel>::const_iterator i=subs.begin();i!=subs.end();i++)//Search channels with coordinates in radius @radius@
         if ((*i).getPosition().distance(target)<=radius*radius) channels.push_back(*i);
     return channels;
  }

/*Subscribe User to channel*/
void ServerConnection::subscribe(const Channel& target){ 
     user_channels.push_back(target);
}


std::list<Channel> ServerConnection::getSubscription(){
     return user_channels;
}
 
inline ServerConnection& ServerConnection::getInstance(){
     if(!current_con) initInstance();
     return *current_con;
}
