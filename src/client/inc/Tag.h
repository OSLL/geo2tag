#ifndef __Tag__h__
#define __Tag__h__

#include <string>

class Tag
{
  TagType     m_type;  // tag type  
  std::string m_value; // tag value
  
public:
  
  Tag(const TagType& type, const std::string &value);

};

#endif

