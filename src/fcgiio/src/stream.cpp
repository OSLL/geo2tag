#include "stream.h"
#include <syslog.h>

Stream::Stream(FCGX_Stream *stream):m_stream(stream)
{
}

Stream& Stream::operator <<(const OStream& obj)
{
  const std::string &s = obj.outToString();
  syslog(LOG_INFO,"writing to Stream string %s",s.c_str());
  FCGX_PutStr(s.c_str(), s.size(), m_stream);
  return *this;
}

Stream& Stream::operator >>(std::string& s) {
  char ch=0;
  for(;;)
  {
    ch = FCGX_GetChar(m_stream);
    if(ch != -1)
       s += ch;
    else
       break;
  } 
  syslog(LOG_INFO, "end stream, read: [%s], last symbol = [%ld]", s.c_str(), s[s.size()-1]);
  return *this;
}

Stream& Stream::operator <<(const std::string& s)
{
  FCGX_PutStr(s.c_str(), s.size(), m_stream);
  return *this;
}

Stream& Stream::operator >>(IStream& obj)
{
  std::string s("");
  char ch=0;
  for(;;)
  {
    ch = FCGX_GetChar(m_stream);
    if(ch != -1)
       s += ch;
    else
       break;
  } 
  obj.inFromString(s);
  return *this;
}

