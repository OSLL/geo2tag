#ifndef __Tag__h__
#define __Tag__h__

#include <string>
#include "TagType.h"
class Tag
{
  TagType     m_type;  // tag type  
  std::string m_value; // tag value
  
public:
  Tag();
  Tag(const TagType& type, const std::string &value);
  const TagType& getType() const;
  const std::string& getValue() const;

};

#endif

