#ifndef PREFWIDGET_H
#define PREFWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WText>

using namespace Wt;

class PrefWidget : public WContainerWidget
{
  std::string m_token;
  WPushButton *changeButton;
  WLineEdit *password1;
  WLineEdit *password2;
  WText *errorText;

  public:
    PrefWidget(const std::string token, WContainerWidget *parent = 0);

    /* signals */

    /* slots */
    void onChangeClicked();

    };
#endif                                                      /* PREFWIDGET_H */
