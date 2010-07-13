#include "OptionsWidget.h"

#include <WHBoxLayout>
#include <WVBoxLayout>
#include <WContainerWidget>

OptionsWidget::OptionsWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    optionsText = new WText("Options of the server:", parent);
    optionsTable = new WTableView(parent);
    optionsModel = new OptionsModel("/opt/geo2tag/geo2tag.conf", parent);
    optionsTable->setModel(optionsModel);
    optionsTable->setMinimumSize(WLength(100), WLength(100));

    /* Setting up channels widget main container */
    WVBoxLayout *mainLayout = new WVBoxLayout();
    mainLayout->addWidget(optionsText);
    mainLayout->addWidget(optionsTable);
    this->setLayout(mainLayout);
}
