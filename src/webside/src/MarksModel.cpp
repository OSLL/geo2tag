#include "MarksModel.h"

MarksModel::MarksModel(const WString &configPath, WObject *parent)
    : WAbstractTableModel(parent)
{
    
}

int MarksModel::columnCount(const WModelIndex & parent) const
{
    return 5;
}

int MarksModel::rowCount(const WModelIndex & parent) const
{
    return 2;
}

boost::any MarksModel::data(const WModelIndex & index,
                              int role) const
{
    return "no data";
}

boost::any MarksModel::headerData(int section,
                                    Orientation orientation,
                                    int role) const
{
    if (section == 0)
        return "empty";
    else if (section == 1)
        return "description";
    else if (section == 2)
        return "channel";
    else if (section == 3)
        return "latitude";
    else if (section == 4)
        return "longitude";
    else if (section == 5)
        return "time";
}
