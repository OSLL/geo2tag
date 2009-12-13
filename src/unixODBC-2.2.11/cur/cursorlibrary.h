#ifndef _CURSORLIBRARY_H
#define _CURSORLIBRARY_H

#define ODBCVER 0x0351

#include <sys/types.h>
#include <pwd.h>
#include <ltdl.h>
#include <string.h>

#include <sqlext.h>                     /* THIS WILL BRING IN sql.h and
                                           sqltypes.h AS WELL AS PROVIDE
                                           MS EXTENSIONS */

#define MAX_CURSOR_NAME         18

/*
 * cursor positions
 */

#define CL_BEFORE_START         -1
#define CL_AFTER_END            -2

#include "drivermanager.h"

typedef struct bound_column
{
    struct bound_column     *next;
    int                     column_number;
    SQLINTEGER              len_ind;
    SQLPOINTER              local_buffer;   /* buffer that the CL binds in */
                                            /* the driver (malloc'd) */
    SQLPOINTER              bound_buffer;   /* buffer that the app binds */
    SQLINTEGER              bound_type;     /* data type of binding */
    SQLINTEGER              bound_length;   /* length of binding */
    SQLINTEGER              *bound_ind;     /* ind ptr bound */
    int                     rs_buffer_offset;
    int                     rs_ind_offset;  /* offset onto the current rowset */
                                            /* buffer */
} CLBCOL;

typedef struct cl_connection
{
    struct driver_func *functions;      /* entry points, from the original */
                                        /* driver */
    SQLHANDLE           driver_dbc;     /* HDBC of the driver */
    DMHDBC              dm_connection;  /* driver manager connection str */ 
    DMHSTMT             cl_handle;      /* dummy to make the macro valid */
    SQLUSMALLINT        active_statement_allowed;
                                        /* can we have more than one active */
                                        /* statement */
    int                 error_count;    /* used to call SQLGetDiagRec */

    /*
     * Use these as entry points back to the driver manager
     */

    struct driver_helper_funcs  dh;
} *CLHDBC;

typedef struct cl_statement
{
    SQLHANDLE           driver_stmt;    /* driver sattement handle */
    CLHDBC              cl_connection;  /* parent cursor lib connection */
    DMHSTMT             dm_statement;   /* Driver manager statement */
    DMHSTMT             fetch_statement;   /* Driver manager statement */
    SQLUINTEGER         cursor_type;    /* statment attr's */
    SQLUINTEGER         concurrency;
    SQLPOINTER          *fetch_bookmark_ptr;
    SQLUINTEGER         *param_bind_offset_ptr;
    SQLUINTEGER         param_bind_type;
    SQLPOINTER          row_bind_offset_ptr;
    SQLUINTEGER         row_bind_type;
    SQLUINTEGER         rowset_array_size;
    SQLUINTEGER         simulate_cursor;
    SQLUINTEGER         use_bookmarks;
    SQLUINTEGER         *rows_fetched_ptr;
    SQLUSMALLINT        *row_status_ptr;
    SQLCHAR             cursor_name[ MAX_CURSOR_NAME + 1 ];
    CLBCOL              *bound_columns;
    int                 first_fetch_done;
    char                *sql_text;      /* text of current statement */
    char                **column_names; /* names of each column */
    SQLSMALLINT         *data_type;
    SQLINTEGER          *column_size;
    SQLSMALLINT         *decimal_digits;
    int                 driver_stmt_closed;
    int                 not_from_select;
    int                 read_only;
	int					fetch_done;
    /*
     * rowset info
     */
    int                 rowset_position;
    int                 rowset_count;
    int                 rowset_complete;
    FILE                *rowset_file;
    char                *rowset_buffer;
    int                 buffer_length;
    int                 column_count;
    int                 curr_rowset_start;
    int                 cursor_pos;
    int                 error_count;    /* used to call SQLGetDiagRec */
} *CLHSTMT;

/*
 * cursor lib function defs
 */

SQLRETURN SQL_API CLAllocHandle( SQLSMALLINT handle_type,
           SQLHANDLE input_handle,
           SQLHANDLE *output_handle,
           SQLHANDLE dm_handle );

SQLRETURN SQL_API CLAllocHandleStd(
           SQLSMALLINT        handle_type,
           SQLHANDLE          input_handle,
           SQLHANDLE          *output_handle,
           SQLHANDLE          dm_handle );

SQLRETURN SQL_API CLAllocStmt( SQLHDBC connection_handle,
           SQLHSTMT *statement_handle,
           SQLHANDLE dm_handle );

SQLRETURN SQL_API CLBindCol( SQLHSTMT statement_handle,
		   SQLUSMALLINT column_number,
           SQLSMALLINT target_type,
		   SQLPOINTER target_value,
           SQLINTEGER buffer_length,
	   	   SQLINTEGER *strlen_or_ind );

SQLRETURN SQL_API CLBindParam( SQLHSTMT statement_handle,
           SQLUSMALLINT parameter_number,
           SQLSMALLINT value_type,
           SQLSMALLINT parameter_type,
           SQLUINTEGER length_precision,
           SQLSMALLINT parameter_scale,
           SQLPOINTER parameter_value,
           SQLINTEGER *strlen_or_ind );

SQLRETURN SQL_API CLBindParameter(
           SQLHSTMT           statement_handle,
           SQLUSMALLINT       ipar,
           SQLSMALLINT        f_param_type,
           SQLSMALLINT        f_c_type,
           SQLSMALLINT        f_sql_type,
           SQLUINTEGER        cb_col_def,
           SQLSMALLINT        ib_scale,
           SQLPOINTER         rgb_value,
           SQLINTEGER         cb_value_max,
           SQLINTEGER         *pcb_value );
       
SQLRETURN SQL_API CLBulkOperations(
           SQLHSTMT        statement_handle,
           SQLSMALLINT     operation );

SQLRETURN SQL_API CLCancel( SQLHSTMT statement_handle );

SQLRETURN SQL_API CLCloseCursor( SQLHSTMT statement_handle );

SQLRETURN SQL_API CLColAttribute ( SQLHSTMT statement_handle,
           SQLUSMALLINT column_number,
           SQLUSMALLINT field_identifier,
           SQLPOINTER character_attribute,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *string_length,
           SQLPOINTER numeric_attribute );

SQLRETURN SQL_API CLColAttributes( SQLHSTMT statement_handle,
           SQLUSMALLINT column_number,
           SQLUSMALLINT field_identifier,
           SQLPOINTER   character_attribute,
           SQLSMALLINT  buffer_length,
           SQLSMALLINT  *string_length,
           SQLINTEGER   *numeric_attribute );

SQLRETURN SQL_API CLColumnPrivileges(
           SQLHSTMT            statement_handle,
           SQLCHAR             *catalog_name,
           SQLSMALLINT         name_length1,
           SQLCHAR             *schema_name,
           SQLSMALLINT         name_length2,
           SQLCHAR             *table_name,
           SQLSMALLINT         name_length3,
           SQLCHAR             *column_name,
           SQLSMALLINT         name_length4 );

SQLRETURN SQL_API CLColumns( SQLHSTMT statement_handle,
           SQLCHAR *catalog_name,
           SQLSMALLINT name_length1,
           SQLCHAR *schema_name,
           SQLSMALLINT name_length2,
           SQLCHAR *table_name,
           SQLSMALLINT name_length3,
           SQLCHAR *column_name,
           SQLSMALLINT name_length4 );

SQLRETURN SQL_API CLCopyDesc( SQLHDESC source_desc_handle,
           SQLHDESC target_desc_handle );

SQLRETURN SQL_API CLDescribeCol( SQLHSTMT statement_handle,
           SQLUSMALLINT column_number,
           SQLCHAR *column_name,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *name_length,
           SQLSMALLINT *data_type,
           SQLUINTEGER *column_size,
           SQLSMALLINT *decimal_digits,
           SQLSMALLINT *nullable );

SQLRETURN SQL_API CLDescribeParam(
           SQLHSTMT           statement_handle,
           SQLUSMALLINT       ipar,
           SQLSMALLINT        *pf_sql_type,
           SQLUINTEGER        *pcb_param_def,
           SQLSMALLINT        *pib_scale,
           SQLSMALLINT        *pf_nullable );

SQLRETURN SQL_API CLDisconnect( SQLHDBC connection_handle );

SQLRETURN SQL_API CLEndTran( SQLSMALLINT handle_type,
        SQLHANDLE handle,
        SQLSMALLINT completion_type );

SQLRETURN SQL_API CLError( SQLHENV environment_handle,
           SQLHDBC connection_handle,
           SQLHSTMT statement_handle,
           SQLCHAR *sqlstate,
           SQLINTEGER *native_error,
           SQLCHAR *message_text,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *text_length );

SQLRETURN SQL_API CLExecDirect( SQLHSTMT statement_handle,
           SQLCHAR *statement_text,
           SQLINTEGER text_length );

SQLRETURN SQL_API CLExecute( SQLHSTMT statement_handle );

SQLRETURN SQL_API CLExtendedFetch(
           SQLHSTMT           statement_handle,
           SQLUSMALLINT       f_fetch_type,
           SQLINTEGER         irow,
           SQLUINTEGER        *pcrow,
           SQLUSMALLINT       *rgf_row_status );

SQLRETURN SQL_API CLFetch( SQLHSTMT statement_handle );

SQLRETURN SQL_API CLFetchScroll( SQLHSTMT statement_handle,
           SQLSMALLINT fetch_orientation,
           SQLINTEGER fetch_offset );

SQLRETURN SQL_API CLForeignKeys(
           SQLHSTMT           statement_handle,
           SQLCHAR            *szpk_catalog_name,
           SQLSMALLINT        cbpk_catalog_name,
           SQLCHAR            *szpk_schema_name,
           SQLSMALLINT        cbpk_schema_name,
           SQLCHAR            *szpk_table_name,
           SQLSMALLINT        cbpk_table_name,
           SQLCHAR            *szfk_catalog_name,
           SQLSMALLINT        cbfk_catalog_name,
           SQLCHAR            *szfk_schema_name,
           SQLSMALLINT        cbfk_schema_name,
           SQLCHAR            *szfk_table_name,
           SQLSMALLINT        cbfk_table_name );

SQLRETURN SQL_API CLFreeHandle( SQLSMALLINT handle_type,
           SQLHANDLE handle );

SQLRETURN SQL_API CLFreeStmt( SQLHSTMT statement_handle,
           SQLUSMALLINT option );

SQLRETURN SQL_API CLGetConnectOption( SQLHDBC connection_handle,
           SQLUSMALLINT option,
           SQLPOINTER value );

SQLRETURN SQL_API CLGetConnectAttr( SQLHDBC connection_handle,
           SQLINTEGER attribute,
           SQLPOINTER value,
           SQLINTEGER buffer_length,
           SQLINTEGER *string_length );

SQLRETURN SQL_API CLGetCursorName( SQLHSTMT statement_handle,
           SQLCHAR *cursor_name,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *name_length );

SQLRETURN SQL_API CLGetData( SQLHSTMT statement_handle,
           SQLUSMALLINT column_number,
           SQLSMALLINT target_type,
           SQLPOINTER target_value,
           SQLINTEGER buffer_length,
           SQLINTEGER *strlen_or_ind );

SQLRETURN SQL_API CLGetDescField( SQLHDESC descriptor_handle,
           SQLSMALLINT rec_number, 
           SQLSMALLINT field_identifier,
           SQLPOINTER value, 
           SQLINTEGER buffer_length,
           SQLINTEGER *string_length );

SQLRETURN SQL_API CLGetDescRec( SQLHDESC descriptor_handle,
           SQLSMALLINT rec_number, 
           SQLCHAR *name,
           SQLSMALLINT buffer_length, 
           SQLSMALLINT *string_length,
           SQLSMALLINT *type, 
           SQLSMALLINT *sub_type, 
           SQLINTEGER *length, 
           SQLSMALLINT *precision, 
           SQLSMALLINT *scale, 
           SQLSMALLINT *nullable );

SQLRETURN SQL_API CLGetDiagField( SQLSMALLINT handle_type,
           SQLHANDLE handle,
           SQLSMALLINT rec_number,
           SQLSMALLINT diag_identifier,
           SQLPOINTER diag_info_ptr,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *string_length_ptr );

SQLRETURN SQL_API CLGetDiagRec( SQLSMALLINT handle_type,
           SQLHANDLE   handle,
           SQLSMALLINT rec_number,
           SQLCHAR     *sqlstate,
           SQLINTEGER  *native,
           SQLCHAR     *message_text,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *text_length_ptr );

SQLRETURN SQL_API CLGetInfo( SQLHDBC connection_handle,
           SQLUSMALLINT info_type,
           SQLPOINTER info_value,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *string_length );

SQLRETURN SQL_API CLGetStmtAttr( SQLHSTMT statement_handle,
           SQLINTEGER attribute,
           SQLPOINTER value,
           SQLINTEGER buffer_length,
           SQLINTEGER *string_length );

SQLRETURN SQL_API CLGetStmtOption( SQLHSTMT statement_handle,
           SQLUSMALLINT option,
           SQLPOINTER value );

SQLRETURN SQL_API CLGetTypeInfo( SQLHSTMT statement_handle,
           SQLSMALLINT data_type );

SQLRETURN SQL_API CLMoreResults( SQLHSTMT statement_handle );

SQLRETURN SQL_API CLNativeSql(
           SQLHDBC            hdbc,
           SQLCHAR            *sz_sql_str_in,
           SQLINTEGER         cb_sql_str_in,
           SQLCHAR            *sz_sql_str,
           SQLINTEGER         cb_sql_str_max,
           SQLINTEGER         *pcb_sql_str );

SQLRETURN SQL_API CLNumParams(
           SQLHSTMT           statement_handle,
           SQLSMALLINT        *pcpar );

SQLRETURN SQL_API CLNumResultCols( SQLHSTMT statement_handle,
           SQLSMALLINT *column_count );

SQLRETURN SQL_API CLParamData( SQLHSTMT statement_handle,
           SQLPOINTER *value );

SQLRETURN SQL_API CLParamOptions(
           SQLHSTMT           statement_handle,
           SQLUINTEGER        crow,
           SQLUINTEGER        *pirow );

SQLRETURN SQL_API CLPrepare( SQLHSTMT statement_handle,
           SQLCHAR *statement_text,
           SQLINTEGER text_length );

SQLRETURN SQL_API CLPrimaryKeys(
           SQLHSTMT           statement_handle,
           SQLCHAR            *sz_catalog_name,
           SQLSMALLINT        cb_catalog_name,
           SQLCHAR            *sz_schema_name,
           SQLSMALLINT        cb_schema_name,
           SQLCHAR            *sz_table_name,
           SQLSMALLINT        cb_table_name );

SQLRETURN SQL_API CLProcedureColumns(
           SQLHSTMT           statement_handle,
           SQLCHAR            *sz_catalog_name,
           SQLSMALLINT        cb_catalog_name,
           SQLCHAR            *sz_schema_name,
           SQLSMALLINT        cb_schema_name,
           SQLCHAR            *sz_proc_name,
           SQLSMALLINT        cb_proc_name,
           SQLCHAR            *sz_column_name,
           SQLSMALLINT        cb_column_name );

SQLRETURN SQL_API CLProcedures(
           SQLHSTMT           statement_handle,
           SQLCHAR            *sz_catalog_name,
           SQLSMALLINT        cb_catalog_name,
           SQLCHAR            *sz_schema_name,
           SQLSMALLINT        cb_schema_name,
           SQLCHAR            *sz_proc_name,
           SQLSMALLINT        cb_proc_name );

SQLRETURN SQL_API CLPutData( SQLHSTMT statement_handle,
           SQLPOINTER data,
           SQLINTEGER strlen_or_ind );

SQLRETURN SQL_API CLRowCount( SQLHSTMT statement_handle,
           SQLINTEGER *rowcount );

SQLRETURN SQL_API CLSetConnectAttr( SQLHDBC connection_handle,
           SQLINTEGER attribute,
           SQLPOINTER value,
           SQLINTEGER string_length );

SQLRETURN SQL_API CLSetConnectOption( SQLHDBC connection_handle,
           SQLUSMALLINT option,
           SQLULEN value );

SQLRETURN SQL_API CLSetCursorName( SQLHSTMT statement_handle,
           SQLCHAR *cursor_name,
           SQLSMALLINT name_length );

SQLRETURN SQL_API CLSetDescField( SQLHDESC descriptor_handle,
           SQLSMALLINT rec_number, 
           SQLSMALLINT field_identifier,
           SQLPOINTER value, 
           SQLINTEGER buffer_length );

SQLRETURN SQL_API CLSetDescRec( SQLHDESC descriptor_handle,
           SQLSMALLINT rec_number, 
           SQLSMALLINT type,
           SQLSMALLINT subtype, 
           SQLINTEGER length,
           SQLSMALLINT precision, 
           SQLSMALLINT scale,
           SQLPOINTER data, 
           SQLINTEGER *string_length,
           SQLINTEGER *indicator );

SQLRETURN SQL_API CLSetParam( SQLHSTMT statement_handle,
           SQLUSMALLINT parameter_number,
           SQLSMALLINT value_type,
           SQLSMALLINT parameter_type,
           SQLUINTEGER length_precision,
           SQLSMALLINT parameter_scale,
           SQLPOINTER parameter_value,
           SQLINTEGER *strlen_or_ind );

SQLRETURN SQL_API CLSetPos(
           SQLHSTMT           statement_handle,
           SQLUSMALLINT       irow,
           SQLUSMALLINT       foption,
           SQLUSMALLINT       flock );

SQLRETURN SQL_API CLSetScrollOptions(
           SQLHSTMT           statement_handle,
           SQLUSMALLINT       f_concurrency,
           SQLINTEGER         crow_keyset,
           SQLUSMALLINT       crow_rowset );

SQLRETURN SQL_API CLSetStmtAttr( SQLHSTMT statement_handle,
           SQLINTEGER attribute,
           SQLPOINTER value,
           SQLINTEGER string_length );

SQLRETURN SQL_API CLSetStmtOption( SQLHSTMT statement_handle,
           SQLUSMALLINT option,
           SQLULEN value );

SQLRETURN SQL_API CLSpecialColumns( SQLHSTMT statement_handle,
           SQLUSMALLINT identifier_type,
           SQLCHAR *catalog_name,
           SQLSMALLINT name_length1,
           SQLCHAR *schema_name,
           SQLSMALLINT name_length2,
           SQLCHAR *table_name,
           SQLSMALLINT name_length3,
           SQLUSMALLINT scope,
           SQLUSMALLINT nullable );

SQLRETURN SQL_API CLStatistics( SQLHSTMT statement_handle,
           SQLCHAR *catalog_name,
           SQLSMALLINT name_length1,
           SQLCHAR *schema_name,
           SQLSMALLINT name_length2,
           SQLCHAR *table_name,
           SQLSMALLINT name_length3,
           SQLUSMALLINT unique,
           SQLUSMALLINT reserved );

SQLRETURN SQL_API CLTablePrivileges(
           SQLHSTMT           statement_handle,
           SQLCHAR            *sz_catalog_name,
           SQLSMALLINT        cb_catalog_name,
           SQLCHAR            *sz_schema_name,
           SQLSMALLINT        cb_schema_name,
           SQLCHAR            *sz_table_name,
           SQLSMALLINT        cb_table_name );

SQLRETURN SQL_API CLTables( SQLHSTMT statement_handle,
           SQLCHAR *catalog_name,
           SQLSMALLINT name_length1,
           SQLCHAR *schema_name,
           SQLSMALLINT name_length2,
           SQLCHAR *table_name,
           SQLSMALLINT name_length3,
           SQLCHAR *table_type,
           SQLSMALLINT name_length4 );

SQLRETURN SQL_API CLTransact( SQLHENV environment_handle,
           SQLHDBC connection_handle,
           SQLUSMALLINT completion_type );

/*
 * internal prototypes
 */

void free_rowset( CLHSTMT cl_statement );
int calculate_buffers( CLHSTMT cl_statement, int column_count );
int free_bound_columns( CLHSTMT cl_statement );
SQLRETURN do_fetch_scroll( CLHSTMT cl_statement,
            int fetch_orientation, 
            int fetch_offset,
            SQLUSMALLINT *row_status_ptr,
            SQLUINTEGER *rows_fetched_ptr );
#endif
