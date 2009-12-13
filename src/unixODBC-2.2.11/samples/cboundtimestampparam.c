#include <stdlib.h>
#include <string.h>
#if defined (WIN32)
# include <windows.h>
#endif

#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

#include "autotest.h"
#include "helper.h"

#ifndef WIN32
#define TRUE    1
#define FALSE   0
#endif

static SQLRETURN create_table(lpSERVERINFO,SQLHDBC *hdbc, char *table, char *columns);
static SQLRETURN create_rows(lpSERVERINFO,SQLHDBC *hdbc, char *table);
static SQLRETURN show_rows(lpSERVERINFO,SQLHDBC *hdbc, char *table);


SQLUINTEGER             array_row_counts;
SQLUINTEGER             array_selects;

 
/************************************************************************/
SWORD testboundparameters( lpSERVERINFO srv_info )
{
    SQLRETURN   	ret;
    char            timestamp[100];
    char            create[256];
    SQLHANDLE       henv, hdbc;
    
    /*
     *  Connect to the data source.
     */

	if(srv_info->szValidServer0[0])
	{
        if (SQLAllocEnv(&henv) != SQL_SUCCESS) {
            do_a_error( srv_info,  SQL_HANDLE_ENV, henv, "SQLAllocHandle");
            goto ErrorRet;
        }

        if (SQLAllocConnect(henv, &hdbc) != SQL_SUCCESS) {
            do_a_error( srv_info,  SQL_HANDLE_ENV, henv, "SQLAllocHandle");
            SQLFreeEnv(henv);
            goto ErrorRet;
        }

        ret = SQLConnect( hdbc, (SQLCHAR*)srv_info -> szValidServer0, SQL_NTS,
                (SQLCHAR*)srv_info -> szValidLogin0, SQL_NTS,
                (SQLCHAR*)srv_info -> szValidPassword0, SQL_NTS );

        if ( ret == SQL_SUCCESS_WITH_INFO )
        {
            do_a_error( srv_info,  SQL_HANDLE_DBC, hdbc, "SQLConnect");
        }
        else if ( ret != SQL_SUCCESS )
        {
            do_a_error( srv_info,  SQL_HANDLE_DBC, hdbc, "SQLConnect");
            SQLFreeConnect(hdbc);
            SQLFreeEnv(henv);
            goto ErrorRet;
        }
    }
	else 
	{
		henv = srv_info->henv;
		hdbc = srv_info->hdbc;
	}

    if (!SQL_SUCCEEDED(
        do_get_dbtype(srv_info, hdbc, SQL_TIMESTAMP, timestamp, sizeof(timestamp)))) {
        SQLFreeConnect(hdbc);
        SQLFreeEnv(henv);
        goto ErrorRet;
    }
    sprintf(create,
            "a integer, ts %s, f float, d char(50), t char(50), ff float", timestamp);
    
    if (!SQL_SUCCEEDED(ret = create_table(srv_info, hdbc,"bindtype", create))) {
        SQLFreeConnect(hdbc);
        SQLFreeEnv(henv);
        goto ErrorRet;
    }
    
    create_rows(srv_info, hdbc, "bindtype");
    
    show_rows(srv_info, hdbc, "bindtype");
    
    SQLDisconnect(hdbc);
    SQLFreeConnect(hdbc);
    SQLFreeEnv(henv);

    return 0;

ErrorRet:
	/* a failure in an ODBC function that prevents completion of the        */
	/* test - for example, connect to the server */

	szLogPrintf( srv_info, 0, "\t\t *** Unrecoverable Quick Test FAILURE ***");
	srv_info->cErrors = -1;

    return 0;
}


/************************************************************************/
/*                                                                      */
/*  create_table                                                        */
/*  ===============                                                     */
/*                                                                      */
/*  Delete the named table and create a new one.                        */
/*                                                                      */
/************************************************************************/
static SQLRETURN create_table(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    char *table,
    char *columns)
{
    SQLHSTMT    hstmt;                          /* statement handle */
    SQLRETURN   ret;                            /* function status return */
    char        qbuf[1024];                     /* query buffer */
    
    szLogPrintf( srv_info, FALSE,"---------- create_table ----------\n");
    /*
     *  Allocate a statement and delete existing table.
     */
    if ((ret = SQLAllocStmt(hdbc, &hstmt)) != SQL_SUCCESS) {
        do_a_error( srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocStmt");
        return SQL_ERROR;
    }
    sprintf(qbuf, "drop table %s", table);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    
    if (!SQL_SUCCEEDED(SQLExecDirect(hstmt, (SQLCHAR *)qbuf, SQL_NTS))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");
    }
    sprintf(qbuf, "create table %s (%s)", table, columns);

    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    if (!SQL_SUCCEEDED(ret = SQLExecDirect(hstmt, (SQLCHAR *)qbuf, SQL_NTS))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");
        return ret;
    }
    
    if (!SQL_SUCCEEDED(ret = SQLFreeStmt(hstmt, SQL_DROP)))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    
    return ret;
}

/************************************************************************/
/*                                                                      */
/*  create_rows                                                         */
/*                                                                      */
/************************************************************************/
static SQLRETURN create_rows(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    char *table)

{
    SQLUSMALLINT        param_status[2];           /* parameter status */
    SQLUINTEGER         params_processed[2];       /* parameters processed */
    SQLINTEGER          p1[2];
    SQL_TIMESTAMP_STRUCT p2[2];
    float               p3[2];
    SQL_DATE_STRUCT     p4[2];
    SQL_TIME_STRUCT     p5[2];
    double              p6[2];
    SQLLEN              ip1[2];
    SQLLEN              ip2[2];
    SQLLEN              ip3[2];
    SQLLEN              ip4[2];
    SQLLEN              ip5[2];
    SQLLEN              ip6[2];     
    SQLLEN              row_counts;             /* rows affected */
    SQLRETURN   	ret;                    /* function status return */
    unsigned int        row;                    /* current row */
    unsigned int        i;                      /* loop variable */
    SQLHSTMT            hstmt;                  /* statement handle */
    char                qbuf[1024];             /* query buffer */
    
    szLogPrintf( srv_info, FALSE,"---------- create_rows ----------\n");

    if ((ret = SQLAllocStmt(hdbc, &hstmt)) != SQL_SUCCESS) {
        do_a_error( srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocStmt");
        return ret;
    }
    /*
     *  Delete all the rows.
     */
    sprintf(qbuf, "delete from %s", table);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    ret = SQLExecDirect(hstmt, (SQLCHAR *)qbuf, SQL_NTS);
    /*  SQL_NO_DATA returned when no rows deleted */
    if ((ret != SQL_SUCCESS) && (ret != SQL_NO_DATA))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");

    /*
     *  Set the parameter binding type.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting bind by row\n");
    if (!SQL_SUCCEEDED(SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_BIND_TYPE,
                                      (SQLPOINTER)SQL_BIND_BY_COLUMN, 0))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(BIND_BY_COLUMN)");
        return SQL_ERROR;
    }
    
    /*
     *  Set the parameter status array and parameters processed.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting Parameter Status Array Ptr\n");

    if (!SQL_SUCCEEDED(
        ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_STATUS_PTR,
                             &param_status[0], 0)))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(PARAM_STATUS)");

    szLogPrintf( srv_info, FALSE,"\tSetting Parameters Processed Ptr\n");
    params_processed[0] = params_processed[1] = 255;
    
    if (!SQL_SUCCEEDED(
        ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR,
                             &params_processed[0], 0)))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(PARAMS_PROCESSED)");

    /*
     *  Create the rows.
     */
    sprintf(qbuf, "insert into %s values(?,?,?,?,?,?)", table);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    if ((ret = SQLPrepare(hstmt, (SQLCHAR *)qbuf, SQL_NTS)) != SQL_SUCCESS) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLPrepare");
        return SQL_ERROR;
    }

    /*
     *  Now look at what the driver thinks the parameters are.
     */
    (void) do_describe_params(srv_info, hstmt, 6);
    
    /*
     *  Bind Parameters
     */
    if (!SQL_SUCCEEDED(
        ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
                               SQL_INTEGER, 0, 0, &p1[0],
                               sizeof(p1[0]), &ip1[0]))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter4");
        return ret;
    }
    if (!SQL_SUCCEEDED(
        ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP,
                               SQL_TYPE_TIMESTAMP,
                               23, 0, &p2[0],
                               sizeof(p2[0]), &ip2[0]))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter4");
        return ret;
    }
    if (!SQL_SUCCEEDED(
        ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT,
                               SQL_FLOAT, 0, 0, &p3[0],
                               sizeof(p3[0]), &ip3[0]))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter4");
        return ret;
    }
    if (!SQL_SUCCEEDED(
        ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_DATE,
                               SQL_CHAR,
                               50, 0, &p4[0],
                               sizeof(p4[0]), &ip4[0]))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter4");
        return ret;
    }
    if (!SQL_SUCCEEDED(
        ret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_TIME,
                               SQL_CHAR,
                               50, 0, &p5[0],
                               sizeof(p5[0]), &ip5[0]))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter4");
        return ret;
    }
    if (!SQL_SUCCEEDED(
        ret = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_DOUBLE,
                               SQL_DOUBLE,
                               sizeof(p6[0]), 0, &p6[0],
                               sizeof(p6[0]), &ip6[0]))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLBindParameter4");
        return ret;
    }

    if (!SQL_SUCCEEDED(
        ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE,
                             (SQLPOINTER)2, 0))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(SQL_ATTR_PARAMSET_SIZE");
        return ret;
    }

    szLogPrintf( srv_info, FALSE,"\tInserting rows into table\n");
    for (row = 0; row < 2; row++)
    {
        p1[0] =  row;
        p2[0].year = 1992;
        p2[0].month = 12;
        p2[0].day = 31;
        p2[0].hour = 23; 
        p2[0].minute = 45; 
        p2[0].second = 55;
	p2[0].fraction = 500000000;
        p3[0] = 1.4142135 * (row + 1);
        p4[0].year = 1992;
        p4[0].month = 12;
        p4[0].day = 31;
        p5[0].hour = 23;
        p5[0].minute = 45;
        p5[0].second = 55;
        p6[0] = 1.23456789 * (row + 1);
        
        ip1[0] = sizeof(p1[0]);
        ip2[0] = sizeof(p2[0]);
        ip3[0] = sizeof(p3[0]);        
        ip4[0] = sizeof(p4[0]);
        ip5[0] = sizeof(p5[0]);
        ip6[0] = sizeof(p6[0]);        
        
        p1[1] = row;
        p2[1].year = 1966;
        p2[1].month = 11;
        p2[1].day = 9;
        p2[1].hour = 1; 
        p2[1].minute = 2; 
        p2[1].second = 3;
	p2[1].fraction = 500000000;
        p3[1] = 3.14 * (row + 1);
        p4[1].year = 1966;
        p4[1].month = 11;
        p4[1].day = 9;
        p5[1].hour = 1;
        p5[1].minute = 2;
        p5[1].second = 3;
        p6[1] = 9.87654321 * (row + 1);
        
        ip1[1] = sizeof(p1[1]);
        ip2[1] = sizeof(p2[1]);
        ip3[1] = sizeof(p3[1]);        
        ip4[1] = sizeof(p4[1]);
        ip5[1] = sizeof(p5[1]);
        ip6[1] = sizeof(p6[1]); 
        
        param_status[0] = param_status[1] = 0xff;
        if (!SQL_SUCCEEDED(ret = SQLExecute(hstmt))) {
            if (ret == SQL_NEED_DATA) {
                 szLogPrintf( srv_info, FALSE, "SQLExecute=SQL_NEED_DATA\n");
                return SQL_ERROR;
            }
             szLogPrintf( srv_info, FALSE, "%d from SQLExecute\n", ret);
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecute");
            return SQL_ERROR;
        } else if (ret != SQL_SUCCESS) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecute");
        }
        if (!SQL_SUCCEEDED(SQLRowCount(hstmt, &row_counts))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLRowCount");
        }
        if (row_counts != 2) {
             szLogPrintf( srv_info, FALSE,
                    "** RowCount=%ld, expected %d **\n",
                    row_counts, 2);
        }
        
        for (i = 0; i < 2; i++) {
            if ((param_status[i] != SQL_PARAM_SUCCESS)) {
                 szLogPrintf( srv_info, FALSE, "** Row %u not executed, status=%u (%s)**\n",
                        i+1, param_status[i],
                        string_param_status(param_status[i]));
            }
        }
        if (params_processed[0] != 2) {
             szLogPrintf( srv_info, FALSE, "** Only %ld params processed, expected 1 **\n",
                    params_processed[0]);
        }
        szLogPrintf( srv_info, FALSE,".");
        fflush(stdout);
    }
    szLogPrintf( srv_info, FALSE,"\n");

    szLogPrintf( srv_info, FALSE,"\tResetting parameters\n");
    if (!SQL_SUCCEEDED(SQLFreeStmt(hstmt, SQL_RESET_PARAMS))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt(RESET_PARAMS)");
    }
    
    szLogPrintf( srv_info, FALSE,"\tClosing statement\n");
    if (!SQL_SUCCEEDED(SQLFreeStmt(hstmt, SQL_CLOSE))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt(SQL_CLOSE)");
    }
    
    szLogPrintf( srv_info, FALSE,"\tClearing Parameter Status Array Ptr\n");
    
    if (!SQL_SUCCEEDED(
        ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_STATUS_PTR, NULL, 0)))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(PARAM_STATUS)");

    szLogPrintf( srv_info, FALSE,"\tClearing Parameters Processed Ptr\n");
    
    if (!SQL_SUCCEEDED(
        ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, NULL, 0)))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(PARAMS_PROCESSED)");

    /*
     *  Set the parameter set size.
     */
    szLogPrintf( srv_info, FALSE,"\tSetting PARAMSETSIZE to 1\n");
    if (!SQL_SUCCEEDED(
        ret = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)1, 0)))
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLSetStmtAttr(PARAMSET_SIZE)");
    
    szLogPrintf( srv_info, FALSE,"\tDropping Statement\n");
    if (!SQL_SUCCEEDED(ret = SQLFreeStmt(hstmt, SQL_DROP))) 
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt(SQL_DROP)");
    return ret;
}

/************************************************************************/
/*                                                                      */
/*  show_rows                                                           */
/*                                                                      */
/************************************************************************/
static SQLRETURN show_rows(
    lpSERVERINFO srv_info,
    SQLHDBC *hdbc,
    char *table)

{
    SQLINTEGER          p1[2];
    SQL_TIMESTAMP_STRUCT p2[2];
    float               p3[2];
    SQL_DATE_STRUCT     p4[2];
    SQL_TIME_STRUCT     p5[2];        
    double              p6[2];
    SQLRETURN   	ret;                    /* function status return */
    SQLHSTMT            hstmt;                  /* statement handle */
    char                qbuf[1024];             /* query buffer */
    SQLLEN              ind;
    
    szLogPrintf( srv_info, FALSE,"---------- create_rows ----------\n");

    if ((ret = SQLAllocStmt(hdbc, &hstmt)) != SQL_SUCCESS) {
        do_a_error( srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocStmt");
        return ret;
    }

        
    szLogPrintf( srv_info, FALSE,"Retrieving rows for comparison\n");

    if (SQLAllocStmt(hdbc, &hstmt) != SQL_SUCCESS)
    {
        do_a_error( srv_info, SQL_HANDLE_DBC, hdbc, "SQLAllocHandle");
        return SQL_ERROR;
    }
    sprintf(qbuf, "select * from %s", table);
    szLogPrintf( srv_info, FALSE,"\"%s\"\n", qbuf);
    if ((ret =
         SQLPrepare(hstmt, (SQLCHAR *)qbuf, SQL_NTS)) != SQL_SUCCESS) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLPrepare");
        return ret;
    }
    if (!SQL_SUCCEEDED(ret = SQLExecute(hstmt))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecute");
        return ret;
    } else if (ret != SQL_SUCCESS) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLExecute");
    }
    memset(&p2[0], '\0', sizeof(p2[0]));
    memset(&p1[0], '\0', sizeof(p1[0]));
    memset(&p3[0], '\0', sizeof(p3[0]));
    memset(&p4[0], '\0', sizeof(p4[0]));    
    while(SQL_SUCCEEDED(SQLFetch(hstmt)))
    {
        if (!SQL_SUCCEEDED(
            SQLGetData(hstmt, 1, SQL_C_LONG,
                       &p1[0], sizeof(p1[0]), &ind))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            return SQL_ERROR;
        }
        if (!SQL_SUCCEEDED(
            SQLGetData(hstmt, 2, SQL_C_TIMESTAMP,
                       &p2[0], sizeof(p2[0]), &ind))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            return SQL_ERROR;
        }
        if (!SQL_SUCCEEDED(
            SQLGetData(hstmt, 3, SQL_C_FLOAT,
                       &p3[0], sizeof(p3[0]), &ind))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            return SQL_ERROR;
        }
        if (!SQL_SUCCEEDED(
            SQLGetData(hstmt, 4, SQL_C_DATE,
                       &p4[0], sizeof(p4[0]), &ind))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            return SQL_ERROR;
        }
        if (!SQL_SUCCEEDED(
            SQLGetData(hstmt, 5, SQL_C_TIME,
                       &p5[0], sizeof(p5[0]), &ind))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            return SQL_ERROR;
        }
        if (!SQL_SUCCEEDED(
            SQLGetData(hstmt, 6, SQL_C_DOUBLE,
                       &p6[0], sizeof(p6[0]), &ind))) {
            do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLGetData");
            return SQL_ERROR;
        }
       szLogPrintf(srv_info, FALSE, "%ld | "
                "%d:%u:%u-%u:%u:%u.%u |"
                "%f | %d:%u:%u | %u:%u:%u | %g\n",
                p1[0],
                p2[0].year, p2[0].month, p2[0].day, p2[0].hour,
                p2[0].minute, p2[0].second, p2[0].fraction,
                p3[0],
                p4[0].year, p4[0].month, p4[0].day,
                p5[0].hour, p5[0].minute, p5[0].second,
                p6[0]);
    }
    szLogPrintf( srv_info, FALSE,"\tDropping Statement\n");
    if (!SQL_SUCCEEDED(ret = SQLFreeStmt(hstmt, SQL_DROP))) {
        do_a_error( srv_info, SQL_HANDLE_STMT, hstmt, "SQLFreeStmt");
    }

    return ret;
}
