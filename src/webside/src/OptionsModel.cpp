#include "OptionsModel.h"

OptionsModel::OptionsModel(const WString &configPath, WObject *parent)
    : WAbstractTableModel(parent)
{
    
}

int OptionsModel::columnCount(const WModelIndex & parent) const
{
    return 2;
}

int OptionsModel::rowCount(const WModelIndex & parent) const
{
    return 1;
}

boost::any OptionsModel::data(const WModelIndex & index,
                              int role) const
{
    return "no data";
}

boost::any OptionsModel::headerData(int section,
                                    Orientation orientation,
                                    int role) const
{
    if (section == 0)
        return "Option";
    else if (section == 1)
        return "Value";
}
