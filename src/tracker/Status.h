#include <QDateTime>
#include <QString>

struct Status
{
    QDateTime datetime;
    QString status;
    QString description;
    bool valid;
};
