#include "ServerConnection.h"

ServerConnection::ServerConnection(){
    m_user=User::NONE;
  }

void ServerConnection::initInstance(){//There will be params with such server is starting
    current_con= new ServerConnection;
  }

static void ServerConnection::deInit(){
    delete current_con;
  }
bool ServerConnection::isLogged() const{
    return m_user==User::NONE;  
  }

bool ServerConnection::login(const User& t_user){
    m_user=User::NONE;
    if (t_user.m_name==t_user.m_pass){
        m_user=t_user;
        user_channels.clear();//New user - new channels
    }
    return isLogged();
  }

std::list<Channel> ServerConnection::getChannels(const Position& target, double radius){
     std::list<Channel> channels; 
     for(std::list<Channel>::const_iterator i=m_channels.begin();i!=m_channels.end();i++)//Search channels with coordinates in radius @radius@
         if ((*i).distance(target)<=radius*radius) channels.push_back(*i);
     return channels;
  }

/*Subscribe User to channel*/
void ServerConnection::subscribe(const Channel& target){ 
     user_channels.push_back(target);
}


std::list<Channel> ServerConnection::getSubscription(){
     return user_channels;
}
 
static ServerConnection& ServerConnection::getInstance(){
     if(!current_con)
     	initInstance();
     return *current_con;
}
