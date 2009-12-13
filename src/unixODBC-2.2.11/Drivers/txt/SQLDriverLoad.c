/**********************************************************************
 * SQLDriverLoad 
 **********************************************************************
 *
 * This function is only called by the unixODBC Driver Manager. This
 * means that this driver will not work with other Driver Managers at this time.
 * 
 * This, non-standard, ODBC function is used because it may be safer and even
 * more portable than using _init().
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "driverextras.h"

void SQLDriverLoad()
{
}
