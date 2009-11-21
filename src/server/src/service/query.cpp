#include <iostream>
#include <sstream>
#include <syslog.h>
#include <string.h>
#include "query.h"

Query::Query(const std::string &s) {
    enum States {
        READ_NAME,
        READ_VALUE
    } state;

    size_t j = 0;
    std::string paramName(""), paramValue("");
    state = READ_NAME;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '=') {
            paramName = s.substr(j, i - j);
            j = i + 1;
            state=READ_VALUE;
        }
        if (s[i] == '&') {
            if (state == READ_NAME) {
                // variable without value
                m_params[s.substr(j, i - j)]="";
                j = i + 1;
                continue;
            }
            paramValue = s.substr(j, i - j );
            j = i + 1;
            m_params[paramName] = paramValue;
            state=READ_NAME;
        }
    }
    if (paramName.size()) {
        paramValue = s.substr(j, s.size() - j);
        m_params[paramName] = paramValue;
    }

    std::ostringstream ss;
    for(std::map<std::string, std::string>::const_iterator it=m_params.begin();it!=m_params.end(); ++it)
      ss << it->first << "=" << it->second << " &&& ";
    syslog(LOG_INFO, ss.str().c_str());
}

const std::string& Query::getParam(const std::string& param) const
{
  static std::string nothing("");
  if(m_params.count(param))
    return m_params.find(param)->second;
 return nothing;
}

