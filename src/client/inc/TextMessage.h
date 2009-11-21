#ifndef __TextMessage__h__
#define __TextMessage__h__

#include <string>
#include <list>

class TextMessage 
{
  std::list<Tag> m_tags;
  std::string m_text;

public: 
  TextMessage();
  
};

#endif

