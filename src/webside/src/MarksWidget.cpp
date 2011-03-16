#include "MarksWidget.h"

MarksWidget::MarksWidget(QSharedPointer<User> user, WContainerWidget *parent)
    : WTabWidget(parent)
{
    m_user = user;
    marksTable = new WTableView();
    marksTable->setMinimumSize(WLength(100), WLength(400));
    marksMapWidget = new WGoogleMap();
    marksMapWidget->setMinimumSize(WLength(300), WLength(400));
    marksMapWidget->setMaximumSize(WLength(500), WLength(400));
    marksMapWidget->setCenter(Wt::WGoogleMap::Coordinate(60, 30));
    marksMapWidget->enableScrollWheelZoom();
    marksModel = new MarksModel(m_user, marksTable->parent());
    marksTable->setModel(marksModel);
    marksTable->setSelectable(true);
    
    /* Setting up tab widget */
    this->addTab(marksTable, "Table");
    this->addTab(marksMapWidget, "Map");

    updateMap();
}

void MarksWidget::updateModel()
{ 
    // marksModel->update();
    MarksModel *oldModel = marksModel;
    marksModel = new MarksModel(m_user, marksTable->parent());
    marksTable->setModel(marksModel);
    if (oldModel != 0)
        delete oldModel;
    updateMap();

}

void MarksWidget::updateMap()
{
    QList<QSharedPointer<DataMark> > marks = marksModel->getMarks();
    marksMapWidget->clearOverlays();
    for (int i = 0; i < marks.size(); i++)
    {
        marksMapWidget->addMarker(WGoogleMap::Coordinate(marks.at(i)->getLatitude(),
                                                         marks.at(i)->getLongitude()),
                                  marks.at(i)->getDescription().toStdString());
    }
}
