#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include "Contact.h"

class ContactModel: public QAbstractListModel
{
    Q_OBJECT
private:
    QList<QSharedPointer<Contact> > contacts;
public:
    enum ContactRoles{
        NameRole = Qt::UserRole+1,
        CustomNameRole
    };

    ContactModel(QObject *parent=0);
    void addContact(QSharedPointer<Contact> contact);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

};

#endif // CONTACTMODEL_H
