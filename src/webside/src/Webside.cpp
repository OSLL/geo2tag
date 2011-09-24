#include "Webside.h"
#include <syslog.h>
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
  Div *topWrapper;
  topWrapper= new Div(this, "top_wrapper");

  //  WText *topText;
  // topText= new WText(tr("top_wrapper"), this);
  //    WBreak *break1;
  //    break1= new WBreak(this);
  //    topWrapper->setId("top_wrapper");

  Div *bannerWrapper;
  bannerWrapper= new Div(this, "banner_wrapper");
  Div *bannerContent;
  bannerContent= new Div(bannerWrapper, "banner_content");
  Div *bannerEnd;
  bannerEnd= new Div(bannerContent, "banner_end");
  Div *banner;
  banner= new Div(bannerEnd, "banner");

  syslog(LOG_INFO,"test test: Webside::Webside(WContainerWidget *parent)");
  Div *mainWrapper;
  mainWrapper= new Div(this, "main_wrapper");
  Div *mainContent;
  mainContent= new Div(mainWrapper, "main_content");
  stackedWidget = new WStackedWidget(mainContent);
  stackedWidget->setId("main_page");
  loginWidget = new LoginWidget(stackedWidget);

  Div *footerWrapper;
  footerWrapper= new Div(this, "footer_wrapper");
  //  WText *footerWrapper = new WText(tr("footer_wrapper"), this);
  //    footerWrapper->setId("footer_wrapper");

  loginWidget->loginSuccessful.connect(this, &Webside::onLoginSuccess);
}


void Webside::onLoginSuccess(QSharedPointer<User> user)
{
  mainWidget = new Sketch(user, stackedWidget);
  stackedWidget->setCurrentWidget(mainWidget);
}
