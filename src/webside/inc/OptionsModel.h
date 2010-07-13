#ifndef OPTIONSMODEL_H
#define OPTIONSMODEL_H

#include <Wt/WAbstractTableModel>

using namespace Wt;

class OptionsModel : public WAbstractTableModel
{
    WString configPath;

public:
    OptionsModel(const WString &configPath, WObject *parent = 0);
    
    virtual int columnCount(const WModelIndex & parent = WModelIndex()) const;
    virtual int rowCount(const WModelIndex & parent = WModelIndex()) const;
    virtual boost::any data(const WModelIndex & index,
                            int role = DisplayRole) const;
    virtual boost::any headerData(int section,
                                  Orientation orientation = Horizontal,
                                  int role = DisplayRole) const;
};


#endif /* OPTIONSMODEL_H */
