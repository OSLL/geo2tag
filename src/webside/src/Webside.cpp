#include "inc/Webside.h"

Webside::Webside(WContainerWidget *parent)
    :WStackedWidget(parent)
{
    loginWidget = new LoginWidget(this);
    loginWidget->loginSuccessful.connect(SLOT(this, Webside::onLoginSuccess));
}

void Webside::onLoginSuccess(std::string token)
{
    mainWidget = new Sketch(token, this);
    this->setCurrentWidget(mainWidget);
}
