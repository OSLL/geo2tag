#include "ContactModel.h"
#include <QDebug>

ContactModel::ContactModel(QObject *parent): QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CustomNameRole] = "customname";
    roles[ImageRole] = "image";
    roles[X] = "X";
    roles[Y] = "Y";
    setRoleNames(roles);
}

void ContactModel::addContact(QSharedPointer<Contact> contact)
{
   // contacts.push_back(contact);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    contacts << contact;
    endInsertRows();
     emit dataChanged(createIndex(0,0),createIndex(contacts.size(),0));
}

int ContactModel::rowCount(const QModelIndex & parent) const {
    return contacts.size();
}

QVariant ContactModel::data(const QModelIndex & index, int role) const {

    if (index.row() < 0 || index.row() > contacts.count())
        return QVariant();
    QSharedPointer<Contact> contact = contacts[index.row()];

    QVariant res;
        switch (role) {
        case NameRole:
            if (!contact->getStatus()==MY_CHANNEL)
            res = contact->getChannelName();
            break;
        case CustomNameRole:
            if (!contact->getStatus()==MY_CHANNEL)
            res = contact->getCustomName();
            break;
        case ImageRole:
            if (contact->getLastMark()->getLatitude() < bottomRightLat && contact->getLastMark()->getLatitude() > topLeftLat &&
                     contact->getLastMark()->getLongitude() < topLeftLng && contact->getLastMark()->getLongitude() > bottomRightLng)
                res = "";
            else
            {

            if (contact->getStatus()==MY_CHANNEL)
                res = QVariant("qrc:/images/red_mark.svg");
            else
                if (contact->getStatus()==AVAILABLE)
                    res = QVariant("qrc:/images/green_mark.svg");
            else
                    if (contact->getStatus()==LOST)
                        res = QVariant("qrc:/images/gray_mark.svg");
            }
            break;
        case X:
         //   if (contact->getLastMark().isNull()) return QVariant();
            if ( contact->getLastMark()->getLongitude() > topLeftLng && contact->getLastMark()->getLongitude() < bottomRightLng)
                res =QVariant((mapWidth)*(contact->getLastMark()->getLongitude()- topLeftLng)/(bottomRightLng - topLeftLng));


            break;
        case Y:
           // if (contact->getLastMark().isNull()) return QVariant();
            if ( contact->getLastMark()->getLatitude() > bottomRightLat && contact->getLastMark()->getLatitude() < topLeftLat)
                res = QVariant(mapHeight*(contact->getLastMark()->getLatitude() - topLeftLat)/(bottomRightLat - topLeftLat));
            break;
        }
    return res;
}

Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return Qt::ItemIsEnabled;

         return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QList<QSharedPointer<Contact> > ContactModel::getContacts()
{
    return contacts;
}

QSharedPointer<Contact> ContactModel::getContactByName(const QString &contactName)
{

    for (int i=0; i<contacts.size(); i++)
        if (contacts.at(i)->getChannelName()==contactName)
            return contacts.at(i);
    QSharedPointer<Contact> contact = QSharedPointer<Contact>(new Contact(contactName,contactName));
    addContact(contact);
    return contact;

}

void ContactModel::drawPins(QString x,QString y,QString xEnd,QString yEnd,QString xMap,QString yMap) {
    topLeftLat = x.toDouble();
    topLeftLng = y.toDouble();
    bottomRightLat = xEnd.toDouble();
    bottomRightLng = yEnd.toDouble();
    mapWidth = xMap.toDouble();
    mapHeight = yMap.toDouble();
    emit dataChanged(createIndex(0,0),createIndex(contacts.size(),0));
}
