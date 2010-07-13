#include "MarksWidget.h"

MarksWidget::MarksWidget(WContainerWidget *parent)
    : WTabWidget(parent)
{
    marksTableWidget = new WText("Here will be table with marks:");
    marksMapWidget = new WGoogleMap();
    marksMapWidget->setMinimumSize(WLength(400), WLength(300));
    marksMapWidget->setCenter(Wt::WGoogleMap::Coordinate(60, 30));
    marksMapWidget->enableScrollWheelZoom();
    
    /* Setting up tab widget */
    this->addTab(marksTableWidget, "Table");
    this->addTab(marksMapWidget, "Map");
}
