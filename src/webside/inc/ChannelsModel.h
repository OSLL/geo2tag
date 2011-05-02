#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include <Wt/WAbstractTableModel>

#include "DataMarks.h"
#include "Channel.h"
#include <sstream>

#include "SubscribedChannelsListQuery.h"
#include "AvailableChannelsListQuery.h"
#include "SubscribeChannelQuery.h"
#include "UnsubscribeChannelQuery.h"

using namespace Wt;

class ChannelsModel : public WAbstractTableModel
{
  GUI::SubscribedChannelsListQuery subscribedQuery;
  GUI::AvailableChannelsListQuery availableQuery;
  GUI::SubscribeChannelQuery subscribeQuery;
  GUI::UnsubscribeChannelQuery unsubscribeQuery;

  QSharedPointer<User> m_user;
  //    std::string m_token;
  QSharedPointer<Channels> m_subscribedChannels;
  QSharedPointer<Channels> m_availableChannels;

  public:
    ChannelsModel(const QSharedPointer<User> user, WObject *parent);
    virtual int columnCount(const WModelIndex & parent = WModelIndex()) const;
    virtual int rowCount(const WModelIndex & parent = WModelIndex()) const;
    virtual boost::any data(const WModelIndex & index,
      int role = DisplayRole) const;
    virtual boost::any headerData(int section,
      Orientation orientation = Horizontal,
      int role = DisplayRole) const;
    WFlags<ItemFlag> flags(const WModelIndex &index) const;

    QSharedPointer<Channels> getAvailableChannels() const;
    virtual bool setData(const WModelIndex &index, const boost::any &value, int role = EditRole);
    //    QSharedPointer<Channels> getSubscribedChannels() const;
    void update();

    /* signals */
    Wt::Signal<> channelsUpdated;
    };
#endif
