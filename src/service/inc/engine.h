#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "query.h"
#include "reply.h"

class Engine
{
    Engine();
    
    /*!
     * \brief process Subscribe request.
     * \param query JSON schema with descripion
     * \return Reply object with JSON reply.
     * This routine use SQL/other ways for access to data.
     */
    Reply processSubscribe(const QStringstream& query);

    Reply processAvailableList(const QStringstream& query);

    Reply processSubscribedList(const QStringstream& query);

public:
    static const Engine& getInstance();

    Reply process(QueryType type, const std::istream& stream);
};

#endif
