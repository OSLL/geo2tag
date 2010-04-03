#ifndef _STREAM_H_
#define _STREAM_H_

#include <fcgiapp.h>
#include <sstream>

class IStream
{
public:
  
  virtual void inFromString(const std::string& )=0;
};

class OStream
{
public:
  
  virtual std::string outToString() const=0;
};


class Stream
{
    FCGX_Stream *m_stream;

public:
    Stream(FCGX_Stream *stream);

    Stream& operator <<(const OStream&);
    Stream& operator >>(IStream&);
    
    Stream& operator <<(const std::string&);
    Stream& operator >>(std::string&);
};
#endif
