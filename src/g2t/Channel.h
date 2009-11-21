#ifndef __Channel__h__
#define __Channel__h__

#include <list>
#include "Position.h"
#include "TextMessage.h"

class Channel
{
  Position m_position; // Each channel have personal geographical position
  std::string m_name;
  std::list<TextMessage> m_content; // Channel's content
public:
  Channel();
  Channel(const std::string& name,const Position& pos);
  const Position& getPosition() const;
  const std::string& getName() const;
  const std::list<TextMessage>& getContent() const;
};


#endif

