#ifndef WEBVIEWIMPL_H
#define WEBVIEWIMPL_H

#include <QtGui/QWidget>
#include "ui_form_WebView.h"

class WebViewImpl : public QWidget
{
    Q_OBJECT;
public:
    WebViewImpl(QWidget *parent);
    virtual ~WebViewImpl();

private:
    Ui::webView  ui;
};

#endif // WEBVIEWIMPL_H

