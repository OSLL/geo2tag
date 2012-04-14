#include "ContactModel.h"
#include <QDebug>
#include <QDateTime>

ContactModel::ContactModel(QObject *parent): QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CustomNameRole] = "customname";
    roles[ImageRole] = "image";
    roles[LatRole] = "lat";
    roles[LngRole] = "lng";

    setRoleNames(roles);
}

void ContactModel::addContact(QSharedPointer<Contact> contact)
{
   // contacts.push_back(contact);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
   // contacts << contact;
    contacts.push_back(contact);
    endInsertRows();
     emit dataChanged(createIndex(0,0),createIndex(contacts.size(),0));
}

void ContactModel::removeContact(const QString &contactName)
{
    int n = getContactNumByName(contactName);
    beginRemoveRows(QModelIndex(),n,n);
    contacts.removeAt(n);
    endRemoveRows();
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
            if (contact->getLastMark()->getTime()>QDateTime::currentDateTime().addSecs(-3600))
                    res = QVariant("qrc:/images/green_mark.svg");
            else
                    if (contact->getLastMark()->getTime()>QDateTime::currentDateTime().addSecs(-7200))
                        res = QVariant("qrc:/images/blue_mark.svg");
            else
                        res = QVariant("qrc:/images/gray_mark.svg");

            break;
        case LatRole:
            res = contact->getLastMark()->getLatitude();
            break;
        case LngRole:
            res = contact->getLastMark()->getLongitude();
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
int ContactModel::getContactNumByName(const QString &contactName)
{
    for (int i=0; i<contacts.size(); i++)
        if (contacts.at(i)->getChannelName()==contactName)
            return i;
    QSharedPointer<Contact> contact = QSharedPointer<Contact>(new Contact(contactName,contactName));
    addContact(contact);
    return contacts.size()-1;
}

void ContactModel::drawPins() {

    emit dataChanged(createIndex(0,0),createIndex(contacts.size(),0));
}
