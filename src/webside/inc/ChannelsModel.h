#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include <Wt/WAbstractTableModel>

#include "DataMarks.h"
#include "Channel.h"
#include "Handle.h"
#include <sstream>

using namespace Wt;

class ChannelsModel : public WAbstractTableModel
{

    std::string m_token;
    CHandlePtr<common::Channels> m_subscribedChannels;
    CHandlePtr<common::Channels> m_availableChannels;

public:
    ChannelsModel(const std::string &token, WObject *parent);
    virtual int columnCount(const WModelIndex & parent = WModelIndex()) const;
    virtual int rowCount(const WModelIndex & parent = WModelIndex()) const;
    virtual boost::any data(const WModelIndex & index,
                            int role = DisplayRole) const;
    virtual boost::any headerData(int section,
                                  Orientation orientation = Horizontal,
                                  int role = DisplayRole) const;
    WFlags<ItemFlag> flags(const WModelIndex &index) const;

    CHandlePtr<common::Channels> getAvailableChannels() const;
    virtual bool setData(const WModelIndex &index, const boost::any &value, int role = EditRole);
//    CHandlePtr<common::Channels> getSubscribedChannels() const;          
    void update();

    /* signals */
    Wt::Signal<> channelsUpdated;
};
#endif
