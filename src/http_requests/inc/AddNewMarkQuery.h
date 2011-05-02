#ifndef ADDNEWMARKQUERY_H
#define ADDNEWMARKQUERY_H

#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "User.h"
#include "DataMarks.h"

class AddNewMarkQuery: public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<DataMark> m_tag;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;
  virtual void processReply(QNetworkReply *reply);

  public:
    AddNewMarkQuery(const QSharedPointer<DataMark> &tag, QObject *parent = 0);

    ~AddNewMarkQuery();

    QSharedPointer<DataMark> getTag();
    const QSharedPointer<DataMark>& getTag() const;

    Q_SIGNALS:

    void tagAdded();
    };
#endif                                                      // ADDNEWMARKQUERY_H
