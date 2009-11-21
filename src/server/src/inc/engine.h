#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "query.h"
#include "reply.h"

class Engine
{
    Engine();

public:
    static const Engine& getInstance();

    Reply process(QueryType type, const std::istream& stream);
};

#endif
