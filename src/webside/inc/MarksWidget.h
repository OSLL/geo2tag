#ifndef MARKSWIDGET_H
#define MARKSWIDGET_H

#include <Wt/WTabWidget>
#include <Wt/WText>
#include "GoogleMap.h"
#include <WTableView>

#include "MarksModel.h"

using namespace Wt;

class MarksWidget : public WTabWidget
{
    WGoogleMap *marksMapWidget;
    WTableView *marksTable;
    MarksModel *marksModel;

public:
    MarksWidget(WContainerWidget *parent = 0);
    void updateModel();

};


#endif /* MARKSWIDGET_H */
