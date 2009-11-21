#ifndef __TagType__h__
#define __TagType__h__

#include <string>

class TagType
{
public:
  enum Class
  {
    GEOGRAPHICAL,
    USER
  };
private:  
  std::string m_value; // tag value
  Class       m_class;
public:  

  TagType(const Class& type, const std::string& value); 
};

#endif

