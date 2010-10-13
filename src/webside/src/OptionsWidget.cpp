#include "OptionsWidget.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WBreak>

OptionsWidget::OptionsWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    optionsText = new WText("Options of the server:", this);
    WBreak *break1 = new WBreak(this);
    optionsTable = new WTableView(this);
    optionsModel = new OptionsModel("/opt/geo2tag/geo2tag.conf", this);
    optionsTable->setModel(optionsModel);
    optionsTable->setMinimumSize(WLength(100), WLength(100));
    optionsTable->setSelectable(true);

//    /* Setting up channels widget main container */
//    WVBoxLayout *mainLayout = new WVBoxLayout();
//    mainLayout->addWidget(optionsText);
//    mainLayout->addWidget(optionsTable);
//    this->setLayout(mainLayout);
}
