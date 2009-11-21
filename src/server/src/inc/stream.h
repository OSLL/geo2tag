#ifndef _STREAM_H_
#define _STREAM_H_

#include <fcgiapp.h>
#include <stream>
#include "answer.h"

class Stream: public std::stream
{
    FCGX_Stream *m_stream;

public:
    Stream(FCGX_Stream *stream);

    Stream& operator <<(Stream &stm, const Answer& answ);
};
#endif
