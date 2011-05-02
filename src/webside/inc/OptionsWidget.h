#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WTableView>

#include "OptionsModel.h"

using namespace Wt;

class OptionsWidget : public WContainerWidget
{
  WText *optionsText;
  WTableView *optionsTable;
  OptionsModel *optionsModel;

  public:
    OptionsWidget(WContainerWidget *parent = 0);

    };
#endif                                                      /* OPTIONSWIDGET_H */
