#ifndef _QUERY_H_
#define _QUERY_H_

#include <map>
#include <string>
#include <sstream>
#include "stream.h"

class Query {
    std::map<std::string, std::string> m_params;
    
    std::stringstream m_query; //! query's data

public:
    enum QueryType {
        SUBSCRIBE,        //! Query subscribe to the channel
        AVAILABLE_LIST,   //! The list with available channels
        SUBSCRIBED_LIST,  //! The list with subscribed by user channels
        UNKNOWN
    };

    Query(const std::string &s, const Stream& stm);

    const std::string& getParam(const std::string& param) const;
    
    /*!
     * \brief Routine return type of query
     */
    QueryType getType() const;
    
    /*!
     * \brief Routine return content of request
     */
    const std::istream& getStream() const;
};

#endif
