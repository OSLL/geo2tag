#include "MarksModel.h"

#include <algorithm>

#include "UserInternal.h"
#include "DbSession.h"
#include "defines.h"
bool comp(CHandlePtr<common::DataMark> x1,CHandlePtr<common::DataMark> x2){return x1->getTime() < x2->getTime();}

MarksModel::MarksModel(const WString &channel, WObject *parent)
    : WAbstractTableModel(parent)
{
    CHandlePtr<common::DataMarks> marks = common::DbSession::getInstance().getMarks();
    m_marks=makeHandle(new common::DataMarks());
    m_marks->clear();
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
    CHandlePtr<common::Channels> channels;

    for (int i = 0; i < users->size(); i++)
    {
        CHandlePtr<loader::User> user = users->at(i).dynamicCast<loader::User>();
        WString token = WString(user->getToken());
        if (token == WString(DEFAULT_TOKEN))
        {
            channels = user->getSubscribedChannels();
            break;
        }
    }

    for (size_t y = 0; y < marks->size(); y++)
    {
        for (size_t i = 0; i < channels->size(); i++)
        {
            if (marks->at(y)->getChannel() == channels->at(i)) // ((*marks)[y]->getChannel() == (*channels)[i])
            {
                m_marks->push_back(marks->at(y)); // ((*marks)[y]);
            }
        }
    }

    std::sort(m_marks->begin(), m_marks->end(), comp);
}

int MarksModel::columnCount(const WModelIndex & parent) const
{
    return 6;
}

int MarksModel::rowCount(const WModelIndex & parent) const
{
    return m_marks->size();
}

boost::any MarksModel::data(const WModelIndex & index,
                              int role) const
{
    switch (index.column())
    {
    case 0:
        return formatTime(m_marks->at(index.row())->getTime().getTime(),"%d %b %Y %H:%M:%S");
        // return m_marks->at(index.row())->getTime();
    case 1:
        return m_marks->at(index.row())->getDescription();
    case 2:
        return m_marks->at(index.row())->getChannel()->getName();
    case 3:
        return m_marks->at(index.row())->getLatitude();
    case 4:
        return m_marks->at(index.row())->getLongitude();
    case 5:
        return m_marks->at(index.row())->getUrl();
    default:
        return "undefined";
    }
return "indefined";
}

boost::any MarksModel::headerData(int section,
                                    Orientation orientation,
                                    int role) const
{
    if (section == 0)
        return "time";
    else if (section == 1)
        return "description";
    else if (section == 2)
        return "channel";
    else if (section == 3)
        return "latitude";
    else if (section == 4)
        return "longitude";
    else if (section == 5)
        return "url";
}

CHandlePtr<common::DataMarks> MarksModel::getMarks() const
{
    return m_marks;
}
