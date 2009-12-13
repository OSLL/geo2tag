/**************************************************
 * classCanvas
 **************************************************
 *
 * PURPOSE
 *
 * This is *THE* parent canvas for *ALL* property widgets. I
 * use a QLabel instead of a QVBoxLayout because I want to
 * stack many custom widgets on this (and hide/show them) and
 * I am concerned that QVBoxLayout will have a problem with this.
 * I did not even try QVBoxLayout because, in anycase, we need
 * a real widget (not a list item) to allow node processing
 * to create popup dialogs, such as the login (they need a
 * widget based parent).
 *
 ***************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey   - pharvey@codebydesign.com
 **************************************************/
#ifndef classCanvas_included
#define classCanvas_included

#include <qwidget.h>
#include <qlabel.h>

class classCanvas : public QLabel
{
    Q_OBJECT

public:
    classCanvas( QWidget *pParent = 0, const char *pszName = 0 );
    ~classCanvas() {}

  void resizeEvent( QResizeEvent * );

signals:
  void changedSize( int nW, int nH );

};
#endif

