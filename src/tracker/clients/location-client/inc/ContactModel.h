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
    enum ContactRoles
    {
      NameRole = Qt::UserRole+1,
      CustomNameRole,
      ImageRole,
      LatRole,
      LngRole
    };

    ContactModel(QObject *parent=0);
    void addContact(QSharedPointer<Contact> contact);
    void removeContact(const QString &contactName);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    QList<QSharedPointer<Contact> > getContacts();
    QSharedPointer<Contact> getContactByName(const QString &contactName);
    int getContactNumByName(const QString &contactName);
  public slots:
    void drawPins();
    void setCustomNameByIndex(/*const QModelIndex &*/int index, const QString & newName);

};
#endif                                  // CONTACTMODEL_H
