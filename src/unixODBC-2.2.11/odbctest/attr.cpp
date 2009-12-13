/*********************************************************************
 *
 * Written by Nick Gorham
 * (nick@lurcher.org).
 *
 * copyright (c) 1999 Nick Gorham
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 **********************************************************************
 *
 * $Id: attr.cpp,v 1.4 2003/12/01 16:37:17 lurcher Exp $
 *
 * $Log: attr.cpp,v $
 * Revision 1.4  2003/12/01 16:37:17  lurcher
 *
 * Fix a bug in SQLWritePrivateProfileString
 *
 * Revision 1.3  2001/12/20 17:26:26  lurcher
 *
 * More warnings removed
 *
 * Revision 1.2  2001/12/20 12:30:44  lurcher
 *
 * Fix 64 problem in odbctest and 7.1 Postgres driver
 *
 * Revision 1.1.1.1  2001/10/17 16:40:30  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.4  2001/07/20 09:42:58  nick
 *
 * Replace char[] with QString to avoid buffer overrun
 *
 * Revision 1.3  2001/06/04 15:24:49  nick
 *
 * Add port to MAC OSX and QT3 changes
 *
 * Revision 1.2  2001/05/31 16:05:55  nick
 *
 * Fix problems with postgres closing local sockets
 * Make odbctest build with QT 3 (it doesn't work due to what I think are bugs
 * in QT 3)
 * Fix a couple of problems in the cursor lib
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.7  2000/06/16 14:09:26  ngorham
 *
 * Fix a couple of bugs in GetStmtOptions
 *
 * Revision 1.6  2000/06/13 12:30:19  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.5  2000/06/12 18:46:18  ngorham
 *
 * More changes
 *
 * Revision 1.4  2000/06/09 17:04:18  ngorham
 *
 * More, and More
 *
 * Revision 1.3  2000/06/05 16:53:14  ngorham
 *
 * Next lot of updates
 *
 * Revision 1.2  2001/05/31 10:26:26  ngorham
 *
 * Fix a few minor typo's
 *
 * Revision 1.1  2000/05/04 17:04:47  ngorham
 *
 * Initial commit
 *
 *
 **********************************************************************/

#include <stdlib.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <sql.h>
#include <sqlext.h>
#include "odbctest.h"
#include "attr.h"

static attr_value strlen_options[] = 
{
	{ "SQL_NTS", SQL_NTS },
	{ "SQL_IS_POINTER", SQL_IS_POINTER },
	{ "SQL_IS_UINTEGER", SQL_IS_UINTEGER },
	{ "SQL_IS_INTEGER", SQL_IS_INTEGER },
	{ "SQL_IS_USMALLINT", SQL_IS_USMALLINT },
	{ "SQL_IS_SMALLINT", SQL_IS_SMALLINT },
	{ "", 0 },
	{ NULL }
};

static attr_options stmt_options[] = 
{
	{ "SQL_ATTR_APP_PARAM_DESC", SQL_ATTR_APP_PARAM_DESC, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_APP_ROW_DESC", SQL_ATTR_APP_ROW_DESC, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ASYNC_ENABLE", SQL_ATTR_ASYNC_ENABLE, 
		{
			{ "SQL_ASYNC_ENABLE_OFF", SQL_ASYNC_ENABLE_OFF }, 
			{ "SQL_ASYNC_ENABLE_ON", SQL_ASYNC_ENABLE_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CONCURRENCY", SQL_ATTR_CONCURRENCY, 
		{
			{ "SQL_CONCUR_READ_ONLY", SQL_CONCUR_READ_ONLY }, 
			{ "SQL_CONCUR_LOCK", SQL_CONCUR_LOCK }, 
			{ "SQL_CONCUR_ROWVER", SQL_CONCUR_ROWVER }, 
			{ "SQL_CONCUR_VALUES", SQL_CONCUR_VALUES }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CURSOR_SCROLLABLE", SQL_ATTR_CURSOR_SCROLLABLE, 
		{
			{ "SQL_NONSCROLLABLE", SQL_NONSCROLLABLE }, 
			{ "SQL_SCROLLABLE", SQL_SCROLLABLE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CURSOR_SENSITIVITY", SQL_ATTR_CURSOR_SENSITIVITY, 
		{
			{ "SQL_UNSPECIFIED", SQL_UNSPECIFIED }, 
			{ "SQL_INSENSITIVE", SQL_INSENSITIVE }, 
			{ "SQL_SENSITIVE", SQL_SENSITIVE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CURSOR_TYPE", SQL_ATTR_CURSOR_TYPE, 
		{
			{ "SQL_CURSOR_FORWARD_ONLY", SQL_CURSOR_FORWARD_ONLY }, 
			{ "SQL_CURSOR_STATIC", SQL_CURSOR_STATIC }, 
			{ "SQL_CURSOR_KEYSET_DRIVEN", SQL_CURSOR_KEYSET_DRIVEN }, 
			{ "SQL_CURSOR_DYNAMIC", SQL_CURSOR_DYNAMIC }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ENABLE_AUTO_IPD", SQL_ATTR_ENABLE_AUTO_IPD, 
		{
			{ "SQL_FALSE", SQL_FALSE }, 
			{ "SQL_TRUE", SQL_TRUE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_FETCH_BOOKMARK_PTR", SQL_ATTR_FETCH_BOOKMARK_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER, FALSE, TRUE
	},
	{ "SQL_ATTR_FETCH_IMP_PARAM_DESC", SQL_ATTR_IMP_PARAM_DESC, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_FETCH_IMP_ROW_DESC", SQL_ATTR_IMP_ROW_DESC, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_KEYSET_SIZE", SQL_ATTR_KEYSET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_MAX_LENGTH", SQL_ATTR_MAX_LENGTH, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_MAX_ROWS", SQL_ATTR_MAX_ROWS, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_METADATA_ID", SQL_ATTR_METADATA_ID, 
		{
			{ "SQL_FALSE", SQL_FALSE }, 
			{ "SQL_TRUE", SQL_TRUE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_NOSCAN", SQL_ATTR_NOSCAN, 
		{
			{ "SQL_NOSCAN_OFF", SQL_NOSCAN_OFF }, 
			{ "SQL_NOSCAN_ON", SQL_NOSCAN_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_PARAM_BIND_OFFSET_PTR", SQL_ATTR_PARAM_BIND_OFFSET_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER, FALSE, TRUE
	},
	{ "SQL_ATTR_PARAM_BIND_TYPE", SQL_ATTR_PARAM_BIND_TYPE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_PARAM_OPERATION_PTR", SQL_ATTR_PARAM_OPERATION_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_SMALLINT, FALSE, TRUE
	},
	{ "SQL_ATTR_PARAM_STATUS_PTR", SQL_ATTR_PARAM_STATUS_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_SMALLINT, FALSE, TRUE
	},
	{ "SQL_ATTR_PARAMS_PROCESSED_PTR", SQL_ATTR_PARAMS_PROCESSED_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_SMALLINT, FALSE, TRUE
	},
	{ "SQL_ATTR_PARAMSET_SIZE", SQL_ATTR_PARAMSET_SIZE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_QUERY_TIMEOUT", SQL_ATTR_QUERY_TIMEOUT, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_RETRIEVE_DATA", SQL_ATTR_RETRIEVE_DATA, 
		{
			{ "SQL_RD_ON", SQL_RD_ON }, 
			{ "SQL_RD_OFF", SQL_RD_OFF }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ROW_ARRAY_SIZE", SQL_ATTR_ROW_ARRAY_SIZE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ROW_BIND_OFFSET_PTR", SQL_ATTR_ROW_BIND_OFFSET_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER, FALSE, TRUE
	},
	{ "SQL_ATTR_ROW_BIND_TYPE", SQL_ATTR_ROW_BIND_TYPE, 
		{
			{ "SQL_BIND_BY_COLUMN", SQL_BIND_BY_COLUMN }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ROW_NUMBER", SQL_ATTR_ROW_NUMBER, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ROW_OPERATION_PTR", SQL_ATTR_ROW_OPERATION_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_SMALLINT, FALSE, TRUE
	},
	{ "SQL_ATTR_ROW_STATUS_PTR", SQL_ATTR_ROW_STATUS_PTR,
		{
			{ NULL }
		}, "3.0", SQL_SMALLINT, FALSE, TRUE
	},
	{ "SQL_ATTR_ROWS_FETCHED_PTR", SQL_ATTR_ROWS_FETCHED_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_SIMULATE_CURSOR", SQL_ATTR_SIMULATE_CURSOR, 
		{
			{ "SQL_SC_NON_UNIQUE", SQL_SC_NON_UNIQUE }, 
			{ "SQL_SC_TRY_UNIQUE", SQL_SC_TRY_UNIQUE }, 
			{ "SQL_SC_UNIQUE", SQL_SC_UNIQUE }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_USE_BOOKMARKS", SQL_ATTR_USE_BOOKMARKS,
		{
			{ "SQL_UB_ON", SQL_UB_ON }, 
			{ "SQL_UB_OFF", SQL_UB_OFF }, 
			{ "SQL_UB_VARIABLE", SQL_UB_VARIABLE }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_options stmt_opt_options[] = 
{
	{ "SQL_ASYNC_ENABLE", SQL_ASYNC_ENABLE, 
		{
			{ "SQL_ASYNC_ENABLE_OFF", SQL_ASYNC_ENABLE_OFF }, 
			{ "SQL_ASYNC_ENABLE_ON", SQL_ASYNC_ENABLE_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_BIND_TYPE", SQL_BIND_TYPE, 
		{
			{ "SQL_BIND_BY_COLUMN", SQL_BIND_BY_COLUMN }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_CONCURRENCY", SQL_CONCURRENCY, 
		{
			{ "SQL_CONCUR_READ_ONLY", SQL_CONCUR_READ_ONLY }, 
			{ "SQL_CONCUR_LOCK", SQL_CONCUR_LOCK }, 
			{ "SQL_CONCUR_ROWVER", SQL_CONCUR_ROWVER }, 
			{ "SQL_CONCUR_VALUES", SQL_CONCUR_VALUES }, 
			{ "SQL_CONCUR_READ_ONLY", SQL_CONCUR_READ_ONLY }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_CURSOR_TYPE", SQL_CURSOR_TYPE, 
		{
			{ "SQL_CURSOR_FORWARD_ONLY", SQL_CURSOR_FORWARD_ONLY }, 
			{ "SQL_CURSOR_STATIC", SQL_CURSOR_STATIC }, 
			{ "SQL_CURSOR_KEYSET_DRIVEN", SQL_CURSOR_KEYSET_DRIVEN }, 
			{ "SQL_CURSOR_DYNAMIC", SQL_CURSOR_DYNAMIC }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_KEYSET_SIZE", SQL_KEYSET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_MAX_LENGTH", SQL_MAX_LENGTH, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_MAX_ROWS", SQL_MAX_ROWS, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_NOSCAN", SQL_NOSCAN, 
		{
			{ "SQL_NOSCAN_OFF", SQL_NOSCAN_OFF }, 
			{ "SQL_NOSCAN_ON", SQL_NOSCAN_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_QUERY_TIMEOUT", SQL_QUERY_TIMEOUT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_RETRIEVE_DATA", SQL_RETRIEVE_DATA, 
		{
			{ "SQL_RD_ON", SQL_RD_ON }, 
			{ "SQL_RD_OFF", SQL_RD_OFF }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ROWSET_SIZE", SQL_ROWSET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_SIMULATE_CURSOR", SQL_SIMULATE_CURSOR, 
		{
			{ "SQL_SC_NON_UNIQUE", SQL_SC_NON_UNIQUE }, 
			{ "SQL_SC_TRY_UNIQUE", SQL_SC_TRY_UNIQUE }, 
			{ "SQL_SC_UNIQUE", SQL_SC_UNIQUE }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_USE_BOOKMARKS", SQL_USE_BOOKMARKS, 
		{
			{ "SQL_UB_ON", SQL_UB_ON }, 
			{ "SQL_UB_OFF", SQL_UB_OFF }, 
			{ "SQL_UB_VARIABLE", SQL_UB_VARIABLE }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_options conn_options[] = 
{
	{ "SQL_ATTR_ACCESS_MODE", SQL_ATTR_ACCESS_MODE, 
		{
			{ "SQL_MODE_READ_WRITE", SQL_MODE_READ_WRITE }, 
			{ "SQL_MODE_READ_ONLY", SQL_MODE_READ_ONLY }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ASYNC_ENABLE", SQL_ATTR_ASYNC_ENABLE, 
		{
			{ "SQL_ASYNC_ENABLE_OFF", SQL_ASYNC_ENABLE_OFF }, 
			{ "SQL_ASYNC_ENABLE_ON", SQL_ASYNC_ENABLE_ON }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_AUTO_IPD", SQL_ATTR_AUTO_IPD, 
		{
			{ "SQL_TRUE", SQL_TRUE }, 
			{ "SQL_FALSE", SQL_FALSE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_AUTOCOMMIT", SQL_ATTR_AUTOCOMMIT, 
		{
			{ "SQL_AUTOCOMMIT_ON", SQL_AUTOCOMMIT_ON }, 
			{ "SQL_AUTOCOMMIT_OFF", SQL_AUTOCOMMIT_OFF }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CONNECTION_TIMEOUT", SQL_ATTR_CONNECTION_TIMEOUT, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CURRENT_CATALOG", SQL_ATTR_CURRENT_CATALOG, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_ATTR_LOGIN_TIMEOUT", SQL_ATTR_LOGIN_TIMEOUT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_METADATA_ID", SQL_ATTR_METADATA_ID, 
		{
			{ "SQL_TRUE", SQL_TRUE }, 
			{ "SQL_FALSE", SQL_FALSE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_ODBC_CURSORS", SQL_ATTR_ODBC_CURSORS, 
		{
			{ "SQL_CUR_USE_IF_NEEDED", SQL_CUR_USE_IF_NEEDED }, 
			{ "SQL_CUR_USE_ODBC", SQL_CUR_USE_ODBC }, 
			{ "SQL_CUR_USE_DRIVER", SQL_CUR_USE_DRIVER }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_PACKET_SIZE", SQL_ATTR_PACKET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_QUIET_MODE", SQL_ATTR_QUIET_MODE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_ATTR_TRACE", SQL_ATTR_TRACE, 
		{
			{ "SQL_OPT_TRACE_OFF", SQL_OPT_TRACE_OFF }, 
			{ "SQL_OPT_TRACE_ON", SQL_OPT_TRACE_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_TRACEFILE", SQL_ATTR_TRACEFILE, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_ATTR_TRANSLATE_LIB", SQL_ATTR_TRANSLATE_LIB, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_ATTR_TRANSLATE_OPTION", SQL_ATTR_TRANSLATE_OPTION, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ATTR_TXN_ISOLATION", SQL_ATTR_TXN_ISOLATION, 
		{
			{ "SQL_TXN_READ_UNCOMMITTED", SQL_TXN_READ_UNCOMMITTED },
			{ "SQL_TXN_READ_COMMITTED", SQL_TXN_READ_COMMITTED },
			{ "SQL_TXN_REPEATABLE_READ", SQL_TXN_REPEATABLE_READ },
			{ "SQL_TXN_SERIALIZABLE", SQL_TXN_SERIALIZABLE },
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_options conn_opt_options[] = 
{
	{ "conn: SQL_ACCESS_MODE", SQL_ACCESS_MODE, 
		{
			{ "SQL_MODE_READ_ONLY", SQL_MODE_READ_ONLY }, 
			{ "SQL_MODE_READ_WRITE", SQL_MODE_READ_WRITE }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "conn: SQL_AUTOCOMMIT", SQL_AUTOCOMMIT, 
		{
			{ "SQL_AUTOCOMMIT_ON", SQL_AUTOCOMMIT_ON }, 
			{ "SQL_AUTOCOMMIT_OFF", SQL_AUTOCOMMIT_OFF }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "conn: SQL_CURRENT_QUALIFIER", SQL_CURRENT_QUALIFIER, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "conn: SQL_LOGIN_TIMEOUT", SQL_LOGIN_TIMEOUT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "conn: SQL_ODBC_CURSORS", SQL_ODBC_CURSORS, 
		{
			{ "SQL_CUR_USE_IF_NEEDED", SQL_CUR_USE_IF_NEEDED }, 
			{ "SQL_CUR_USE_ODBC", SQL_CUR_USE_ODBC }, 
			{ "SQL_CUR_USE_DRIVER", SQL_CUR_USE_DRIVER }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "conn: SQL_OPT_TRACE", SQL_OPT_TRACE, 
		{
			{ "SQL_OPT_TRACE_ON", SQL_OPT_TRACE_ON }, 
			{ "SQL_OPT_TRACE_OFF", SQL_OPT_TRACE_OFF }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "conn: SQL_OPT_TRACEFILE", SQL_OPT_TRACEFILE, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "conn: SQL_PACKET_SIZE", SQL_PACKET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "conn: SQL_QUIET_MODE", SQL_QUIET_MODE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "conn: SQL_TRANSLATE_DLL", SQL_TRANSLATE_DLL, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "conn: SQL_TRANSLATE_OPTION", SQL_TRANSLATE_OPTION, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "conn: SQL_TXN_ISOLATION", SQL_TXN_ISOLATION, 
		{
			{ "SQL_TXN_READ_UNCOMMITED", SQL_TXN_READ_UNCOMMITTED }, 
			{ "SQL_TXN_READ_COMMITED", SQL_TXN_READ_COMMITTED }, 
			{ "SQL_TXN_REPEATABLE_READ", SQL_TXN_REPEATABLE_READ }, 
			{ "SQL_TXN_SERIALIZABLE", SQL_TXN_SERIALIZABLE }, 
			{ "SQL_TXN_VERSIONING", 0x00000010L }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_ASYNC_ENABLE", SQL_ASYNC_ENABLE, 
		{
			{ "SQL_ASYNC_ENABLE_OFF", SQL_ASYNC_ENABLE_OFF }, 
			{ "SQL_ASYNC_ENABLE_ON", SQL_ASYNC_ENABLE_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_BIND_TYPE", SQL_BIND_TYPE, 
		{
			{ "SQL_BIND_BY_COLUMN", SQL_BIND_BY_COLUMN }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_CONCURRENCY", SQL_CONCURRENCY, 
		{
			{ "SQL_CONCUR_READ_ONLY", SQL_CONCUR_READ_ONLY }, 
			{ "SQL_CONCUR_LOCK", SQL_CONCUR_LOCK }, 
			{ "SQL_CONCUR_ROWVER", SQL_CONCUR_ROWVER }, 
			{ "SQL_CONCUR_VALUES", SQL_CONCUR_VALUES }, 
			{ "SQL_CONCUR_READ_ONLY", SQL_CONCUR_READ_ONLY }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "stmt: SQL_CURSOR_TYPE", SQL_CURSOR_TYPE, 
		{
			{ "SQL_CURSOR_FORWARD_ONLY", SQL_CURSOR_FORWARD_ONLY }, 
			{ "SQL_CURSOR_STATIC", SQL_CURSOR_STATIC }, 
			{ "SQL_CURSOR_KEYSET_DRIVEN", SQL_CURSOR_KEYSET_DRIVEN }, 
			{ "SQL_CURSOR_DYNAMIC", SQL_CURSOR_DYNAMIC }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "stmt: SQL_KEYSET_SIZE", SQL_KEYSET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "stmt: SQL_MAX_LENGTH", SQL_MAX_LENGTH, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_MAX_ROWS", SQL_MAX_ROWS, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_NOSCAN", SQL_NOSCAN, 
		{
			{ "SQL_NOSCAN_OFF", SQL_NOSCAN_OFF }, 
			{ "SQL_NOSCAN_ON", SQL_NOSCAN_ON }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_QUERY_TIMEOUT", SQL_QUERY_TIMEOUT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "stmt: SQL_RETRIEVE_DATA", SQL_RETRIEVE_DATA, 
		{
			{ "SQL_RD_ON", SQL_RD_ON }, 
			{ "SQL_RD_OFF", SQL_RD_OFF }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "stmt: SQL_ROWSET_SIZE", SQL_ROWSET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "stmt: SQL_SIMULATE_CURSOR", SQL_SIMULATE_CURSOR, 
		{
			{ "SQL_SC_NON_UNIQUE", SQL_SC_NON_UNIQUE }, 
			{ "SQL_SC_TRY_UNIQUE", SQL_SC_TRY_UNIQUE }, 
			{ "SQL_SC_UNIQUE", SQL_SC_UNIQUE }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "stmt: SQL_USE_BOOKMARKS", SQL_USE_BOOKMARKS, 
		{
			{ "SQL_UB_ON", SQL_UB_ON }, 
			{ "SQL_UB_OFF", SQL_UB_OFF }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_options conn_gopt_options[] = 
{
	{ "SQL_ACCESS_MODE", SQL_ACCESS_MODE, 
		{
			{ "SQL_MODE_READ_ONLY", SQL_MODE_READ_ONLY }, 
			{ "SQL_MODE_READ_WRITE", SQL_MODE_READ_WRITE }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_AUTOCOMMIT", SQL_AUTOCOMMIT, 
		{
			{ "SQL_AUTOCOMMIT_ON", SQL_AUTOCOMMIT_ON }, 
			{ "SQL_AUTOCOMMIT_OFF", SQL_AUTOCOMMIT_OFF }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_CURRENT_QUALIFIER", SQL_CURRENT_QUALIFIER, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_LOGIN_TIMEOUT", SQL_LOGIN_TIMEOUT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_ODBC_CURSORS", SQL_ODBC_CURSORS, 
		{
			{ "SQL_CUR_USE_IF_NEEDED", SQL_CUR_USE_IF_NEEDED }, 
			{ "SQL_CUR_USE_ODBC", SQL_CUR_USE_ODBC }, 
			{ "SQL_CUR_USE_DRIVER", SQL_CUR_USE_DRIVER }, 
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_OPT_TRACE", SQL_OPT_TRACE, 
		{
			{ "SQL_OPT_TRACE_ON", SQL_OPT_TRACE_ON }, 
			{ "SQL_OPT_TRACE_OFF", SQL_OPT_TRACE_OFF }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_OPT_TRACEFILE", SQL_OPT_TRACEFILE, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_PACKET_SIZE", SQL_PACKET_SIZE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_QUIET_MODE", SQL_QUIET_MODE, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_TRANSLATE_DLL", SQL_TRANSLATE_DLL, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_TRANSLATE_OPTION", SQL_TRANSLATE_OPTION, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_TXN_ISOLATION", SQL_TXN_ISOLATION, 
		{
			{ "SQL_TXN_READ_UNCOMMITED", SQL_TXN_READ_UNCOMMITTED }, 
			{ "SQL_TXN_READ_COMMITED", SQL_TXN_READ_COMMITTED }, 
			{ "SQL_TXN_REPEATABLE_READ", SQL_TXN_REPEATABLE_READ }, 
			{ "SQL_TXN_SERIALIZABLE", SQL_TXN_SERIALIZABLE }, 
			{ "SQL_TXN_VERSIONING", 0x00000010L }, 
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_options env_options[] = 
{
	{ "SQL_ATTR_ODBC_VERSION", SQL_ATTR_ODBC_VERSION, 
		{
			{ "SQL_OV_ODBC2", SQL_OV_ODBC2 }, 
			{ "SQL_OV_ODBC3", SQL_OV_ODBC3 }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CP_MATCH", SQL_ATTR_CP_MATCH,
		{
			{ "SQL_CP_STRICT_MATCH", SQL_CP_STRICT_MATCH },
			{ "SQL_CP_RELAXED_MATCH", SQL_CP_RELAXED_MATCH },
			{ "SQL_CP_MATCH_DEFAULT", SQL_CP_MATCH_DEFAULT },
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_CONNECTION_POOLING", SQL_ATTR_CONNECTION_POOLING, 
		{
			{ "SQL_CP_OFF", SQL_OV_ODBC2 }, 
			{ "SQL_CP_ONE_PER_DRIVER", SQL_CP_ONE_PER_DRIVER }, 
			{ "SQL_CP_ONE_PER_HENV", SQL_CP_ONE_PER_HENV }, 
			{ "SQL_CP_DEFAULT", SQL_CP_DEFAULT }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_ATTR_OUTPUT_NTS", SQL_ATTR_OUTPUT_NTS, 
		{
			{ "SQL_TRUE", SQL_TRUE }, 
			{ "SQL_FALSE", SQL_FALSE }, 
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ NULL 
	}
};

void dSetEnvAttr::Activated( int index )
{
	value -> clear();

	odbctest->fill_list_box( env_options[ index ].values, value );
}

void dSetEnvAttr::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HENV;
	SQLINTEGER attribute;
	SQLPOINTER vptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_ENV, handles );
	int i;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetEnvAttr():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Environment Handle: %p", in_handle );
	else
		txt.sprintf( "    Environment Handle: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = env_options[ types -> currentItem() ].attr;
	txt.sprintf( "    Attribute: %s=%d", 
		env_options[ types -> currentItem() ].text,
		env_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	vptr = (SQLPOINTER) env_options[ types -> currentItem() ].values[ value -> currentItem() ].value;
	txt.sprintf( "    Value: %s=%d", 
		env_options[ types -> currentItem() ].values[ value -> currentItem() ].text,
		env_options[ types -> currentItem() ].values[ value -> currentItem() ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	string_length = strlen_options[ stringlen -> currentItem() ].value;
	txt.sprintf( "    String Length: %s=%d", 
		strlen_options[ stringlen -> currentItem() ].text,
		strlen_options[ stringlen -> currentItem() ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetEnvAttr( in_handle, attribute, vptr, string_length );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dSetEnvAttr::dSetEnvAttr( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Environment Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( env_options, types );

	value = new QComboBox( FALSE, this, "Value" );
	value -> setGeometry( 170, 110, 300, 20 );
	parent->fill_list_box( env_options[ 0 ].values, value );

	stringlen = new QComboBox( FALSE, this, "String Length" );
	stringlen -> setGeometry( 170, 140, 300, 20 );
	parent->fill_list_box( strlen_options, stringlen );

	l_handle = new QLabel( "Environment Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "Attribute:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	l_value = new QLabel( "Value:", this );
	l_value -> setGeometry( 10, 110, 130, 20 );

	l_slen = new QLabel( "String Length:", this );
	l_slen -> setGeometry( 10, 140, 130, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( types, SIGNAL(activated(int)), this, SLOT( Activated(int)));
}

dSetEnvAttr::~dSetEnvAttr()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete value;
	delete stringlen;
	delete l_handle;
	delete l_types;
	delete l_value;
	delete l_slen;
}

void dGetEnvAttr::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HENV;
	SQLINTEGER attribute;
	SQLPOINTER vptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_ENV, handles );
	SQLINTEGER b_len, strlen_or_ind;
	SQLINTEGER *strlen_ptr;
	char *buf = NULL;
	int i;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetEnvAttr():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Environment Handle: %p", in_handle );
	else
		txt.sprintf( "    Environment Handle: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = env_options[ types -> currentItem() ].attr;
	txt.sprintf( "    Attribute: %s=%d", 
		env_options[ types -> currentItem() ].text,
		env_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( target_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = new char[ 300 ];	
	}
	else
	{
		buf = new char[ b_len ];	
	}

	if ( buf )
	{
		txt.sprintf( "    Value Ptr: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Value Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    Buffer Length: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	strlen_or_ind = -999999;

	if ( strlen_valid -> isOn())
	{
		strlen_ptr = NULL;
	}
	else
	{
		strlen_ptr = &strlen_or_ind;
	}

	if ( strlen_ptr )
	{
		txt.sprintf( "    Strlen Ptr: %p", strlen_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Strlen Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetEnvAttr( in_handle, attribute, 
			buf, b_len, strlen_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( strlen_ptr )
	{	
		if ( strlen_or_ind == -999999 )
		{
			txt.sprintf( "    *Strlen Ptr: <unchanged>" );
		}
		else
		{
			txt.sprintf( "    *Strlen Ptr: %d", strlen_or_ind );
		}
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( SQL_SUCCEEDED( ret ))
	{
		int index = types -> currentItem();
		switch( env_options[ index ].data_type )
		{
			case SQL_INTEGER:
				SQLUINTEGER ival;
				memcpy( &ival, buf, sizeof( ival ));
				txt.sprintf( "    *ValuePtr = %d (0x%08X)", ival, ival );
				odbctest -> out_win -> insertLineLimited( txt );

				if ( env_options[ index ].values[ 0 ].text )
				{
					if ( env_options[ index ].is_bitmap )
					{
						int i;

						for ( i = 0; env_options[ index ].values[ i ].text; i ++ )
						{
							if ( env_options[ index ].values[ i ].value & ival )
							{
								txt.sprintf( "        %s", 
									env_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
					else
					{
						int i;

						for ( i = 0; env_options[ index ].values[ i ].text; i ++ )
						{
							if ( env_options[ index ].values[ i ].value == ival )
							{
								txt.sprintf( "        %s", 
									env_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
				}
				break;

			case SQL_SMALLINT:
				SQLSMALLINT sval;
				memcpy( &sval, buf, sizeof( sval ));
				txt.sprintf( "    *ValuePtr = %d (0x%04X)", sval, sval );
				odbctest -> out_win -> insertLineLimited( txt );

				if ( env_options[ index ].values[ 0 ].text )
				{
					if ( env_options[ index ].is_bitmap )
					{
						int i;

						for ( i = 0; env_options[ index ].values[ i ].text; i ++ )
						{
							if ( env_options[ index ].values[ i ].value & sval )
							{
								txt.sprintf( "        %s", 
									env_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
					else
					{
						int i;

						for ( i = 0; env_options[ index ].values[ i ].text; i ++ )
						{
							if ( env_options[ index ].values[ i ].value == sval )
							{
								txt.sprintf( "        %s", 
									env_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
				}
				break;
		}		
	}

	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

void dGetEnvAttr::target_clkd()
{
	if ( target_valid -> isOn() )
	    target_valid -> setText( "ValuePtr: SQL_NULL_POINTER" );
	else
	    target_valid -> setText( "ValuePtr: VALID" );
}

void dGetEnvAttr::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StrLen_Ptr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StrLen_Ptr: VALID" );
}

dGetEnvAttr::dGetEnvAttr( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Environment Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( env_options, types );

	l_handle = new QLabel( "Environment Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "Attribute:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	target_valid = new QCheckBox( "ValuePtr: VALID", this );
	target_valid -> setGeometry( 10, 110, 300, 15 );

	strlen_valid = new QCheckBox( "StrLen_Ptr: VALID", this );
	strlen_valid -> setGeometry( 10, 140, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 400, 110, 70, 20 );
	buffer_len -> setMaxLength( 7 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Len:", this );
    l_buffer_len -> setGeometry( 320, 110, 60, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( target_valid, SIGNAL( clicked()), this, SLOT( target_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
}

dGetEnvAttr::~dGetEnvAttr()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
	delete target_valid;
	delete strlen_valid;
	delete buffer_len;
	delete l_buffer_len;
}

void dSetStmtAttr::Activated( int index )
{
	value -> clear();

	if ( !stmt_options[ index ].values[ 0 ].text )
	{
		switch( stmt_options[ index ].attr )
		{
		  case SQL_ATTR_FETCH_BOOKMARK_PTR:
		  case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
		  case SQL_ATTR_PARAM_OPERATION_PTR:
		  case SQL_ATTR_PARAM_STATUS_PTR:
		  case SQL_ATTR_PARAMS_PROCESSED_PTR:
		  case SQL_ATTR_ROW_BIND_OFFSET_PTR:
		  case SQL_ATTR_ROW_OPERATION_PTR:
		  case SQL_ATTR_ROW_STATUS_PTR:
		  case SQL_ATTR_ROWS_FETCHED_PTR:
			value->insertItem( "<valid ptr>", 0 );
			break;

		  default:
			value->insertItem( "0", 0 );
			break;
		}
	}
	else
	{
		odbctest->fill_list_box( stmt_options[ index ].values, value );
	}
}

void dSetStmtAttr::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HSTMT;
	SQLINTEGER attribute;
	SQLPOINTER vptr;
	SQLUINTEGER ival;
	const char *tptr;
	attr_value *ptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	int index = types->currentItem();
	int i;
    QString qtptr;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetStmtAttr():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statment Handle: %p", in_handle );
	else
		txt.sprintf( "    Statment Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = stmt_options[ types -> currentItem() ].attr;
	txt.sprintf( "    Attribute: %s=%d", 
		stmt_options[ types -> currentItem() ].text,
		stmt_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

    qtptr = value -> currentText();
	tptr = qtptr.ascii();

	/*
     * try and match the text
	 */

	for ( ptr = stmt_options[ types -> currentItem() ].values; 
		ptr -> text; ptr ++ )
	{
		if ( strncmp( ptr -> text, tptr, strlen( ptr -> text )) == 0 )
		{
			break;
		}
	}

	if ( !ptr -> text )
	{
		if ( attribute == SQL_ATTR_ROW_ARRAY_SIZE ||
        		attribute == SQL_ATTR_PARAMSET_SIZE )
		{
			ival = atoi( tptr );
		}

		vptr = (SQLPOINTER) atoi( tptr );
		txt.sprintf( "    Value: %d", atoi( tptr ));
	}
	else
	{
		if ( attribute == SQL_ATTR_ROW_ARRAY_SIZE ||
        		attribute == SQL_ATTR_PARAMSET_SIZE )
		{
			ival = ptr -> value;
		}

		vptr = (SQLPOINTER) ptr -> value;
		txt.sprintf( "    Value: %s=%d", ptr -> text, ptr -> value );
	}

	if ( strcmp( tptr, "<valid ptr>" ) == 0 )
	{
		switch( stmt_options[ types -> currentItem() ].attr )
		{
		  case SQL_ATTR_FETCH_BOOKMARK_PTR:
			vptr = hand-> bookmark_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
			vptr = hand-> param_bind_offset_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_PARAM_OPERATION_PTR:
			vptr = hand-> param_opt_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_PARAM_STATUS_PTR:
			vptr = hand-> param_status_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_PARAMS_PROCESSED_PTR:
			vptr = hand-> params_processed_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_ROW_BIND_OFFSET_PTR:
			vptr = hand-> row_bind_offset_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_ROW_OPERATION_PTR:
			vptr = hand-> row_operation_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_ROW_STATUS_PTR:
			vptr = hand-> row_status_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;

		  case SQL_ATTR_ROWS_FETCHED_PTR:
			vptr = hand-> rows_fetched_ptr;
			txt.sprintf( "    Value: %x", vptr );
			break;
		}
	}

	odbctest -> out_win -> insertLineLimited( txt );

	string_length = strlen_options[ stringlen -> currentItem() ].value;
	txt.sprintf( "    String Length: %s=%d", 
		strlen_options[ stringlen -> currentItem() ].text,
		strlen_options[ stringlen -> currentItem() ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetStmtAttr( in_handle, attribute, vptr, string_length );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );

    /*
     * save the row or param array size
     */

    if ( SQL_SUCCEEDED( ret ))
    {
        if ( attribute == SQL_ATTR_ROW_ARRAY_SIZE )
        {
            hand -> row_array_size = ival;
        }
        else if ( attribute == SQL_ATTR_PARAMSET_SIZE )
        {
            hand -> param_array_size = ival;
        }
    }
}

dSetStmtAttr::dSetStmtAttr( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( stmt_options, types );

	value = new QComboBox( TRUE, this, "Value" );
	value -> setGeometry( 170, 110, 300, 20 );
	parent->fill_list_box( stmt_options[ 0 ].values, value );

	stringlen = new QComboBox( FALSE, this, "String Length" );
	stringlen -> setGeometry( 170, 140, 300, 20 );
	parent->fill_list_box( strlen_options, stringlen );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "Attribute:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	l_value = new QLabel( "Value:", this );
	l_value -> setGeometry( 10, 110, 130, 20 );

	l_slen = new QLabel( "String Length:", this );
	l_slen -> setGeometry( 10, 140, 130, 20 );

	Activated( 0 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( types, SIGNAL(activated(int)), this, SLOT( Activated(int)));
}

dSetStmtAttr::~dSetStmtAttr()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete value;
	delete stringlen;
	delete l_handle;
	delete l_types;
	delete l_value;
	delete l_slen;
}

void dSetStmtOption::Activated( int index )
{
	value -> clear();

	if ( !stmt_opt_options[ index ].values[ 0 ].text )
	{
        value->insertItem( "0", 0 );
	}
	else
	{
		odbctest->fill_list_box( stmt_opt_options[ index ].values, value );
	}
}

void dSetStmtOption::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HSTMT;
	SQLINTEGER attribute;
	SQLINTEGER ival;
	SQLROWCOUNT vptr;
	const char *tptr;
	attr_value *ptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	int index = types->currentItem();
	int i;
    QString qtptr;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetStmtOption():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    hstmt: %p", in_handle );
	else
		txt.sprintf( "    hstmt: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = stmt_opt_options[ types -> currentItem() ].attr;
	txt.sprintf( "    fOption: %s=%d", 
		stmt_opt_options[ types -> currentItem() ].text,
		stmt_opt_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

    qtptr = value -> currentText();
	tptr = qtptr.ascii();

	/*
     * try and match the text
	 */

	for ( ptr = stmt_opt_options[ types -> currentItem() ].values; 
		ptr -> text; ptr ++ )
	{
		if ( strncmp( ptr -> text, tptr, strlen( ptr -> text )) == 0 )
		{
			break;
		}
	}

	if ( !ptr -> text )
	{
		vptr = (SQLROWCOUNT) atoi( tptr );
		txt.sprintf( "    vParam: %d", atoi( tptr ));
		if ( attribute == SQL_ROWSET_SIZE )
		{
			ival = atoi( tptr );
		}
	}
	else
	{
		vptr = (SQLROWCOUNT) ptr -> value;
		txt.sprintf( "    vParam: %s=%d", ptr -> text, ptr -> value );
		if ( attribute == SQL_ROWSET_SIZE )
		{
			ival = ptr -> value;
		}
	}

	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetStmtOption( in_handle, attribute, vptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );

    /*
     * save the row or param array size
     */

    if ( SQL_SUCCEEDED( ret ))
    {
        if ( attribute == SQL_ROWSET_SIZE )
        {
            hand -> row_array_size = ival;
        }
    }
}

dSetStmtOption::dSetStmtOption( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( stmt_opt_options, types );

	value = new QComboBox( TRUE, this, "Value" );
	value -> setGeometry( 170, 110, 300, 20 );
	parent->fill_list_box( stmt_opt_options[ 0 ].values, value );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "fOption:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	l_value = new QLabel( "vParam:", this );
	l_value -> setGeometry( 10, 110, 130, 20 );

	Activated( 0 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( types, SIGNAL(activated(int)), this, SLOT( Activated(int)));
}

dSetStmtOption::~dSetStmtOption()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete value;
	delete l_handle;
	delete l_types;
	delete l_value;
}

void dGetStmtAttr::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HENV;
	SQLINTEGER attribute;
	SQLPOINTER vptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLINTEGER b_len, strlen_or_ind;
	SQLINTEGER *strlen_ptr;
	char *buf = NULL; 
	int i;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetStmtAttr():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = stmt_options[ types -> currentItem() ].attr;
	txt.sprintf( "    Attribute: %s=%d", 
		stmt_options[ types -> currentItem() ].text,
		stmt_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( target_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = new char[ 300 ];	
	}
	else
	{
		buf = new char[ b_len ];	
	}

	if ( buf )
	{
		txt.sprintf( "    Value Ptr: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Value Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    Buffer Length: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	strlen_or_ind = -999999;

	if ( strlen_valid -> isOn())
	{
		strlen_ptr = NULL;
	}
	else
	{
		strlen_ptr = &strlen_or_ind;
	}

	if ( strlen_ptr )
	{
		txt.sprintf( "    Strlen Ptr: %p", strlen_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Strlen Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetStmtAttr( in_handle, attribute, 
			buf, b_len, strlen_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( strlen_ptr )
	{	
		if ( strlen_or_ind == -999999 )
		{
			txt.sprintf( "    *Strlen Ptr: <unchanged>" );
		}
		else
		{
			txt.sprintf( "    *Strlen Ptr: %d", strlen_or_ind );
		}
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( SQL_SUCCEEDED( ret ))
	{
		int index = types -> currentItem();

		if ( stmt_options[ index ].is_pointer )
		{
			void *ival;
			memcpy( &ival, buf, sizeof( ival ));
			txt.sprintf( "    ValuePtr = 0x%08X", ival );
			odbctest -> out_win -> insertLineLimited( txt );
		}
		else
		{
			switch( stmt_options[ index ].data_type )
			{
				case SQL_INTEGER:
					SQLUINTEGER ival;
					memcpy( &ival, buf, sizeof( ival ));
					txt.sprintf( "    *ValuePtr = %d (0x%08X)", ival, ival );
					odbctest -> out_win -> insertLineLimited( txt );
	
					if ( stmt_options[ index ].values[ 0 ].text )
					{
						if ( stmt_options[ index ].is_bitmap )
						{
							int i;
	
							for ( i = 0; stmt_options[ index ].values[ i ].text; i ++ )
							{
								if ( stmt_options[ index ].values[ i ].value & ival )
								{
									txt.sprintf( "        %s", 
										stmt_options[ index ].values[ i ].text );
									odbctest -> out_win -> insertLineLimited( txt );
								}
							}
						}
						else
						{
							int i;
	
							for ( i = 0; stmt_options[ index ].values[ i ].text; i ++ )
							{
								if ( stmt_options[ index ].values[ i ].value == ival )
								{
									txt.sprintf( "        %s", 
										stmt_options[ index ].values[ i ].text );
									odbctest -> out_win -> insertLineLimited( txt );
								}
							}
						}
					}
					break;
	
				case SQL_SMALLINT:
					SQLSMALLINT sval;
					memcpy( &sval, buf, sizeof( sval ));
					txt.sprintf( "    *ValuePtr = %d (0x%04X)", sval, sval );
					odbctest -> out_win -> insertLineLimited( txt );
	
					if ( stmt_options[ index ].values[ 0 ].text )
					{
						if ( stmt_options[ index ].is_bitmap )
						{
							int i;
	
							for ( i = 0; stmt_options[ index ].values[ i ].text; i ++ )
							{
								if ( stmt_options[ index ].values[ i ].value & sval )
								{
									txt.sprintf( "        %s", 
										stmt_options[ index ].values[ i ].text );
									odbctest -> out_win -> insertLineLimited( txt );
								}
							}
						}
						else
						{
							int i;
	
							for ( i = 0; stmt_options[ index ].values[ i ].text; i ++ )
							{
								if ( stmt_options[ index ].values[ i ].value == sval )
								{
									txt.sprintf( "        %s", 
										stmt_options[ index ].values[ i ].text );
									odbctest -> out_win -> insertLineLimited( txt );
								}
							}
						}
					}
					break;
			}		
		}
	}
	
	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

void dGetStmtAttr::target_clkd()
{
	if ( target_valid -> isOn() )
	    target_valid -> setText( "ValuePtr: SQL_NULL_POINTER" );
	else
	    target_valid -> setText( "ValuePtr: VALID" );
}

void dGetStmtAttr::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StrLen_Ptr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StrLen_Ptr: VALID" );
}

dGetStmtAttr::dGetStmtAttr( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( stmt_options, types );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "Attribute:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	target_valid = new QCheckBox( "ValuePtr: VALID", this );
	target_valid -> setGeometry( 10, 110, 300, 15 );

	strlen_valid = new QCheckBox( "StrLen_Ptr: VALID", this );
	strlen_valid -> setGeometry( 10, 140, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 400, 110, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Len:", this );
    l_buffer_len -> setGeometry( 320, 110, 60, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( target_valid, SIGNAL( clicked()), this, SLOT( target_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
}

dGetStmtAttr::~dGetStmtAttr()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
	delete target_valid;
	delete strlen_valid;
	delete buffer_len;
	delete l_buffer_len;
}

void dGetStmtOption::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HENV;
	SQLINTEGER attribute, value;
	SQLPOINTER vptr;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	int index = types->currentItem();
	char *buf = NULL;
	int i;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetStmtOption():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    hstmt: %p", in_handle );
	else
		txt.sprintf( "    hstmt: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = stmt_opt_options[ types -> currentItem() ].attr;
	txt.sprintf( "    fOption: %s=%d", 
		stmt_opt_options[ types -> currentItem() ].text,
		stmt_opt_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( target_valid -> isOn())
	{
		buf = NULL;
        vptr = NULL;
	}
    else
    {
        if ( stmt_opt_options[ index ].data_type == SQL_INTEGER )
        {
            vptr = &value;
        }
        else
        {
		    buf = new char[ 300 ];	
            vptr = buf;
        }
	}

	if ( vptr )
	{
		txt.sprintf( "    vParam: %p", vptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    vParam: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetStmtOption( in_handle, attribute, 
			vptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );

	if ( SQL_SUCCEEDED( ret ))
    {
        if ( vptr )
        {	
			switch( stmt_opt_options[ index ].data_type )
            {
				case SQL_INTEGER:
                {
                    int i;

			        txt.sprintf( "    *vptr = 0x%08X", value );
                    odbctest -> out_win -> insertLineLimited( txt );
	
                    for ( i = 0; stmt_opt_options[ index ].values[ i ].text; i ++ )
                    {
                        if ( stmt_opt_options[ index ].values[ i ].value == value )
                        {
                            txt.sprintf( "        %s", 
                                stmt_opt_options[ index ].values[ i ].text );
                            odbctest -> out_win -> insertLineLimited( txt );
                        }
                    }
                    break;
                }

                default:
			        txt.sprintf( "    *vptr = %s", buf );
                    break;
            }
        }
    }

	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

void dGetStmtOption::target_clkd()
{
	if ( target_valid -> isOn() )
	    target_valid -> setText( "vParam: SQL_NULL_POINTER" );
	else
	    target_valid -> setText( "vParam: VALID" );
}

dGetStmtOption::dGetStmtOption( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "hstmt" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	types = new QComboBox( FALSE, this, "fOption" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( stmt_opt_options, types );

	l_handle = new QLabel( "hstmt:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "fOption:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	target_valid = new QCheckBox( "vParam: VALID", this );
	target_valid -> setGeometry( 10, 110, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( target_valid, SIGNAL( clicked()), this, SLOT( target_clkd()));
}

dGetStmtOption::~dGetStmtOption()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
	delete target_valid;
}

void dSetConnAttr::Activated( int index )
{
	value -> clear();

	if ( !conn_options[ index ].values[ 0 ].text )
	{
		if ( conn_options[ index ].data_type == SQL_CHAR )
		{
			value->insertItem( "<null pointer>", 0 );
			value->insertItem( "", 1 );
		}
		else
		{
			value->insertItem( "0", 0 );
		}
	}
	else
	{
		odbctest->fill_list_box( conn_options[ index ].values, value );
	}
}

void dSetConnAttr::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HDBC;
	SQLINTEGER attribute;
	SQLPOINTER vptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_DBC, handles );
	int i;
	attr_value *ptr;
	const char *tptr;
    QString qtptr;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetConnectAttr():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Connection Handle: %p", in_handle );
	else
		txt.sprintf( "    Connection Handle: SQL_NULL_HDBC" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = conn_options[ types -> currentItem() ].attr;
	txt.sprintf( "    Attribute: %s=%d", 
		conn_options[ types -> currentItem() ].text,
		conn_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	qtptr = value -> currentText();
    tptr = qtptr.ascii();

	/*
     * try and match the text
	 */

	for ( ptr = conn_options[ types -> currentItem() ].values; 
		ptr -> text; ptr ++ )
	{
		if ( strncmp( ptr -> text, tptr, strlen( ptr -> text )) == 0 )
		{
			break;
		}
	}

	if ( !ptr -> text )
	{
		if ( conn_options[ types -> currentItem() ].data_type == SQL_CHAR )
		{
			vptr = (SQLPOINTER) tptr;
            if ( strncmp( "<null pointer>", tptr, 14 ) == 0 )
            {
                vptr = NULL;
			    txt.sprintf( "    Value: SQL_NULL_POINTER" );
            }
            else
            {
			    vptr = (SQLPOINTER) tptr;
			    txt.sprintf( "    Value: %s", tptr );
            }
		}
		else
		{
			vptr = (SQLPOINTER) atoi( tptr );
			txt.sprintf( "    Value: %d", atoi( tptr ));
		}
	}
	else
	{
		vptr = (SQLPOINTER) ptr -> value;
		txt.sprintf( "    Value: %s=%d", ptr -> text, ptr -> value );
	}

	odbctest -> out_win -> insertLineLimited( txt );

	string_length = strlen_options[ stringlen -> currentItem() ].value;
	txt.sprintf( "    String Length: %s=%d", 
		strlen_options[ stringlen -> currentItem() ].text,
		strlen_options[ stringlen -> currentItem() ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetConnectAttr( in_handle, attribute, vptr, string_length );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dSetConnAttr::dSetConnAttr( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Connection Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_DBC, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( conn_options, types );

	value = new QComboBox( TRUE, this, "Value" );
	value -> setGeometry( 170, 110, 300, 20 );
	parent->fill_list_box( conn_options[ 0 ].values, value );

	stringlen = new QComboBox( FALSE, this, "String Length" );
	stringlen -> setGeometry( 170, 140, 300, 20 );
	parent->fill_list_box( strlen_options, stringlen );

	l_handle = new QLabel( "Connection Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "Attribute:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	l_value = new QLabel( "Value:", this );
	l_value -> setGeometry( 10, 110, 130, 20 );

	l_slen = new QLabel( "String Length:", this );
	l_slen -> setGeometry( 10, 140, 130, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( types, SIGNAL(activated(int)), this, SLOT( Activated(int)));
}

dSetConnAttr::~dSetConnAttr()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete value;
	delete stringlen;
	delete l_handle;
	delete l_types;
	delete l_value;
	delete l_slen;
}

void dGetConnAttr::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HDBC;
	SQLINTEGER attribute;
	SQLPOINTER vptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_DBC, handles );
	SQLINTEGER b_len, strlen_or_ind;
	SQLINTEGER *strlen_ptr;
	char *buf = NULL;
	int i;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetConnectAttr():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Connection Handle: %p", in_handle );
	else
		txt.sprintf( "    Connection Handle: SQL_NULL_HDBC" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = conn_options[ types -> currentItem() ].attr;
	txt.sprintf( "    Attribute: %s=%d", 
		conn_options[ types -> currentItem() ].text,
		conn_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( target_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = new char[ 300 ];	
	}
	else
	{
		buf = new char[ b_len ];	
	}

	if ( buf )
	{
		txt.sprintf( "    Value Ptr: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Value Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    Buffer Length: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	strlen_or_ind = -999999;

	if ( strlen_valid -> isOn())
	{
		strlen_ptr = NULL;
	}
	else
	{
		strlen_ptr = &strlen_or_ind;
	}

	if ( strlen_ptr )
	{
		txt.sprintf( "    Strlen Ptr: %p", strlen_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Strlen Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetConnectAttr( in_handle, attribute, 
			buf, b_len, strlen_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( strlen_ptr )
	{	
		if ( strlen_or_ind == -999999 )
		{
			txt.sprintf( "    *Strlen Ptr: <unchanged>" );
		}
		else
		{
			txt.sprintf( "    *Strlen Ptr: %d", strlen_or_ind );
		}
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( SQL_SUCCEEDED( ret ))
	{
		int index = types -> currentItem();
		switch( conn_options[ index ].data_type )
		{
			case SQL_CHAR:
				txt.sprintf( "    *InfoValuePtr = \"%s\"", buf );
				odbctest -> out_win -> insertLineLimited( txt );
				break;

			case SQL_INTEGER:
				SQLUINTEGER ival;
				memcpy( &ival, buf, sizeof( ival ));
				txt.sprintf( "    *ValuePtr = %d (0x%08X)", ival, ival );
				odbctest -> out_win -> insertLineLimited( txt );

				if ( conn_options[ index ].values[ 0 ].text )
				{
					if ( conn_options[ index ].is_bitmap )
					{
						int i;

						for ( i = 0; conn_options[ index ].values[ i ].text; i ++ )
						{
							if ( conn_options[ index ].values[ i ].value & ival )
							{
								txt.sprintf( "        %s", 
									conn_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
					else
					{
						int i;

						for ( i = 0; conn_options[ index ].values[ i ].text; i ++ )
						{
							if ( conn_options[ index ].values[ i ].value == ival )
							{
								txt.sprintf( "        %s", 
									conn_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
				}
				break;

			case SQL_SMALLINT:
				SQLSMALLINT sval;
				memcpy( &sval, buf, sizeof( sval ));
				txt.sprintf( "    *ValuePtr = %d (0x%04X)", sval, sval );
				odbctest -> out_win -> insertLineLimited( txt );

				if ( conn_options[ index ].values[ 0 ].text )
				{
					if ( conn_options[ index ].is_bitmap )
					{
						int i;

						for ( i = 0; conn_options[ index ].values[ i ].text; i ++ )
						{
							if ( conn_options[ index ].values[ i ].value & sval )
							{
								txt.sprintf( "        %s", 
									conn_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
					else
					{
						int i;

						for ( i = 0; conn_options[ index ].values[ i ].text; i ++ )
						{
							if ( conn_options[ index ].values[ i ].value == sval )
							{
								txt.sprintf( "        %s", 
									conn_options[ index ].values[ i ].text );
								odbctest -> out_win -> insertLineLimited( txt );
							}
						}
					}
				}
				break;
		}		
	}

	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

void dGetConnAttr::target_clkd()
{
	if ( target_valid -> isOn() )
	    target_valid -> setText( "ValuePtr: SQL_NULL_POINTER" );
	else
	    target_valid -> setText( "ValuePtr: VALID" );
}

void dGetConnAttr::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StrLen_Ptr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StrLen_Ptr: VALID" );
}

dGetConnAttr::dGetConnAttr( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Connection Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_DBC, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( conn_options, types );

	l_handle = new QLabel( "Connection Handle:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "Attribute:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	target_valid = new QCheckBox( "ValuePtr: VALID", this );
	target_valid -> setGeometry( 10, 110, 300, 15 );

	strlen_valid = new QCheckBox( "StrLen_Ptr: VALID", this );
	strlen_valid -> setGeometry( 10, 140, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 400, 110, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Len:", this );
    l_buffer_len -> setGeometry( 320, 110, 60, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( target_valid, SIGNAL( clicked()), this, SLOT( target_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
}

dGetConnAttr::~dGetConnAttr()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
	delete target_valid;
	delete strlen_valid;
	delete buffer_len;
	delete l_buffer_len;
}

void dSetConnectOption::Activated( int index )
{
	value -> clear();

	if ( !conn_opt_options[ index ].values[ 0 ].text )
	{
		if ( conn_opt_options[ index ].data_type == SQL_CHAR )
		{
			value->insertItem( "<null pointer>", 0 );
			value->insertItem( "", 1 );
		}
		else
		{
            value->insertItem( "0", 0 );
        }
	}
	else
	{
		odbctest->fill_list_box( conn_opt_options[ index ].values, value );
	}
}

void dSetConnectOption::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HDBC;
	SQLINTEGER attribute;
	SQLULEN vptr;
	const char *tptr;
	attr_value *ptr;
	SQLINTEGER string_length;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_DBC, handles );
	int i;
    QString qtptr;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetConnectOption():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    hdbc: %p", in_handle );
	else
		txt.sprintf( "    hdbc: SQL_NULL_HDBC" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = conn_opt_options[ types -> currentItem() ].attr;
	txt.sprintf( "    fOption: %s=%d", 
		conn_opt_options[ types -> currentItem() ].text,
		conn_opt_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	qtptr = value -> currentText();
    tptr = qtptr.ascii();

	/*
     * try and match the text
	 */

	for ( ptr = conn_opt_options[ types -> currentItem() ].values; 
		ptr -> text; ptr ++ )
	{
		if ( strncmp( ptr -> text, tptr, strlen( ptr -> text )) == 0 )
		{
			break;
		}
	}
	if ( !ptr -> text )
	{
		// This can't be done on 64 bit with old definitions...
		if ( conn_opt_options[ types -> currentItem() ].data_type == SQL_CHAR )
		{
#if (SIZEOF_LONG == 8)
#ifndef DO_YOU_KNOW_WHAT_YOUR_ARE_DOING
			fprintf( stderr, "unable to do this on this processor with 32 bit build options...\n" );
#else
			vptr = (SQLULEN) tptr;
#endif
#else
			vptr = (SQLULEN) tptr;
#endif
            if ( strncmp( "<null pointer>", tptr, 14 ) == 0 )
            {
                vptr = 0;
			    txt.sprintf( "    vParam: SQL_NULL_POINTER" );
            }
            else
            {
#if (SIZEOF_LONG == 8)
#ifndef DO_YOU_KNOW_WHAT_YOUR_ARE_DOING
			    fprintf( stderr, "unable to do this on this processor with 32 bit build options...\n" );
#else
			    vptr = (SQLULEN) tptr;
#endif
#else
			    vptr = (SQLULEN) tptr;
#endif
			    txt.sprintf( "    vParam: %s", tptr );
            }
		}
		else
		{
			vptr = (SQLULEN) atoi( tptr );
			txt.sprintf( "    vParam: %d", atoi( tptr ));
		}
	}
	else
	{
		vptr = (SQLULEN) ptr -> value;
		txt.sprintf( "    vParam: %s=%d", ptr -> text, ptr -> value );
	}

	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetConnectOption( in_handle, attribute, vptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dSetConnectOption::dSetConnectOption( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	types = new QComboBox( FALSE, this, "Attribute" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( conn_opt_options, types );

	value = new QComboBox( TRUE, this, "Value" );
	value -> setGeometry( 170, 110, 300, 20 );
	parent->fill_list_box( conn_opt_options[ 0 ].values, value );

	l_handle = new QLabel( "hdbc:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "fOption:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	l_value = new QLabel( "vParam:", this );
	l_value -> setGeometry( 10, 110, 130, 20 );

	Activated( 0 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( types, SIGNAL(activated(int)), this, SLOT( Activated(int)));
}

dSetConnectOption::~dSetConnectOption()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete value;
	delete l_handle;
	delete l_types;
	delete l_value;
}

void dGetConnectOption::Ok()
{
	SQLHANDLE in_handle = SQL_NULL_HENV;
	SQLINTEGER attribute, value;
	SQLPOINTER vptr;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	int index = types->currentItem();
	char *buf = NULL;
	int i;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetConnectOption():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    hstmt: %p", in_handle );
	else
		txt.sprintf( "    hstmt: SQL_NULL_HDBC" );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = conn_gopt_options[ types -> currentItem() ].attr;
	txt.sprintf( "    fOption: %s=%d", 
		conn_gopt_options[ types -> currentItem() ].text,
		conn_gopt_options[ types -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( target_valid -> isOn())
	{
		buf = NULL;
        vptr = NULL;
	}
    else
    {
        if ( conn_gopt_options[ index ].data_type == SQL_INTEGER )
        {
            vptr = &value;
        }
        else
        {
		    buf = new char[ 300 ];	
            vptr = buf;
        }
	}

	if ( vptr )
	{
		txt.sprintf( "    vParam: %p", vptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    vParam: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetConnectOption( in_handle, attribute, 
			vptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );

	if ( SQL_SUCCEEDED( ret ))
    {
        if ( vptr )
        {	
			switch( conn_gopt_options[ index ].data_type )
            {
				case SQL_INTEGER:
                {
                    int i;

			        txt.sprintf( "    *vptr = 0x%08X", value );
                    odbctest -> out_win -> insertLineLimited( txt );
	
                    for ( i = 0; conn_gopt_options[ index ].values[ i ].text; i ++ )
                    {
                        if ( conn_gopt_options[ index ].values[ i ].value == value )
                        {
                            txt.sprintf( "        %s", 
                                conn_gopt_options[ index ].values[ i ].text );
                            odbctest -> out_win -> insertLineLimited( txt );
                        }
                    }
                    break;
                }

                default:
			        txt.sprintf( "    *vptr = %s", buf );
                    break;
            }
        }
    }

	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

void dGetConnectOption::target_clkd()
{
	if ( target_valid -> isOn() )
	    target_valid -> setText( "vParam: SQL_NULL_POINTER" );
	else
	    target_valid -> setText( "vParam: VALID" );
}

dGetConnectOption::dGetConnectOption( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "hdbc" );
	handles -> setGeometry( 170, 50, 300, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_DBC, handles );

	types = new QComboBox( FALSE, this, "fOption" );
	types -> setGeometry( 170, 80, 300, 20 );

	parent->fill_list_box( conn_gopt_options, types );

	l_handle = new QLabel( "hstmt:", this );
	l_handle -> setGeometry( 10, 50, 130, 20 );

	l_types = new QLabel( "fOption:", this );
	l_types -> setGeometry( 10, 80, 130, 20 );

	target_valid = new QCheckBox( "vParam: VALID", this );
	target_valid -> setGeometry( 10, 110, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( target_valid, SIGNAL( clicked()), this, SLOT( target_clkd()));
}

dGetConnectOption::~dGetConnectOption()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
	delete target_valid;
}

void OdbcTest::sqlsetstmtattr()
{
	dSetStmtAttr *dlg = new dSetStmtAttr( this, "SQLSetStmtAttr" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetstmtattr()
{
	dGetStmtAttr *dlg = new dGetStmtAttr( this, "SQLGetStmtAttr" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetconnectattr()
{
	dSetConnAttr *dlg = new dSetConnAttr( this, "SQLSetConnectAttr" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetconnectattr()
{
	dGetConnAttr *dlg = new dGetConnAttr( this, "SQLGetConnectAttr" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetenvattr()
{
	dSetEnvAttr *dlg = new dSetEnvAttr( this, "SQLSetEnvAttr" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetenvattr()
{
	dGetEnvAttr *dlg = new dGetEnvAttr( this, "SQLGetEnvAttr" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetstmtoption()
{
	dSetStmtOption *dlg = new dSetStmtOption( this, "SQLSetStmtOption" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetstmtoption()
{
	dGetStmtOption *dlg = new dGetStmtOption( this, "SQLGetStmtOption" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetconnectoption()
{
	dSetConnectOption *dlg = new dSetConnectOption( this, "SQLSetConnectOption" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetconnectoption()
{
	dGetConnectOption *dlg = new dGetConnectOption( this, "SQLGetConnectOption" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::setcursoratributes()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}
