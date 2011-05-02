#ifndef MARKSWIDGET_H
#define MARKSWIDGET_H

#include <Wt/WTabWidget>
#include <Wt/WText>
#include "GoogleMap.h"
#include <Wt/WTableView>

#include "MarksModel.h"

using namespace Wt;

class MarksWidget : public WTabWidget
{
  QSharedPointer<User> m_user;
  WGoogleMap *marksMapWidget;
  WTableView *marksTable;
  MarksModel *marksModel;

  public:
    MarksWidget(QSharedPointer<User> user, WContainerWidget *parent = 0);
    void updateModel();
    void updateMap();

    };
#endif                                                      /* MARKSWIDGET_H */
