#ifndef FEEDVIEWIMPL_H
#define FEEDVIEWIMPL_H

#include <QtGui/QWidget>
#include "ui_form_FeedView.h"

class FeedViewImpl : public QWidget
{
    Q_OBJECT;
public:
    FeedViewImpl(QWidget *parent);
    virtual ~FeedViewImpl();

private:
    Ui::feedView  ui;
};

#endif // FEEDVIEWIMPL_H

