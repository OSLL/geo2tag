#include "OptionsModel.h"

#include "Parameters.h"

#include "WFlags"

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

WFlags<ItemFlag> OptionsModel::flags(const WModelIndex &index) const
{
    if (index.column() == 1)
    {
        return (/*Wt::ItemIsSelectable |*/ Wt::ItemIsEditable);
    }

    return 0 /*Wt::ItemIsSelectable*/;
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

boost::any OptionsModel::data(const WModelIndex & index,
                              int role) const
{
    switch (index.row())
    {
    case 0: /* default_radius */
        switch (index.column())
        {
        case 0:
            return "default_radius";
        case 1:
            return common::Parameters::getInstance().getDefaultRadius();
        default:
            return "no data";
        }
        break;
    default:
        return "no data";
    }

    return "no data";
}

bool OptionsModel::setData(const WModelIndex &index, const boost::any &value, int role)
{
    if (index.column() != 1) /* not column with values */
    {
        return false;
    }

    int val_int;
    switch (index.row())
    {
    case 0: /* default radius */
        /* set value */
        val_int = atoi(asString(value).toUTF8().c_str());
        if (val_int < 0)
            return false;
        common::Parameters::getInstance().setDefaultRadius(val_int);
        break;
    default:
        return false;
    }

    dataChanged().emit(index, index);
    return true;

}
