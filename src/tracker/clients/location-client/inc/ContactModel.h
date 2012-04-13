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
    double topLeftLat;
    double topLeftLng;
    double bottomRightLat;
    double bottomRightLng;
    double mapWidth;
    double mapHeight;
public:
    enum ContactRoles{
        NameRole = Qt::UserRole+1,
        CustomNameRole,
        ImageRole,
        X,
        Y
    };

    ContactModel(QObject *parent=0);
    void addContact(QSharedPointer<Contact> contact);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    QList<QSharedPointer<Contact> > getContacts();
    QSharedPointer<Contact> getContactByName(const QString &contactName);
public slots:
    void drawPins(QString x,QString y,QString xEnd,QString yEnd,QString xMap,QString yMap);

};

#endif // CONTACTMODEL_H
