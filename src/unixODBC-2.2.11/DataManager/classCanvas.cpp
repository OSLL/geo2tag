/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include "classCanvas.h"

classCanvas::classCanvas( QWidget *pParent, const char *pszName )
	: QLabel( pParent, pszName )
{

}

classCanvas::~classCanvas()
{

}

void classCanvas::resizeEvent( QResizeEvent *p )
{
	changedSize( p->size().width(), p->size().height() );
}



