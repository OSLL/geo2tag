#ifndef MARKSWIDGET_H
#define MARKSWIDGET_H

#include <Wt/WTabWidget>
#include <Wt/WText>
#include <Wt/WGoogleMap>

using namespace Wt;

class MarksWidget : public WTabWidget
{
    WText *marksTableWidget;
    WGoogleMap *marksMapWidget;

public:
    MarksWidget(WContainerWidget *parent = 0);

};


#endif /* MARKSWIDGET_H */
