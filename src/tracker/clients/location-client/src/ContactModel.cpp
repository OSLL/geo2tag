#include "ContactModel.h"

ContactModel::ContactModel(QObject *parent): QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CustomNameRole] = "customname";
    setRoleNames(roles);
}

void ContactModel::addContact(QSharedPointer<Contact> contact)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    contacts << contact;
    endInsertRows();
}

int ContactModel::rowCount(const QModelIndex & parent) const {
    return contacts.count();
}

QVariant ContactModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > contacts.count())
        return QVariant();
    QSharedPointer<Contact> contact = contacts.at(index.row());
    if (role == NameRole)
        return contact->getChannelName();
    else if (role == CustomNameRole)
        return contact->getCustomName();
    return QVariant();
}
