#include "MarksWidget.h"

MarksWidget::MarksWidget(WContainerWidget *parent)
    : WTabWidget(parent)
{
    marksTable = new WTableView();
    marksTable->setMinimumSize(WLength(100), WLength(100));
    marksMapWidget = new WGoogleMap();
    marksMapWidget->setMinimumSize(WLength(300), WLength(400));
    marksMapWidget->setMaximumSize(WLength(500), WLength(400));
    marksMapWidget->setCenter(Wt::WGoogleMap::Coordinate(60, 30));
    marksMapWidget->enableScrollWheelZoom();
    
//    /* Setting up tab widget */
    this->addTab(marksTable, "Table");
    this->addTab(marksMapWidget, "Map");

    updateModel();
}

void MarksWidget::updateModel()
{
    marksModel = new MarksModel(WString(""), marksTable->parent());
    marksTable->setModel(marksModel);
}
