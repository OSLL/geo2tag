#include "Webside.h"

/* Shortcut for a <div id=""> */
class Div : public WContainerWidget
{
public:
  Div(WContainerWidget *parent, const std::string& id)
    : WContainerWidget(parent)
  {
    setId(id);
  }
};

Webside::Webside(WContainerWidget *parent)
    :WContainerWidget(parent)
{
    //Div *topWrapper = new Div(this, "top_wrapper");
    //Div *bannerWrapper = new Div(this, "banner_wrapper");
    Div *topWrapper = new Div(this, "top_wrapper");

    //WText *topText = new WText(tr("top_wrapper"), this);
    //WBreal *break1 = new WBreak(this);
    //topWrapper->setId("top_wrapper");

    Div *bannerWrapper = new Div(this, "banner_wrapper");
    Div *bannerContent = new Div(bannerWrapper, "banner_content");
    Div *bannerEnd = new Div(bannerContent, "banner_end");
    Div *banner = new Div(bannerEnd, "banner");


    Div *mainWrapper = new Div(this, "main_wrapper");
    Div *mainContent = new Div(mainWrapper, "main_content");
    stackedWidget = new WStackedWidget(mainContent);
    stackedWidget->setId("main_page");
    loginWidget = new LoginWidget(stackedWidget);

    Div *footerWrapper = new Div(this, "footer_wrapper");
    //WText *footerWrapper = new WText(tr("footer_wrapper"), this);
    //footerWrapper->setId("footer_wrapper");

    loginWidget->loginSuccessful.connect(this, &Webside::onLoginSuccess);
}

void Webside::onLoginSuccess(QSharedPointer<User> user)
{
    mainWidget = new Sketch(user, stackedWidget);
    stackedWidget->setCurrentWidget(mainWidget);
}
