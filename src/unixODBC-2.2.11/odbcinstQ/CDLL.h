/**************************************************
 * 
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign. 
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#ifndef CDLL_included
#define CDLL_included

#include <string.h>
#include <ltdl.h>

typedef void	(*HCBDPROC)( void * );
typedef lt_dlhandle HCBDDLL;

class CDLL
{

public:
    CDLL( char *szFileName );
    ~CDLL();

    int Symbol( HCBDPROC *hProc, char *szSymbol );
    char szError[501];

private:
	HCBDDLL	hDLL;

};
#endif 

