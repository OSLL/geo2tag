#ifndef SCRETCH_H
#define SCRETCH_H

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WStackedWidget>

#include "ChannelsWidget.h"
#include "MarksWidget.h"
#include "OptionsWidget.h"

using namespace Wt;

/*
 * A scketch for futher webside application.
 */
class Scetch : public WApplication
{
public:
  Scetch(const WEnvironment& env);

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

#endif /* SCRETCH_H */
