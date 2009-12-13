/*
 *  $Id: helper.c,v 1.2 2001/12/13 13:00:34 lurcher Exp $
 *
 *  $Log: helper.c,v $
 *  Revision 1.2  2001/12/13 13:00:34  lurcher
 *
 *  Remove most if not all warnings on 64 bit platforms
 *  Add support for new MS 3.52 64 bit changes
 *  Add override to disable the stopping of tracing
 *  Add MAX_ROWS support in postgres driver
 *
 *  Revision 1.1.1.1  2001/10/17 16:40:32  lurcher
 *
 *  First upload to SourceForge
 *
 *  Revision 1.2  2001/05/23 13:48:37  nick
 *
 *  Remove unwanted include
 *
 *  Revision 1.1  2001/05/04 15:32:54  nick
 *
 *  Some makefile tweeks
 *
 *  Revision 1.3  2001/04/18 15:58:27  martin
 *  Add string_param_status.
 *  Make sure strings passed to ODBC fns are SQLCHAR.
 *
 *  Revision 1.2  1999/05/19 12:06:10  martin
 *  Quote table name to avoid reserved words problems.
 *
 *  Revision 1.1  1999/02/12 15:16:07  martin
 *  Initial revision
 *
 */
#include <stdlib.h>
#include <string.h>
#if defined (WIN32)
# include <windows.h>
#endif

#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

#define IN_HELPER_C 1
#include "helper.h"

#define TABLE_ROWS 50
#define paramset_size 2
#define rowset_size 2

#ifndef FALSE
#define FALSE 0
#endif


/************************************************************************/
/*                                                                      */
/*  do_create_table                                                     */
/*  ===============                                                     */
/*                                                                      */
/*  Delete the named table and create a new one.                        */
/*                                                                      */
/************************************************************************/
SQLRETURN do_create_table(
    lpSERVERINFO srv_info, 
    SQLHDBC *hdbc,
    char *table_name,
    unsigned long flags,
    SQLINTEGER length,
    char *type,
    char *params)
{
    SQLHSTMT    hstmt;                          /* statement handle */
    SQLRETURN   ret;                            /* function status return */
    char        qbuf[1024];                     /* query buffer */
    
    szLogPrintf( srv_info, FALSE,"---------- do_create_table ----------\n");
    /*
     *  Allocate a statement and delete existing table.
     */
    if (SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt) != SQL_SUCCESS)
    {
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocHandle");
        return SQL_ERROR;
    }
    sprintf(qbuf, "drop table \"%s\"", table_name);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    if (!SQL_SUCCEEDED(SQLExecDirect(hstmt, (SQLCHAR *)qbuf, SQL_NTS)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");
    }
    if (flags & CREATE_TABLE_BIGCOL)
    {
        if (strstr(params, "length"))
        {
            sprintf(qbuf, "create table \"%s\" "
                    "(a INTEGER PRIMARY KEY, b %s(%ld))",
                    table_name, type, length);
        }
        else
        {
            sprintf(qbuf, "create table \"%s\" "
                    "(a INTEGER PRIMARY KEY, b %s)",
                    table_name, type);
        }
    }
    else
    {
        sprintf(qbuf, "create table \"%s\" "
                "(a INTEGER PRIMARY KEY, b CHARACTER VARYING(20))",
                table_name);
    }
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    ret = SQLExecDirect(hstmt, (SQLCHAR *)qbuf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");
    
    ret = SQLFreeStmt(hstmt, SQL_DROP);
    if (!SQL_SUCCEEDED(ret))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    }
    
    return ret;
}


/************************************************************************/
/*                                                                      */
/*  do_type_info                                                        */
/*                                                                      */
/************************************************************************/
SQLRETURN do_type_info(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    typeinfo_t *typeinfo)
{
    struct types_s
    {
        SQLSMALLINT       type;
        char              *name;
    };
    struct types_s        types[] =
    {
        {SQL_ALL_TYPES,"ALL"},          
        {SQL_CHAR,"CHAR"},
        {SQL_NUMERIC,"NUMERIC"},
        {SQL_DECIMAL,"DECIMAL"},
        {SQL_INTEGER,"INTEGER"},
        {SQL_SMALLINT,"SMALLINT"},
        {SQL_FLOAT,"FLOAT"},
        {SQL_REAL,"REAL"},
        {SQL_DOUBLE,"DOUBLE"},
        {SQL_DATETIME,"DATETIME"},
        {SQL_VARCHAR,"VARCHAR"},
        {SQL_TIME,"TIME"},
        {SQL_DATE,"DATE"},        
        {SQL_TIMESTAMP,"TIMESTAMP"},
        {SQL_LONGVARCHAR,"LONGVARCHAR"},
        {SQL_BINARY,"BINARY"},
        {SQL_VARBINARY,"VARBINARY"},
        {SQL_LONGVARBINARY,"LONGVARBINARY"},
        {SQL_BIGINT,"BIGINT"},
        {SQL_TINYINT,"TINYINT"},
        {SQL_BIT,"BIT"},
        {0,NULL}
    };
    SQLRETURN           ret;                    /* function status return */
    unsigned int        t_index;                /* type index */
    SQLHSTMT            hstmt;                  /* statement handle */
    unsigned int        varchar_found=0;        /* found SQL_VARCHAR */
    unsigned int        integer_found=0;        /* found SQL_INTEGER */
    SQLSMALLINT         columns;                /* no. of resultant columns */
    
    szLogPrintf( srv_info, FALSE,"---------- do_type_info ----------\n");
    /*
     *  Allocate a statement and delete existing table.
     */
    if (SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt) != SQL_SUCCESS)
    {
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocHandle");
        return SQL_ERROR;
    }
    szLogPrintf( srv_info, FALSE,"\tCollecting type information with SQLGetTypeInfo\n");
    t_index = 0;
    while(types[t_index].name)
    {
        char            local_name[50];
        char            type_name[50];        
        SQLSMALLINT     sql_data_type;
        SQLINTEGER      column_size;
        char            create_params[256];
        SQLSMALLINT     min_scale;
        SQLSMALLINT     max_scale;        
        SQLSMALLINT     fix_prec_scale;
        
        if (srv_info->fDebug) szLogPrintf( srv_info, FALSE,"%s:\n", types[t_index].name);
        ret = SQLGetTypeInfo(hstmt, types[t_index].type);
        if (!SQL_SUCCEEDED(ret))
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetTypeInfo");
            ret = SQLCloseCursor(hstmt);
            if (!SQL_SUCCEEDED(ret))
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLCloseCursor");
            t_index++;
            continue;
        }
        if (!SQL_SUCCEEDED(SQLNumResultCols(hstmt, &columns)))
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLNumResultCols");
            return SQL_ERROR;
        }
        if (columns < 19)
        {
             szLogPrintf( srv_info, FALSE,
                    "** Can't find right number of columns in the result **\n");
             szLogPrintf( srv_info, FALSE, "** Found %d columns\n", columns);
        }
        while(SQL_SUCCEEDED(ret = SQLFetch(hstmt)))
        {
            SQLLEN          ind;
            
            local_name[0] = '\0';
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 1, SQL_C_CHAR,
                                                type_name, sizeof(type_name),
                                                &ind)))
            
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 2, SQL_C_SHORT,
                                                &sql_data_type,
                                                sizeof(sql_data_type),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 3, SQL_C_LONG,
                                                &column_size,
                                                sizeof(column_size),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            /*
             *  We are specifically interested in column size as we can use
             *  this to choose a big data type for blob testing. However,
             *  driver can legitimately return SQL_NULL_DATA (which we will
             *  class as 0 length) and SQL_NO_TOTAL which sort of implies
             *  very big (no limit).
             */
            if (ind == SQL_NO_TOTAL) column_size = SQL_NO_TOTAL;
            if (ind == SQL_NULL_DATA) column_size = 0;
            create_params[0] = '\0';
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 6, SQL_C_CHAR,
                                                create_params,
                                                sizeof(create_params),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 11, SQL_C_SHORT,
                                                &fix_prec_scale,
                                                sizeof(fix_prec_scale),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 13, SQL_C_CHAR,
                                                local_name, sizeof(local_name),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 14, SQL_C_SHORT,
                                                &min_scale, sizeof(min_scale),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (ind == SQL_NULL_DATA) min_scale = -1;
            
            if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 15, SQL_C_SHORT,
                                                &max_scale, sizeof(max_scale),
                                                &ind)))
            {
                do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            }
            if (ind == SQL_NULL_DATA) max_scale = -1;
            if (srv_info -> fDebug)
            {
                if (column_size == SQL_NO_TOTAL)
                {
                    szLogPrintf( srv_info, FALSE,"%20s %20s %d oo %d %d %d (%16s)\n",
                           type_name, local_name, sql_data_type,
                           min_scale, max_scale, fix_prec_scale, create_params);
                }
                else
                {
                    szLogPrintf( srv_info, FALSE,"%20s %20s %d %ld %d %d %d (%16s)\n",
                           type_name, local_name, sql_data_type, column_size,
                           min_scale, max_scale, fix_prec_scale, create_params);
                }
            }
            if (types[t_index].type == SQL_VARCHAR) varchar_found = 1;
            if (types[t_index].type == SQL_VARCHAR) integer_found = 1;
            if (!strcmp(types[t_index].name, "ALL"))
            {
                if (typeinfo)
                {
                    strcpy(typeinfo->local_name, local_name);
                    strcpy(typeinfo->type_name, type_name);
                    strcpy(typeinfo->create_params, create_params);
                    typeinfo->sql_data_type = sql_data_type;
                    typeinfo->column_size = column_size;
                    typeinfo++;
                }
            }
        }
        if (ret != SQL_NO_DATA)
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFetch");
        }
        ret = SQLCloseCursor(hstmt);
        if (!SQL_SUCCEEDED(ret))
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLCloseCursor");
        t_index++;
    }
    if (srv_info->fDebug) szLogPrintf( srv_info, FALSE,"\n");

    ret = SQLFreeStmt(hstmt, SQL_DROP);
    if (!SQL_SUCCEEDED(ret))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    }
    if (!varchar_found || !integer_found)
    {
         szLogPrintf( srv_info, FALSE,
                "** Can't seem to locate the INTEGER and VARCHAR types **\n");
        return SQL_ERROR;
    }
    return ret;
    
}


/************************************************************************/
/*                                                                      */
/*  do_table_info                                                       */
/*  =============                                                       */
/*                                                                      */
/*  Display a list of tables in the database.                           */
/*                                                                      */
/************************************************************************/
SQLRETURN do_table_info(lpSERVERINFO srv_info, SQLHSTMT *hdbc)
{
    SQLSMALLINT         columns;
    SQLRETURN   	ret;
    SQLSMALLINT         col;
    SQLHSTMT            hstmt;
    
    szLogPrintf( srv_info, FALSE,"---------- do_table_info ----------\n");
    /*
     *  Allocate a statement and delete existing table.
     */
    if (SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt) != SQL_SUCCESS)
    {
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocHandle");
        return SQL_ERROR;
    }
    /*
     *  Find out what tables we have.
     */
    szLogPrintf( srv_info, FALSE,"\tFinding out what tables there are with SQLTables\n");
    if (!SQL_SUCCEEDED(SQLTables(hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0)))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLTables");

    if (!SQL_SUCCEEDED(SQLNumResultCols(hstmt, &columns)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLNumResultCols");
        return SQL_ERROR;
    }
    if (columns < 5)
    {
         szLogPrintf( srv_info, FALSE, "** Can't find enough columns in the table info **\n");
        return SQL_ERROR;
    }
    if (srv_info->fDebug) szLogPrintf( srv_info, FALSE,"\tColumns: %d\n", columns);
    
    szLogPrintf( srv_info, FALSE,"\tCollecting column information for Tables call\n");
    for (col = 1; col <= columns; col++)
    {
        SQLCHAR         column_name[100];
        SQLSMALLINT     column_name_len;
        SQLSMALLINT     data_type;
        SQLULEN         column_size;
        
        
        ret = SQLDescribeCol(hstmt, col, column_name, sizeof(column_name),
                             &column_name_len, &data_type, &column_size,
                             NULL, NULL);
        if (!SQL_SUCCEEDED(ret))
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLDescribeCol");
        if (column_name_len > sizeof(column_name))
            column_name[sizeof(column_name) - 1] = '\0';
        else
            column_name[column_name_len] = '\0';
        if (srv_info->fDebug) szLogPrintf( srv_info, FALSE,"\tname:%s type=%d size=%lu\n",
               column_name, data_type, column_size);
    }
    
    /*
     *  Fetch the names of the tables.
     */
    szLogPrintf( srv_info, FALSE,"\tFetching the names of the tables\n");
    while(SQL_SUCCEEDED(SQLFetch(hstmt)))
    {
        char            buf[1024];
        SQLLEN          slen;
        
        ret = SQLGetData(hstmt, 3, SQL_C_CHAR, buf, sizeof(buf), &slen);
        if (SQL_SUCCEEDED(ret))
        {
            buf[slen] = '\0';
            if (srv_info->fDebug) szLogPrintf( srv_info, FALSE,"%s ", buf);
        }
    }
    if (srv_info->fDebug) szLogPrintf( srv_info, FALSE,"\n");
    if (!SQL_SUCCEEDED(SQLCloseCursor(hstmt)))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLCloseCursor");

    ret = SQLFreeStmt(hstmt, SQL_DROP);
    if (!SQL_SUCCEEDED(ret))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLCloseCursor");
    }
    return ret;
}


/************************************************************************/
/*                                                                      */
/*  do_create_rows                                                      */
/*                                                                      */
/************************************************************************/
SQLRETURN do_create_rows(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    char *table)
{
    SQLLEN      	len_ind[2][paramset_size]; /* parameter lengths */
    SQLINTEGER  	p1[paramset_size];      /* first parameter array */
    SQLCHAR     	p2[paramset_size][30];  /* second parameter array */
    SQLUSMALLINT        param_status[paramset_size]; /* parameter status */
    SQLUINTEGER         params_processed;       /* parameters processed */
    SQLLEN          row_counts[paramset_size]; /* rows affected */
    SQLRETURN   	ret;                    /* function status return */
    unsigned int        row;                    /* current row */
    unsigned int        i;                      /* loop variable */
    SQLHSTMT            hstmt;                  /* statement handle */
    char                qbuf[1024];             /* query buffer */
    
    szLogPrintf( srv_info, FALSE,"---------- do_create_rows ----------\n");
    szLogPrintf( srv_info, FALSE,"-- Creating rows with column-wise bound parameters --\n");

    if (SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt) != SQL_SUCCESS)
    {
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocHandle");
        return SQL_ERROR;
    }
    /*
     *  Delete all the rows.
     */
    sprintf(qbuf, "delete from \"%s\"", table);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    ret = SQLExecDirect(hstmt, (SQLCHAR *)qbuf, SQL_NTS);
    if (ret != SQL_SUCCESS)
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");

    /*
     *  Set the parameter binding type.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting bind by column\n");
    
    if (!SQL_SUCCEEDED(SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_BIND_TYPE,
                                      SQL_PARAM_BIND_BY_COLUMN, 0)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");
        return SQL_ERROR;
    }
    /*
     *  Set the parameter status array and parameters processed.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting Parameter Status Array Ptr\n");

    ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_STATUS_PTR, &param_status[0], 0);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");

    szLogPrintf( srv_info, FALSE,"\tSetting Parameters Processed Ptr\n");

    ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR,
                         &params_processed, 0);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");

    /*
     *  Set the parameter set size.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting PARAMSETSIZE to %d\n", paramset_size);

    ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE,
                         (SQLPOINTER)paramset_size, 0);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");
    
    /*
     *  Create the rows.
     */
    sprintf(qbuf, "insert into \"%s\" (a,b) values(?,?)", table);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    if (SQLPrepare(hstmt, (SQLCHAR *)qbuf, SQL_NTS) != SQL_SUCCESS)
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLPrepare");
        return SQL_ERROR;
    }

    /*
     *  Now look at what the driver thinks the parameters are.
     */
    (void) do_describe_params(srv_info, hstmt, paramset_size);

    /*
     *  Bind Parameters
     */
    ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                           5, 0, p1, 0, len_ind[0]);
    if (!SQL_SUCCEEDED(ret))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter");
        return ret;
    }
    ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                           sizeof(p2[0]) - 1, 0, p2, sizeof(p2[0]),
                           len_ind[1]);
    if (!SQL_SUCCEEDED(ret))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter");
        return ret;
    }
    szLogPrintf( srv_info, FALSE,"\tInserting rows into table\n");
    for (row = 0; row < TABLE_ROWS; row++)
    {
        /* a */
        p1[0] = row;
        len_ind[0][0] = sizeof(p1[0]);
        p1[1] = TABLE_ROWS + row;
        len_ind[0][1] = sizeof(p1[1]);

        /* b */
        sprintf((char*)p2[0], "this is row %u", row);
        len_ind[1][0] = SQL_NTS;
        sprintf((char*)p2[1], "and this is row %u", TABLE_ROWS + row);
        len_ind[1][1] = SQL_NTS;
        
        memset(param_status, 0xff, sizeof(param_status));
        if (!SQL_SUCCEEDED(ret = SQLExecute(hstmt)))
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecute");
            return SQL_ERROR;
        }
        else if (ret != SQL_SUCCESS)
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecute");
        }
        if (!SQL_SUCCEEDED(SQLRowCount(hstmt, row_counts)))
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLRowCount");
        }
        if (row_counts[0] != paramset_size)
        {
             szLogPrintf( srv_info, FALSE, "** RowCount=%ld, expected %d **\n",
                    row_counts[0], paramset_size);
        }
        
        for (i = 0; i < paramset_size; i++)
        {
            if ((param_status[i] != SQL_PARAM_SUCCESS))
            {
                 szLogPrintf( srv_info, FALSE, "** Row %u not executed, status=%u**\n",
                        i+1, param_status[i]);
            }
        }
        if (params_processed != paramset_size)
        {
             szLogPrintf( srv_info, FALSE, "** Only %ld rows processed **\n",
                    params_processed);
        }
        szLogPrintf( srv_info, FALSE,".");
        fflush(stdout);
    }
    szLogPrintf( srv_info, FALSE,"\n");

    szLogPrintf( srv_info, FALSE,"\tResetting parameters\n");
    if (!SQL_SUCCEEDED(SQLFreeStmt(hstmt, SQL_RESET_PARAMS)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    }
    
    szLogPrintf( srv_info, FALSE,"\tClosing statement\n");
    if (!SQL_SUCCEEDED(SQLFreeStmt(hstmt, SQL_CLOSE)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    }
    
    szLogPrintf( srv_info, FALSE,"\tClearing Parameter Status Array Ptr\n");
    ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_STATUS_PTR, NULL, 0);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");

    szLogPrintf( srv_info, FALSE,"\tClearing Parameters Processed Ptr\n");
    ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, NULL, 0);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");

    /*
     *  Set the parameter set size.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting PARAMSETSIZE to 1\n");
    ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE,
                         (SQLPOINTER)1, 0);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr");
    
    szLogPrintf( srv_info, FALSE,"\tDropping Statement\n");
    ret = SQLFreeStmt(hstmt, SQL_DROP);
    if (!SQL_SUCCEEDED(ret))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    }
    
    return ret;
}


/************************************************************************/
/*                                                                      */
/*  do_describe_params                                                  */
/*                                                                      */
/************************************************************************/
SQLSMALLINT do_describe_params(
    lpSERVERINFO srv_info,
    SQLHSTMT *hstmt,
    SQLSMALLINT params)
{
    unsigned int        i;
    SQLRETURN           ret;
    SQLSMALLINT         data_type;
    SQLULEN             param_size;
    SQLSMALLINT         decimal_digits;
    SQLSMALLINT         nullable;
    SQLSMALLINT         parameters;
    
    szLogPrintf( srv_info, FALSE,"---------- do_describe_params ----------\n");
    /*
     *  Find out how many parameters the driver thinks we need.
     */
    if (!SQL_SUCCEEDED(SQLNumParams(hstmt, &parameters)))
    {
         szLogPrintf( srv_info, FALSE, "** SQLNumParams() failed **\n");
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLNumParams");
        return 0;
    }
    szLogPrintf( srv_info, FALSE,"\tDriver thinks we have %d parameters\n", parameters);
    if (parameters != params)
    {
         szLogPrintf( srv_info, FALSE,
                "** Inconsistent parameter counts expected %d, got %d **\n",
                params, parameters);
    }
    
    for (i = 1; i <= parameters; i++)
    {
        ret = SQLDescribeParam(hstmt, i, &data_type, &param_size,
                               &decimal_digits, &nullable);
        if (!SQL_SUCCEEDED(ret))
        {
            do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLDescribeParam");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"\tparam:%u type:%d size=%lu digits:%d nullable:%d\n",
                   i, data_type, param_size, decimal_digits, nullable);
        }
    }
    return parameters;
}


/************************************************************************/
/*                                                                      */
/*  do_a_error                                                          */
/*                                                                      */
/************************************************************************/
SQLRETURN do_a_error(
    lpSERVERINFO srv_info,
    SQLSMALLINT type,
    SQLHANDLE handle,
    char *fn)
{
    SQLRETURN           ret;
    SQLCHAR             state[7];
    SQLCHAR             text[1024];
    SQLSMALLINT         len;
    int                 i=0;
    SQLINTEGER          native;
    char                cbuf[2048];
    SQLINTEGER          ndiags;
    
    szLogPrintf( srv_info, FALSE,"** Error from %s **\n", fn);
    ret = SQLGetDiagField(type, handle, 0, SQL_DIAG_NUMBER, &ndiags, 0, NULL);
    szLogPrintf( srv_info, FALSE,"%ld diagnostics found\n", ndiags);
    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state,
                            &native, text, sizeof(text), &len);
        if (SQL_SUCCEEDED(ret))
        {
            sprintf(cbuf, "** error: %s:%d:%ld:%s **\n",
                    state, i, native, text);
             szLogPrintf( srv_info, FALSE, cbuf);
        }
    }
    while (SQL_SUCCEEDED(ret));
    return ret;
}
    

/************************************************************************/
/*                                                                      */
/*  do_get_info                                                         */
/*  ===========                                                         */
/*                                                                      */
/************************************************************************/
SQLRETURN do_get_info(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    SQLUINTEGER *array_row_counts,
    SQLUINTEGER *array_selects,
    SQLUINTEGER *static_ca1,
    SQLUINTEGER *static_ca2)
{
    SQLRETURN           ret;

    szLogPrintf( srv_info, FALSE,"---------- do_get_info ----------\n");
    ret = SQLGetInfo(hdbc, SQL_PARAM_ARRAY_ROW_COUNTS, array_row_counts, 0, NULL);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLGetInfo");
    if (*array_row_counts == SQL_PARC_BATCH)
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_ROW_COUNTS = SQL_PARC_BATCH\n");
    }
    else if (*array_row_counts == SQL_PARC_NO_BATCH)
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_ROW_COUNTS = SQL_PARC_BATCH\n");
    }
    else
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_ROW_COUNTS = unknown, %lu\n",
               *array_row_counts);
    }

    ret = SQLGetInfo(hdbc, SQL_PARAM_ARRAY_SELECTS, array_selects, 0, NULL);
    if (!SQL_SUCCEEDED(ret))
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLGetInfo");
    if (*array_selects == SQL_PAS_BATCH)
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_SELECTS = SQL_PAS_BATCH\n");
    }
    else if (*array_selects == SQL_PAS_NO_BATCH)
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_SELECTS = SQL_PAS_NO_BATCH\n");
    }
    else if (*array_selects == SQL_PAS_NO_SELECT)
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_SELECTS = SQL_PAS_NO_SELECT\n");
    }
    else
    {
        szLogPrintf( srv_info, FALSE,"Driver: SQL_PARAM_ARRAY_SELECTS = unknown, %lu\n",
               *array_selects);
    }

    if (static_ca1)
    {
        ret = SQLGetInfo(hdbc, SQL_STATIC_CURSOR_ATTRIBUTES1, static_ca1,
                         0, NULL);
        if (!SQL_SUCCEEDED(ret))
            do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLGetInfo");
        if (*static_ca1 & SQL_CA1_POS_POSITION)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says SQL_CA1_POS_POSITION"
                   "\nand so a static cursor can be positioned with SQLSetPos\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says !SQL_CA1_POS_POSITION"
                   "\nand so a static cursor can NOT be positioned with SQLSetPos\n");
        }
        if (*static_ca1 & SQL_CA1_POS_UPDATE)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says SQL_CA1_POS_UPDATE"
                   "\nand so a static cursor can be used to SQL_UPDATE with SQLSetPos\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says !SQL_CA1_POS_UPDATE"
                   "\nand so a static cursor can NOT be used to SQL_UPDATE with SQLSetPos\n");
        }
        if (*static_ca1 & SQL_CA1_POS_DELETE)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says SQL_CA1_POS_DELETE"
                   "\nand so a static cursor can be used to SQL_DELETE with SQLSetPos\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says !SQL_CA1_POS_DELETE"
                   "\nand so a static cursor can NOT be used to SQL_DELETE with SQLSetPos\n");
        }
        if (*static_ca1 & SQL_CA1_POS_REFRESH)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says SQL_CA1_POS_REFRESH"
                   "\nand so a static cursor can be used to SQL_REFRESH with SQLSetPos\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES1 says !SQL_CA1_POS_REFRESH"
                   "\nand so a static cursor can NOT be used to SQL_REFRESH with SQLSetPos\n");
        }
        
    }
    if (static_ca2)
    {
        ret = SQLGetInfo(hdbc, SQL_STATIC_CURSOR_ATTRIBUTES2, static_ca2,
                         0, NULL);
        if (!SQL_SUCCEEDED(ret))
            do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLGetInfo");
        if (*static_ca2 & SQL_CA2_SENSITIVITY_ADDITIONS)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES2 says SQL_CA2_SENSITIVITY_ADDITIONS"
                   "\nand so added rows are visible with a static cursor\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES2 says !SQL_CA2_SENSITIVITY_ADDITIONS"
                   "\nand so added rows NOT are visible with a static cursor\n");
        }
        if (*static_ca2 & SQL_CA2_SENSITIVITY_DELETIONS)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES2 says SQL_CA2_SENSITIVITY_DELETIONS"
                   "\nand so deleted rows are visible with a static cursor\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES2 says !SQL_CA2_SENSITIVITY_DELETIONS"
                   "\nand so deleted rows NOT are visible with a static cursor\n");
        }
        if (*static_ca2 & SQL_CA2_SENSITIVITY_UPDATES)
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES2 says SQL_CA2_SENSITIVITY_UPDATES"
                   "\nand so updated rows are visible with a static cursor\n");
        }
        else
        {
            szLogPrintf( srv_info, FALSE,"Driver: SQL_STATIC_CURSOR_ATTRIBUTES2 says !SQL_CA2_SENSITIVITY_UPDATES"
                   "\nand so updated rows NOT are visible with a static cursor\n");
        }
    }
    
    return ret;
    
}    

/************************************************************************/
/*                                                                      */
/*  do_get_dbtype                                                       */
/*  =============                                                       */
/*                                                                      */
/************************************************************************/
SQLRETURN do_get_dbtype(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    SQLSMALLINT type,
    char *type_name,
    size_t len)
{
    SQLRETURN           ret;
    SQLHSTMT            hstmt;
    SQLLEN              ind;
    
    /*
     *  Find out what type to use for a timestamp.
     */
    if (!SQL_SUCCEEDED(ret = SQLAllocStmt(hdbc, &hstmt)))
    {
        do_a_error(srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocStmt");
        return ret;
    }
    
    if (!SQL_SUCCEEDED(ret = SQLGetTypeInfo(hstmt, type)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetTypeInfo");
        SQLFreeStmt(hstmt, SQL_DROP);
        return ret;
    }
    if (!SQL_SUCCEEDED(ret = SQLFetch(hstmt)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLFetch");
        SQLFreeStmt(hstmt, SQL_DROP);
        return ret;
    }
    if (!SQL_SUCCEEDED(ret = SQLGetData(hstmt, 1, SQL_C_CHAR, type_name,
                                        len, &ind)))
    {
        do_a_error(srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
        SQLFreeStmt(hstmt, SQL_DROP);
        return ret;
    }
    return SQLFreeStmt(hstmt, SQL_DROP);
}


/************************************************************************/
/*                                                                      */
/*  string_row_status                                                   */
/*  =================                                                   */
/*                                                                      */
/************************************************************************/
void string_row_status(
    SQLUSMALLINT status,
    char *string)
{
    switch(status)
    {
      case SQL_ROW_SUCCESS:
        strcpy(string, "SQL_ROW_SUCCESS");
        break;
      case SQL_ROW_SUCCESS_WITH_INFO:
        strcpy(string, "SQL_ROW_SUCCESS_WITH_INFO");
        break;
      case SQL_ROW_ERROR:
        strcpy(string, "SQL_ROW_ERROR");
        break;
      case SQL_ROW_UPDATED:
        strcpy(string, "SQL_ROW_UPDATED");
        break;
      case SQL_ROW_DELETED:
        strcpy(string, "SQL_ROW_DELETED");
        break;
      case SQL_ROW_ADDED:
        strcpy(string, "SQL_ROW_ADDED");
        break;
      case SQL_ROW_NOROW:
        strcpy(string, "SQL_ROW_NOROW");
        break;
      default:
        sprintf(string, "%d", status);
        break;
    }
}


char *string_param_status(SQLUSMALLINT status)
{
    switch (status)
    {
      case SQL_PARAM_SUCCESS:
        return "SQL_PARAM_SUCCESS";
      case SQL_PARAM_SUCCESS_WITH_INFO:
        return "SQL_PARAM_SUCCESS_WITH_INFO";
      case SQL_PARAM_ERROR:
        return "SQL_PARAM_ERROR";
      case SQL_PARAM_UNUSED:
        return "SQL_PARAM_UNUSED";
      case SQL_PARAM_DIAG_UNAVAILABLE:
        return "SQL_PARAM_DIAG_UNAVAILABLE";
      default:
        return "UNKNOWN";
    }
}
