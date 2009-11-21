#include "Tag.h"

Tag::Tag(){
    m_type=TagType();
    m_value="";
}


Tag::Tag(const TagType& type, const std::string &value){
    m_type=type;
    m_value=value;
}

const TagType& Tag::getType() const{
    return m_type;
}

const std::string& Tag::getValue() const{
    return m_value;
}
