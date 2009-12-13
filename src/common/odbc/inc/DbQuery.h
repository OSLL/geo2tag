/*  */
/*!
 * \file DbQuery.h
 * \brief заголовок базовых классов доступа к бд через ODBC
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag Real Time Project
 *
 * 
 * ------------------------------------------------------------------------ */

#ifndef _DBQUERY_H_INCLUDED_1f66b57a_934e_4362_abef_c09fc91841e1
#define _DBQUERY_H_INCLUDED_1f66b57a_934e_4362_abef_c09fc91841e1

#include "Db.h"
#include <vector>

namespace ODBC
{
  /*!
   * \brief абстрактный базовый класс параметризованных запросов
   */
  class CDbQuery : public CDbStmt
  {
  protected:
    /*! последний выполненный SQL */
    const char *m_pSql;


    /*!
     * \brief связывание буферов данных с запросом
     * \param colSet: [in] идентификатор набора колонок,
     *   (работает, если присутствуют BEGIN_COLSET(id))
     * \return успешно ли связано
     */
    bool bind(int colSet=0);

    /*!
     * \brief связывание буферов параметров с запросом
     * \return успешно ли связано
     */
    bool bindParam();

  public:
    virtual ~CDbQuery()
    {
    }

    /*!
     * \brief интерфейс потребителя данных из полей произвольного размера
     */
    struct IDataSink
    {
      virtual ~IDataSink()
      {
      }

      /*!
       * \brief инициализировать потребителя перед началом получения данных
       */
      virtual void init()=0;

      /*!
       * \brief обработать входящий блок данных
       * \param data: [in] буфер с данными
       * \param sz: [in] размер буфера
       * \param lenOrInd: [in] размер доступных для извлечения данных
       * \return продолжать получение данных
       */
      virtual bool putData(const void *data,size_t sz,SQLLEN lenOrInd)=0;
    }; // struct IDataSink
    
    /*!
     * \brief простая реализация строки из БД произвольного размера
     */
    class CDefaultStringDataSink: public IDataSink
    {
      std::string m_str;
      bool m_isNull;

    public:
      CDefaultStringDataSink(): m_isNull(true)
      {
      }

      virtual void init();
      virtual bool putData(const void *data,size_t sz,SQLLEN lenOrInd);

      bool isNull() const
      {
        return m_isNull;
      }

      std::string& getStr()
      {
        return m_str;
      }

      const std::string& getStr() const
      {
        return m_str;
      }
    }; // class CDefaultStringDataSink


    /*!
     * \brief описатель выходного параметра (колонки) запроса
     */     
    struct CFieldDef
    {
      /*! порядковый номер колонки (1..)  */
      long m_id;
      /*! имя колонки (для проверки целостности, NULL -- не проверять)  */
      const char *m_pName;
      /*! идентификатор С-типа колонки (SQL_C_*)  */
      long m_cType;
      /*! выходной буфер */
      void *m_p;
      /*! размер буфера */
      long m_sz;
      /*! размер запрошенных данных/признак NULL в колонке
       *  NULL если отсутствует (чревато ошибкой при fetch() если в колонке NULL)
       */
      SQLLEN *m_pLen;

      /*! потребитель данных, NULL если отсутствует */
      IDataSink *m_pDataSink;
    };

    /*!
     * \brief описатель входного параметра запроса
     */     
    struct CParDef
    {
      /*! порядковый номер колонки (1..)  */
      long m_id;
      /*! идентификатор С-типа колонки (SQL_C_*)  */
      long m_cType;
      /*! идентификатор SQL-типа колонки (SQL_*)  */
      long m_sqlType;
      /*! выходной буфер */
      void *m_p;
      /*! размер буфера */
      long m_sz;
      /*! размер запрошенных данных/признак NULL
       *  NULL если отсутствует (чревато ошибкой при fetch() если в колонке NULL)
       */
      SQLLEN *m_pLen;

      friend std::ostream& operator << (std::ostream &stm,const CParDef &a);
    };
    
    typedef std::vector<CParDef> CParList;

  public:
    CDbQuery(const CDbConn &dbConn) : CDbStmt(dbConn)
    {
      m_pSql="no SQL";
    }

    /*!
     * \brief запрос определения набора колонок
     * \param v: [out] вектор определения набора колонок
     * \param colSet: [in] идентификатор требуемого набора колонок
     */
    virtual void getColset(std::vector<CFieldDef> &v,int colSet)=0;

    /*!
     * \brief запрос определения набора параметров
     * \param v: [out] вектор определения набора параметров
     */
    virtual void getParset(CParList &) const {}

    /*!
     * \brief текст запроса SQL по умолчанию
     * \return текст запроса SQL по умолчанию
     *
     * новые запросы определяют свой текст здесь
     * если текст не определен, prepare() должен вызываться с аргументом
     */
    virtual const char *sql() const {return "крокозяблик";}
    
    /*!
     * \brief подготовка запроса к выполнению: prepare, bind, bindParam
     * \param pSql: [in] запрос SQL (опустить для запроса по умолчанию)
     * \return успешно ли приготовлено
     */
    bool prepare(const char *pSql=NULL)
    {
      if(pSql==NULL)
        pSql=sql();

      m_pSql=pSql;

      return CDbStmt::prepare(pSql) && bind() && bindParam();
    }

    /*!
     * \brief чтение строчки из курсора в текущий набор колонок
     * \return прочитано ли
     */
    bool fetch();
    
    /*!
     * \brief чтение строчки из курсора в заданный набор колонок
     * \param colSet: [in] набор колонок, который становится текущим
     * \return прочитано ли
     */
    bool fetch(int colSet)
    {
      return unbind() && bind(colSet) && fetch();
    }

    /*!
     * \brief получение SQL использованного в последнем запросе
     * \return указатель на строчку последнего запроса
     */
    const char *getSql() const
    {
      return m_pSql;
    }

  protected:
    /*!
     * текущие активные привязки установленные последним bind()
     */
    std::vector<CFieldDef> m_fields;

  };

  namespace Exception
  {
    /*!
     * \brief исключение описывающее ошибки в запросах
     */
    class CSql: public CException
    {
    protected:
      /*! текст запроса SQL */
      const char *m_pSql;
      
      /*! параметры запроса */
      CDbQuery::CParList m_param;
  
      class CBuf: private std::vector<char>
      {
      public:
        CBuf(void *buf,long sz): std::vector<char>((char*)buf,sz+(char*)buf)
        {
        }
  
        operator void*()
        {
          return &((*this)[0]);
        }
      };
  
      std::vector<CBuf> m_parVal;
      
    public:
      CSql(const CSource &src,const std::string &comment,
           const std::string &errorText, const CDbQuery &query) :
        CException(src,comment,errorText), m_pSql(query.getSql())
      {
        query.getParset(m_param);
        for(CDbQuery::CParList::iterator i=m_param.begin();i!=m_param.end();i++)
        {
          m_parVal.push_back(CBuf(i->m_p,i->m_sz));
          i->m_p=m_parVal.back();
        }
      }
  
      CSql(const CSql &a) :
        CException(a), m_pSql(a.m_pSql), m_param(a.m_param), m_parVal(a.m_parVal)
      {
        int i;
        CDbQuery::CParList::iterator p;
        
        for(i=0,p=m_param.begin();p!=m_param.end();i++,p++)
        {
          p->m_p=m_parVal[i];
        }
      }
      
      virtual std::ostream& outDescription(std::ostream& stm) const;    
    };
    
    class CEmptyRowset: public CSql
    {
    public:
      CEmptyRowset(const CSource &src,const std::string &comment,
           const std::string &errorText, const CDbQuery &query) :
        CSql(src,comment,errorText,query)
      {
      }
    };
  }//namespace Exception
/*!
 * \param comment: [in,safe] строка описания действия, вызвавшего исключение
 * \param query: [in,unsafe] объект-запрос, из которого извлекать ошибку, текст и параметры
 */
#define SQLXC(comment,query) Exception::CSql(SRC(),(comment),(query).error(),(query))

/*!
 * \param comment: [in,safe] строка описания действия, вызвавшего исключение
 * \param errorText: [in,safe] текст сообщения об ошибке
 * \param query: [in,safe] объект-запрос, из которого извлекать текст и параметры
 */
#define SQLXCE(comment,errorText,query) Exception::CSql(SRC(),(comment),(errorText),(query))

  /*!
   * \brief абстрактный базовый класс параметризованных запросов с исключениями
   */
  class CDbQueryX : public CDbQuery
  {
  protected:
  
    void bind(int colSet=0);
    void bindParam();
    
  public:
    CDbQueryX(const CDbConn &dbConn) : CDbQuery(dbConn)
    {
    }

    /*!
     * \brief чтение строчки из курсора в текущий набор колонок
     * \return прочитано ли
     */
    bool fetch()
    {
      bool r=CDbQuery::fetch();
      if(!r && isFetchFailed())
        throw SQLXC("fetch",*this);
        
      return r;
    }

    /*!
     * \brief чтение строчки из курсора в текущий набор колонок
     * отсутствие данных рассматривается как ошибка
     */
    void fetchNoEmpty()
    {
      STRACE();
      if(!fetch())
        throw Exception::CEmptyRowset(SRC(),"fetchNoEmpty","empty rowset",*this);
    }

    /*!
     * \brief чтение строчки из курсора в заданный набор колонок
     * \param colSet: [in] набор колонок, который становится текущим
     * \return прочитано ли
     */
    bool fetch(int colSet)
    {
      STRACE();
      if(!unbind())
        throw SQLXC("unbind",*this);

      bind(colSet);
      
      return fetch();
    }

    /*!
     * \brief чтение строчки из курсора в заданный набор колонок
     * отсутствие данных рассматривается как ошибка
     * \param colSet: [in] набор колонок, который становится текущим
     */
    void fetchNoEmpty(int colSet)
    {
      STRACE();
      if(!fetch(colSet))
        throw Exception::CEmptyRowset(SRC(),"fetchNoEmpty","empty rowset",*this);
    }

    /*!
     * \brief подготовка запроса к выполнению: prepare, bind, bindParam
     * \param pSql: [in] запрос SQL (опустить для запроса по умолчанию)
     */
    void prepare(const char *pSql=NULL)
    {
      STRACE();
      if(pSql==NULL)
        pSql=sql();

      m_pSql=pSql;

      if(pSql==NULL)
        throw DBXCE("prepare","NULL SQL statement");

      if(!CDbStmt::prepare(pSql))
        throw SQLXC("prepare",*this);

      bind();
      bindParam();
    }

    /* 
     * обертки функций нижнего уровня возбуждающие исключения по ошибкам
     */
    void execute()
    {
      if(!CDbStmt::execute())
        throw SQLXC("execute",*this);
    }

    void execDirect(const char *sql)
    {
      if(!CDbStmt::execDirect(sql))
        throw SQLXC("execDirect",*this);
    }
    
    void close()
    {
      if(!CDbStmt::close())
        throw SQLXC("close",*this);
    }
  };


  /*!
   * \brief вывод описателя параметра в поток
   */       
  std::ostream& operator << (std::ostream &stm,const CDbQuery::CParDef &a);


  template<class Q,class F>
  void disposeQuery(Q &q,F &f)
  {
    STRACE();

    CExecuteClose x(q);
    while(q.fetch())
    {
      f(q);
    }
  }

  template<class Q,class F>
  void disposeQueryNoEmpty(Q &q,F &f)
  {
    STRACE();

    CExecuteClose x(q);
    q.fetchNoEmpty();
    f(q);
    while(q.fetch())
    {
      f(q);
    }
  }
};

/*!
 * \brief секция привязки полей класса к полям запроса
 *
 * размещается в public секции класса запроса
 * и переопределяет виртуальную функцию getColset,
 * шаблонную функцию класса CDbQuery
 */
#define BEGIN_COLMAP() void getColset(std::vector<CFieldDef> &v,int colSet) {v.clear();
#define END_COLMAP()   }

/*!
 * \brief определяет подсекцию альтернативного набора колонок
 * \param a: [in,safe] номер набора колонок
 *
 * класс запроса может иметь несколько наборов колонок
 * и переключаться между ними посредством fetch(colSet)
 */
#define BEGIN_COLSET(a) if(a==colSet){
#define END_COLSET()    }

/*!
 * \brief простое определение буфера колонки (deprecated)
 * \param colNumber: [in,safe] порядковый номер колонки в запросе
 * \param targetType: [in,safe] C-тип данных поля (SQL_C_*)
 * \param target: [in,unsafe] поле данных
 */
#define COL(colNumber,targetType,target) COL_NAME(colNumber,NULL,targetType,target)

/*!
 * \brief определение именованного буфера колонки
 * \param colNumber: [in,safe] порядковый номер колонки в запросе
 * \param colName: [in,safe] имя колонки
 * \param targetType: [in,safe] C-тип данных поля (SQL_C_*)
 * \param target: [in,unsafe] поле данных
 *
 * если запрос возвращает колонку с другим именем,
 * bind (а с ним и prepare) завершаются с ошибкой
 */
#define COL_NAME(colNumber,colName,targetType,target) \
  {CFieldDef x={(colNumber),(colName),(targetType),&(target),sizeof(target),NULL,NULL};v.push_back(x);}

/*!
 * \brief определение именованного буфера колонки с полем длины/признака NULL
 * \param colNumber: [in,safe] порядковый номер колонки в запросе
 * \param colName: [in,safe] имя колонки
 * \param targetType: [in,safe] C-тип данных поля (SQL_C_*)
 * \param target: [in,unsafe] поле данных
 * \param len: [in,safe] SQLLEN поле длины/признака NULL
 *
 * если запрос возвращает колонку, в которой NULL, len принимает значение SQL_NULL_DATA
 * поле связанное \ref COL_NAME в такой ситуации вызывает ошибку в fetch
 */
#define COL_NAME_LEN(colNumber,colName,targetType,target,len) \
  {CFieldDef x={(colNumber),(colName),(targetType),&(target),sizeof(target),&(len),NULL};v.push_back(x);}


/*!
 * \brief определение именованного буфера колонки с интерфейсом накопления данных неопределенного размера
 * \param colNumber: [in,safe] порядковый номер колонки в запросе
 * \param colName: [in,safe] имя колонки
 * \param targetType: [in,safe] C-тип данных поля (SQL_C_*)
 * \param target: [in,unsafe] поле данных
 * \param sink: [in,safe] указатель на интерфейс IDataSink
 *
 * в процессе выполнения fetch, если CDbStmt::fetch завершается успешно, вызывается sink->init() и далее
 * в цикле sink->setData с данными принимаемыми через CDbStmt::getData в буфер target
 */
#define COL_NAME_SINK(colNumber,colName,targetType,target,sink) \
  {CFieldDef x={(colNumber),(colName),(targetType),&(target),sizeof(target),NULL,(sink)};v.push_back(x);}

/*!
 * \brief секция привязки полей класса к параметрам запроса (?)
 */
#define BEGIN_PARMAP() void getParset(CParList &v) const {v.clear();
#define END_PARMAP() }

/*!
 * \brief определение параметра запроса
 * \param parNumber: [in,safe] порядковый номер параметра в запросе
 * \param sourceType: [in,safe] C-тип параметра (SQL_C_*)
 * \param sqlType: [in,safe] SQL-тип параметра (SQL_*)
 * \param source: [in,unsafe] поле параметра
 */
#define PAR(parNumber,sourceType,sqlType,source)\
  {CParDef x={(parNumber),(sourceType),(sqlType),(void*)&(source),sizeof(source),NULL};v.push_back(x);}

#define PAR_SZ(parNumber,sourceType,sqlType,source,sz)\
  {CParDef x={(parNumber),(sourceType),(sqlType),(void*)(source),(sz),NULL};v.push_back(x);}

#define PAR_LEN(parNumber,sourceType,sqlType,source,len)\
  {CParDef x={(parNumber),(sourceType),(sqlType),(void*)&(source),sizeof(source),(SQLLEN*)&(len)};v.push_back(x);}

#endif
