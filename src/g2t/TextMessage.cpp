#include "TextMessage.h"

TextMessage::TextMessage(){
    m_text="";
}

TextMessage::TextMessage(  std::list<Tag> tags, std::string text){
      m_tags=tags;
      m_text=text;
  }

const std::list<Tag>& TextMessage::getTags()const{
	return m_tags;
}

const std::string& TextMessage::getText() const{
	return m_text;
}
