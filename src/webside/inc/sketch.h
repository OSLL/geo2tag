#ifndef SKETCH_H
#define SKETCH_H

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WStackedWidget>
#include <Wt/WContainerWidget>

#include "ChannelsWidget.h"
#include "MarksWidget.h"
#include "OptionsWidget.h"

using namespace Wt;

/*
 * A sketch for futher webside application.
 */
class Sketch : public WContainerWidget
{
public:
  Sketch(WContainerWidget *parent = 0);

private:
  WPushButton *privateButton;
  WPushButton *channelsButton;
  WPushButton *marksButton;
  WPushButton *optionsButton;
  WPushButton *adminButton;

  WContainerWidget *mainContainer;
  WContainerWidget *menuContainer;
  WStackedWidget *stackedWidget;

  ChannelsWidget *channelsWidget;
  MarksWidget *marksWidget;
  OptionsWidget *optionsWidget;

  /* slots */
  void onPrivateClicked();
  void onChannelsClicked();
  void onMarksClicked();
  void onOptionsClicked();
  void onAdminClicked();

};

#endif /* SKETCH_H */
