#define AMOUNT_OF_COLUMNS 4
#include <Wt/WContainerWidget>
#include "ChannelsModel.h"
#include "UserInternal.h"
#include "DbSession.h"
#include "defines.h"
#include <Wt/WCheckBox>
#include "DbSession.h"
#include "defines.h"
#include "UserInternal.h"

ChannelsModel::ChannelsModel(const std::string &token, WObject *parent)
    : WAbstractTableModel(parent)
{
    m_token = token;
    m_availableChannels = common::DbSession::getInstance().getChannels();
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
    for (int i = 0; i < users->size(); i++)
    {
        CHandlePtr<loader::User> user = users->at(i).dynamicCast<loader::User>();
//        WString token = WString(user->getToken());
        if (m_token == user->getToken())
        {
            m_subscribedChannels = user->getSubscribedChannels();
            break;
        }
    }
}


int ChannelsModel::columnCount(const WModelIndex & parent) const
{
    return AMOUNT_OF_COLUMNS;
}

int ChannelsModel::rowCount(const WModelIndex & parent) const
{
    return m_availableChannels->size();
}

boost::any ChannelsModel::data(const WModelIndex & index,
                               int role) const
{
    if (role == Wt::DisplayRole || role == Wt::EditRole
        || role == ToolTipRole || CheckStateRole)
    {
        switch (index.column())
        {
        case 0:
            return m_availableChannels->at(index.row())->getName();
        case 1:
            return m_availableChannels->at(index.row())->getDescription();
        case 2:
            return m_availableChannels->at(index.row())->getRadius();
        case 3:{

                //	  WCheckBox *Check;
                //  	  Check = new WCheckBox(/*dynamic_cast<WContainerWidget*>(WObject::parent())*/);
                for (int i=0;i<m_subscribedChannels->size();i++){
                    if (m_subscribedChannels->at(i)==m_availableChannels->at(index.row())){
                        //			Check->setChecked(true);
                        //	return boost::any(Check);
			if (role == Wt::CheckStateRole) return true;
			return "";
                    }
                }
                //	  return *Check;
                if (role == Wt::CheckStateRole) return false;
                return "";
            }
        default:
            return "undefined";
        }
        return "indefined";
    }
    else
    {
        return boost::any();
    }
}

boost::any ChannelsModel::headerData(int section,
                                    Orientation orientation,
                                    int role) const
{
    if (section == 0)
        return "name";
    else if (section == 1)
        return "description";
    else if (section == 2)
        return "radius";
    else if (section == 3)
        return "subscription";
}

CHandlePtr<common::Channels> ChannelsModel::getAvailableChannels() const
{
    return m_availableChannels;
}

#include <Wt/WFlags>

WFlags<ItemFlag> ChannelsModel::flags(const WModelIndex &index) const
{
	if (index.column() == 3){
		return Wt::ItemIsUserCheckable;
	}
	return 0;
//    return WFlags<ItemFlag>();
}


void ChannelsModel::update()
{
    m_availableChannels->clear();
    m_subscribedChannels->clear();
    m_availableChannels = common::DbSession::getInstance().getChannels();
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
    for (int i = 0; i < users->size(); i++)
    {
        CHandlePtr<loader::User> user = users->at(i).dynamicCast<loader::User>();
//        WString token = WString(user->getToken());
        if (m_token == user->getToken())
        {
            m_subscribedChannels = user->getSubscribedChannels();
            break;
        }
    }
    int *a = NULL;
    dataChanged().emit(createIndex(0, 0, a),
                           createIndex(m_availableChannels->size() - 1,
                                       AMOUNT_OF_COLUMNS - 1,
                                       a));
}


bool ChannelsModel::setData(const WModelIndex &index, const boost::any &value, int role)
{
	if (index.column() == 3) {
	        CHandlePtr<common::User> du;
        	CHandlePtr<common::Channel> ch;
		ch = m_availableChannels->at(index.row());
		du = common::DbSession::getInstance().getTokensMap().find(std::string(m_token))->second;
		bool subscribed=false;
		for (int i = 0; i < m_subscribedChannels->size(); i++){
			if (m_subscribedChannels->at(i) == ch) {
				subscribed = true;
				break;
			}
		}
		if (subscribed){
		//unsubscribe
			common::DbSession::getInstance().unsubscribe(du,ch);
		}else {
		//subscribe
            		common::DbSession::getInstance().subscribe(du,ch);
		}
//		index.data(Wt::CheckStateRole)=;
		dataChanged().emit(index, index);
                this->channelsUpdated.emit();
		return true;
	}
	return false;
}
