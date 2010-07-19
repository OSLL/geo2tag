#include "MarksWidget.h"

MarksWidget::MarksWidget(WContainerWidget *parent)
    : WTabWidget(parent)
{
    marksTable = new WTableView();
    marksTable->setMinimumSize(WLength(100), WLength(400));
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

    CHandlePtr<common::DataMarks> marks = marksModel->getMarks();
    marksMapWidget->clearOverlays();
    for (int i = 0; i < marks->size(); i++)
    {
        marksMapWidget->addMarker(WGoogleMap::Coordinate(marks->at(i)->getLatitude(),
                                                         marks->at(i)->getLongitude()),
                                  marks->at(i)->getDescription());
                                  //std::string("dfdf $1 dsfsd ."));
    }
}
