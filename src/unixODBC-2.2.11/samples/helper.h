/*
 *  $Id: helper.h,v 1.1.1.1 2001/10/17 16:40:32 lurcher Exp $
 *
 *  $Log: helper.h,v $
 *  Revision 1.1.1.1  2001/10/17 16:40:32  lurcher
 *
 *  First upload to SourceForge
 *
 *  Revision 1.1  2001/05/10 05:52:01  nick
 *
 *  Add helper.h
 *
 *  Revision 1.2  2001/04/18 15:58:06  martin
 *  Add string_param_status().
 *
 *  Revision 1.1  1999/02/12 15:16:49  martin
 *  Initial revision
 *
 */

#include "autotest.h"

typedef struct typeinfo
{
    char        local_name[100];
    char        type_name[100];
    SQLSMALLINT sql_data_type;
    SQLINTEGER  column_size;
    char        create_params[256];
}typeinfo_t;

SQLRETURN do_a_error( lpSERVERINFO srv_info, SQLSMALLINT type, SQLHANDLE handle, char *fn);
SQLRETURN do_create_rows(lpSERVERINFO srv_info, SQLHDBC *hdbc, char *table);
SQLRETURN do_table_info(lpSERVERINFO srv_info, SQLHDBC *hdbc);
SQLRETURN do_type_info(lpSERVERINFO srv_info, SQLHDBC *hdbc, typeinfo_t *typeinfo);
SQLRETURN do_get_dbtype(lpSERVERINFO srv_info, SQLHDBC *hdbc, SQLSMALLINT type, char *type_name,
                        size_t len);

SQLSMALLINT do_describe_params(lpSERVERINFO srv_info, SQLHSTMT *hstmt, SQLSMALLINT params);
SQLRETURN do_create_table(lpSERVERINFO srv_info, SQLHDBC *hdbc, char *name, unsigned long flags,
                          SQLINTEGER length, char *type, char *params);
SQLRETURN do_get_info(lpSERVERINFO srv_info, SQLHDBC *hdbc,
                      SQLUINTEGER *array_row_counts,
                      SQLUINTEGER *array_selects,
                      SQLUINTEGER *static_ca1,
                      SQLUINTEGER *static_ca2);
void string_row_status(SQLUSMALLINT status, char *string);
char *string_param_status(SQLUSMALLINT status);

#ifdef IN_HELPER_C
extern unsigned int debug;
#endif

#define CREATE_TABLE_BIGCOL 0x1


