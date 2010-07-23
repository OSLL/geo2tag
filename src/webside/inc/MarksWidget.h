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
    std::string m_token;
    WGoogleMap *marksMapWidget;
    WTableView *marksTable;
    MarksModel *marksModel;

public:
    MarksWidget(const std::string &token, WContainerWidget *parent = 0);
    void updateModel();
    void updateMap();

};


#endif /* MARKSWIDGET_H */
