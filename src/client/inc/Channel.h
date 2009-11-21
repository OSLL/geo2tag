#ifndef __Channel__h__
#define __Channel__h__

#include <list>
#include "Position.h"
#include "TextMessage.h"

class Channel:
{
  Position m_position; // Each channel have personal geographical position

  std::list<TextMessage> m_content; // Channel's content
public:
  
};


#endif

