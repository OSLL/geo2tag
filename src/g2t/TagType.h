#ifndef __TagType__h__
#define __TagType__h__

#include <string>

class TagType
{
public:
  enum Type
  {
    GEOGRAPHICAL,
    USER
  };
private:  
  std::string m_value; // tag value
  Type     m_class;
public:
  TagType();
  TagType(const Type type, const std::string& value);
  const std::string& getValue() const;
  const int getClass() const;
};

#endif

