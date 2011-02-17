#ifndef MARKSMODEL_H
#define MARKSMODEL_H

#include <Wt/WAbstractTableModel>

#include "DataMarks.h"
#include "Channel.h"
#include <sstream>

using namespace Wt;

class MarksModel : public WAbstractTableModel
{


    std::string m_token;
    double m_latitude;
    double m_longitude;
    double m_radius;

    QSharedPointer<DataMarks> m_marks;

public:

    enum Type
    {
      ALL_MARKS,
      THE_LATEST_FROM_EACH_MEMBER
    };

    Type   m_type;
    MarksModel(const std::string &token, const WString &channel, WObject *parent = 0);
    
    virtual int columnCount(const WModelIndex & parent = WModelIndex()) const;
    virtual int rowCount(const WModelIndex & parent = WModelIndex()) const;
    virtual boost::any data(const WModelIndex & index,
                            int role = DisplayRole) const;
    virtual boost::any headerData(int section,
                                  Orientation orientation = Horizontal,
                                  int role = DisplayRole) const;
    WFlags<ItemFlag> flags(const WModelIndex &index) const;

    QSharedPointer<DataMarks> getMarks() const;

    void update();
};


#endif /* OPTIONSMODEL_H */
