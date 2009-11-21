#include "TagType.h"

TagType::TagType(){
    m_class=GEOGRAPHICAL;
}

TagType::TagType(const Type type, const std::string& value){
    m_class=type;
    m_value=value;
}

const std::string& TagType::getValue() const {
	return m_value;
}

const int TagType::getClass()const{
	return m_class;
}
