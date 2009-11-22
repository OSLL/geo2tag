#ifndef MAPVIEWIMPL_H
#define MAPVIEWIMPL_H

#include <QtGui/QWidget>
#include "ui_form_MapView.h"

class MapViewImpl : public QWidget
{
    Q_OBJECT;
public:
    MapViewImpl(QWidget *parent);
    virtual ~MapViewImpl();

private:
    Ui::mapView  ui;
};

#endif // MAPVIEWIMPL_H

