#include "autotest.h"
#include "custom.h"

typedef SWORD (*TESTCASEFUNC)(lpSERVERINFO);

extern SWORD testboundparameters( lpSERVERINFO server_info );

struct {
	char *					name;			/* Test case name */
	char *					desc;			/* Test case description */
	TESTCASEFUNC		    lpTestFunc;		/* Pointer to function to implement test */
	} TestCases[] = {
/*  szName					szDesc						lpTestFunc      */
/*  --------------------	-----------------------	------------------------*/
    "Test Bound Parameters", "Test Bound Parameters", testboundparameters
	};

BOOL EXTFUN AutoTestName(LPSTR szName, UINT FAR * pcbTestCases)
{
    strcpy( szName, "Test Bound Parameters" );
	*pcbTestCases = NumItems(TestCases);

   return TRUE;
}

BOOL EXTFUN AutoTestDesc(UWORD iTest, LPSTR szName, LPSTR szDesc)
{
	/* Extra protection should AutoTestName return invalid pcbTestCases */
	if(iTest > NumItems(TestCases))
		return FALSE;

    strcpy( szName, TestCases[ iTest - 1 ].name );
    strcpy( szDesc, TestCases[ iTest - 1 ].desc );

    return TRUE;
}
 
void EXTFUN AutoTestFunc(lpSERVERINFO lpSrvr)
{
	int		    iDex;
	SWORD		cErrCnt;						    /* Count errors */
	char		szName[AUTO_MAX_TESTCASE_NAME+6];	/* Room for NULL and \r\n */


	/* Sets the error count to 0 */
	InitTest(lpSrvr);

	/* Loop through the count of test cases looking for set bits via GETBIT. */
	/*		When a bit is set, that test is to be run.  We've stored the */
	/*		function address which will implement the test, so simply call it. */
	for(iDex=1;  iDex<=NumItems(TestCases);  iDex++)
		if(GETBIT(lpSrvr->rglMask, iDex)) {
			/* Print out title of test */
            strcpy( szName, TestCases[ iDex - 1 ].name );
			szLogPrintf(lpSrvr, FALSE, "%s:\r\n", (LPSTR)szName);

			/* Call the test case and add errors */
			cErrCnt = 
				(*TestCases[(iDex-1)].lpTestFunc)(lpSrvr);
			if(cErrCnt != TEST_ABORTED)
				lpSrvr->cErrors += cErrCnt;
			else
				goto abort;
			}
	return;


	/* When a test must abort, the test case should call the AbortTest */
	/*		macro which sets lpSrvr->cErrors to TEST_ABORTED. */
abort:
	return;
}
