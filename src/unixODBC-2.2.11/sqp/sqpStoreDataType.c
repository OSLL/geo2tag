/****************************************
 *
 * modifications Lorne Sunley July 2004
 *
 * change to return SQL datatypes in new field nType in SQPDATATYPE
 *
 ****************************************/

#include "sqp.h"
#include <sqlext.h>

void sqpStoreDataType( char *pszType, int nPrecision, int nScale )
{
    HSQPDATATYPE    hDataType;
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN: %s ( %d, %d )\n", __FILE__, __LINE__, pszType, nPrecision, nScale );
#endif

    if ( g_hDataType )
    {
        printf( "[SQP][%s][%d] END: Warning; Possible memory leak.\n", __FILE__, __LINE__ );
        return;
    }

	hDataType = (HSQPDATATYPE)malloc( sizeof(SQPDATATYPE) );
	hDataType->pszType	    = (char*)strdup( pszType );
    hDataType->nPrecision   = nPrecision;
	hDataType->nScale       = nScale;
    hDataType->nType = sqpStringTypeToSQLTYPE(pszType);

    g_hDataType = hDataType;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}


/* dumb little routine to return SQL datatype from sqp parsed datatype */

short sqpStringTypeToSQLTYPE (char * pszType)

{


	if (strcmp( pszType, "CHAR") == 0)
		return SQL_CHAR;
	if (strcmp( pszType, "VARCHAR") == 0)
		return SQL_VARCHAR;
	if (strcmp( pszType, "LONG VARCHAR") == 0)
		return SQL_LONGVARCHAR;
	if (strcmp( pszType, "NUMERIC") == 0)
		return SQL_NUMERIC;
	if (strcmp( pszType, "DECIMAL") == 0)
		return SQL_DECIMAL;
	if (strcmp( pszType, "SMALLINT") == 0)
		return SQL_SMALLINT;
	if (strcmp( pszType, "INTEGER") == 0)
		return SQL_INTEGER;
	if (strcmp( pszType, "REAL") == 0)
		return SQL_REAL;
	if (strcmp( pszType, "FLOAT") == 0)
		return SQL_DOUBLE;
	if (strcmp( pszType, "DOUBLE PRECISION") == 0)
		return SQL_DOUBLE;
	if (strcmp( pszType, "BIT") == 0)
		return SQL_BIT;
	if (strcmp( pszType, "TINYINT") == 0)
		return SQL_TINYINT;
	if (strcmp( pszType, "BIGINT") == 0)
		return SQL_BIGINT;
	if (strcmp( pszType, "BINARY") == 0)
		return SQL_BINARY;
	if (strcmp( pszType, "VARBINARY") == 0)
		return SQL_VARBINARY;
	if (strcmp( pszType, "LONG VARBINARY") == 0)
		return SQL_LONGVARBINARY;
	if (strcmp( pszType, "DATE") == 0)
		return SQL_DATE;
	if (strcmp( pszType, "TIME") == 0)
		return SQL_TIME;
	if (strcmp( pszType, "TIMESTAMP") == 0)
		return SQL_TIMESTAMP;
	return SQL_CHAR;


}
