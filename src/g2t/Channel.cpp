#include "Channel.h"


Channel::Channel(){
    m_name="";
    m_position=Position();
}

Channel::Channel(const std::string& name,const Position& pos){
    m_name=name;
    m_position=pos;
  }

const Position& Channel::getPosition() const{
    return m_position;
}

const std::string& Channel::getName() const{
    return m_name;
}

const std::list<TextMessage>& Channel::getContent() const{
    return m_content;
}

