#ifndef _REPLY_H_
#define _REPLY_H_

#include "quick.h"
/*!
 * \brief Answer interface for server replys. 
 */
class Reply 
{
public:
    Reply(const json::QuickInterpreter&)=0;

    virtual std::ostream& operator <<(std::ostream &stm) const=0;
};
#endif
