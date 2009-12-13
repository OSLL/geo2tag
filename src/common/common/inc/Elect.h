/*  */
/*!
 * \file Elect.h
 * \brief заголовок класса CElect
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CElect_H_E510BE6F_8FC0_4D70_AD57_4937408BF239_INCLUDED_
#define _CElect_H_E510BE6F_8FC0_4D70_AD57_4937408BF239_INCLUDED_

#include <map>
#include <list>
#include <algorithm>
#include <functional>
#include "Exception.h"

class CElectException: public CExceptionSource
{
public:
  CElectException(const CSource& src): CExceptionSource(0,src,0)
  {
  }
};

/*!
 * \brief фильтр выбора простым большинством.
 * 
 * если два кандидата набирают равное число голосов, побеждает получивший последний голос.
 * \param Name: [typename] тип идентификатора кандидата
 * \param Pred: [typename] предикат сравнения имен кандидатов
 */
template<typename Name,typename Pred=std::less<Name> >
class CElect
{
  std::map<Name,long,Pred> m_voteCount;
  std::map<long,Name,std::greater<long> > m_chart;
public:
  /*!
   * \brief голосовать за кандидата
   * \param name: [in] имя кандидата
   */
  void vote(const Name& name)
  {
    m_chart[++m_voteCount[name]]=name;
  }
  /*!
   * \brief получить имя победителя
   * \return имя победителя
   * \exception CElectException если невозможно определить победителя
   */
  const Name& getWinner() const
  {
    if(!m_chart.empty())
      return m_chart.begin()->second;
      
    throw CElectException(SRC());
  }
};


/*!
 * \brief фильтр выбора последовательным большинством.
 *
 * побеждает последний кандидат набравший не менее leastVotes голосов.
 * если таких нет, побеждает последний, набравший голосов как минимум на leastGap больше,
 * чем следующий более поздний.
 * если и таких нет или список пуст, то исключение...
 *
 * \param Name: [typename] тип идентификатора кандидата
 * \param Pred: [typename] предикат сравнения имен кандидатов
 */
template<typename Name,typename Pred=std::less<Name> >
class CElectSeq
{
  size_t m_leastVotes;/*!< количество голосов для безусловной победы */
  size_t m_leastGap;/*!< минимальный отрыв от более позднего кандидата для победы */
  size_t m_scoreSz;/*!< количество отслеживаемых кандидатов */
  
  struct CCandidate;

  typedef std::map<Name,CCandidate,Pred> CRegistry;
  typedef std::list<typename CRegistry::iterator> CScore;
  
  struct CCandidate
  {
    size_t m_voteCount;
    typename CScore::iterator m_score;
    
    CCandidate()
    {
      m_voteCount=0;
    }
  };
  
  /*! реестр кандидатов */
  CRegistry m_registry;
  /*! хронологический список кандидатов, front() -- самый недавний */
  CScore m_score;
  
  void insertCopy(typename CRegistry::iterator it)
  {
    typename CRegistry::iterator c=m_registry.insert(*it).first;
    m_score.push_front(c);
    c->second.m_score=m_score.begin();
  }
  
  void copy(const CElectSeq& obj)
  {
    m_leastVotes=obj.m_leastVotes;
    m_leastGap=obj.m_leastGap;
    m_scoreSz=obj.m_scoreSz;
    
    std::for_each(obj.m_score.rbegin(),obj.m_score.rend(),
      std::bind1st(std::mem_fun(&CElectSeq::insertCopy),this));
  }
  
public:
  CElectSeq(size_t leastVotes,size_t leastGap,size_t scoreSz):
    m_leastVotes(leastVotes),m_leastGap(leastGap),m_scoreSz(scoreSz)
  {
  }

  CElectSeq(const CElectSeq& obj)
  {
    copy(obj);
  }
  
  CElectSeq& operator =(const CElectSeq& obj)
  {
    m_registry.clear();
    m_score.clear();
    copy(obj);
    return *this;
  }
  
  /*!
   * \brief голосовать за кандидата.
   *
   * имя кандидата не обновляется при каждом голосовании
   * (а разные имена могут принадлежать одному классу эквивалентности по Pred).
   * \param name: [in] имя кандидата
   */
  void vote(const Name& name)
  {
    CCandidate& c=m_registry[name];
    
    m_score.push_front(m_registry.find(name));
    
    if(c.m_voteCount++)
      m_score.erase(c.m_score);
    
    c.m_score=m_score.begin();
    
    while(m_score.size()>m_scoreSz)
    {
      m_registry.erase(m_score.back());
      m_score.pop_back();
    }
  }
  
  /*!
   * \brief получить имя победителя
   * \return имя победителя
   * \exception CElectException если невозможно определить победителя
   */
  const Name& getWinner() const
  {
    typename CScore::const_iterator it,j;
    for(it=m_score.begin();it!=m_score.end();++it)
    {
      if((*it)->second.m_voteCount>=m_leastVotes)
        return (*it)->first;
        
      j=it;
      
      if(it!=m_score.begin() && 
        (*it)->second.m_voteCount>=(*--j)->second.m_voteCount+m_leastGap)
        return (*it)->first;
    }
    
    if(!m_score.empty())
      return m_score.front()->first;
      
    throw CElectException(SRC());
  }
};


#endif //_CElect_H_E510BE6F_8FC0_4D70_AD57_4937408BF239_INCLUDED_

/* ===[ End of file  ]=== */
