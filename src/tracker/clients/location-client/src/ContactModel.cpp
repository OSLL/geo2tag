#include "ContactModel.h"
#include <QDebug>

ContactModel::ContactModel(QObject *parent): QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CustomNameRole] = "customname";
    setRoleNames(roles);
}

void ContactModel::addContact(QSharedPointer<Contact> contact)
{
   // contacts.push_back(contact);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    contacts << contact;
    endInsertRows();
}

int ContactModel::rowCount(const QModelIndex & parent) const {
    return contacts.size();
}

QVariant ContactModel::data(const QModelIndex & index, int role) const {
    qDebug()<<"index.row()="<<index.row();
    if (index.row() < 0 || index.row() > contacts.count())
        return QVariant();
    QSharedPointer<Contact> contact = contacts[index.row()];
    if (role == NameRole)
        return contact->getChannelName();
    else if (role == CustomNameRole)
        return contact->getCustomName();
    return QVariant();
}
