#ifndef __TextMessage__h__
#define __TextMessage__h__

#include <QString>
#include <QList>
#include "Tag.h"

class TextMessage 
{
  std::list<Tag> m_tags;
  std::string m_text;

public: 
  TextMessage();
  TextMessage(  std::list<Tag> tags, std::string text);
  const std::list<Tag>& getTags()const;
  const std::string& getText() const;
};

#endif

