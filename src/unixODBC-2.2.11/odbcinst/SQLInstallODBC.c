/*************************************************
 * SQLInstallODBC
 *
 * Just provide stub for ODBC 2 installer functions
 *
 **************************************************/

#include <odbcinstext.h>

BOOL INSTAPI SQLInstallODBC(HWND       hwndParent,
                                      LPCSTR     lpszInfFile,
									  LPCSTR     lpszSrcPath,
									  LPCSTR     lpszDrivers)
{
	return FALSE;
}

BOOL INSTAPI SQLInstallDriver        (LPCSTR     lpszInfFile,
                                      LPCSTR     lpszDriver,
                                      LPSTR      lpszPath,
                                      WORD       cbPathMax,
                                      WORD 		* pcbPathOut)
{
	return FALSE;
}

BOOL INSTAPI SQLInstallTranslator( LPCSTR lpszInfFile,
								LPCSTR	pszTranslator,
								LPCSTR	pszPathIn,
								LPSTR	pszPathOut,
								WORD	nPathOutMax,
								WORD	*pnPathOut,
								WORD	nRequest,
								LPDWORD	pnUsageCount )

{
	return FALSE;
}

BOOL INSTAPI SQLRemoveDefaultDataSource( void ) {
	return SQLConfigDataSource (NULL, ODBC_REMOVE_DEFAULT_DSN, NULL, NULL);
}
