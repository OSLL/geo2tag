#include "sqp.h"

/*
 * APP THAT USES sqp MUST 'ADOPT' hParsedSQL POSSIBLY BEYOND sqpClose. THEN CALL HERE
 * TO FREE IT.
 *
 * IT IS ASSUMED THAT A FAILED PARSE DOES NOT MAKE IT THIS FAR UP THE SYNTAX
 * STRUCTURE... AND IS THUS FREED WITH sqpClose.
 *
 */
int sqpFreeParsedSQL( HSQPPARSEDSQL hParsedSQL )
{
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN: hParsedSQL = %p\n", __FILE__, __LINE__, hParsedSQL );
#endif

	if ( !hParsedSQL )
    {
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: nothing to do\n", __FILE__, __LINE__ );
#endif
		return false;
    }
	
	switch ( hParsedSQL->nType )
	{
	case sqpcreatetable:
		sqpFreeCreateTable( hParsedSQL->h.hCreateTable );
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: CREATE TABLE\n", __FILE__, __LINE__ );
#endif
		return true;

	case sqpdroptable:
		sqpFreeDropTable( hParsedSQL->h.hDropTable );
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: DROP TABLE\n", __FILE__, __LINE__ );
#endif
		return true;

	case sqpselect:
		sqpFreeSelect( hParsedSQL->h.hSelect );
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: SELECT\n", __FILE__, __LINE__ );
#endif
		return true;

	case sqpdelete:
		sqpFreeDelete( hParsedSQL->h.hDelete );
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: DELETE\n", __FILE__, __LINE__ );
#endif
		return true;

	case sqpinsert:
		sqpFreeInsert( hParsedSQL->h.hInsert );
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: INSERT\n", __FILE__, __LINE__ );
#endif
		return true;

	case sqpupdate:
		sqpFreeUpdate( hParsedSQL->h.hUpdate );
#ifdef SQPDEBUG
	    printf( "[SQP][%s][%d] END: UPDATE\n", __FILE__, __LINE__ );
#endif
		return true;

	default:
        printf( "[SQP][%s][%d] Unknown SQL statement type %d. Will continue but possible memory loss.\n", __FILE__, __LINE__, hParsedSQL->nType );
		break;
	}

    free( hParsedSQL );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

	return true;
}


