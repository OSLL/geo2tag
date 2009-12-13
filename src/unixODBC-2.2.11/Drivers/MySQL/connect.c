/***************************************************************************  
 * Copyright (C) 1995-2002 MySQL AB, www.mysql.com                         *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program; if not, write to the Free Software Foundation  *
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA           *
****************************************************************************/

/*************************************************************************** 
 * CONNECT.C                                                               *
 *                                                                         *
 * @description: This is the MyODBC 3.51 sample driver code for connection *
 *                                                                         *
 * @author     : MySQL AB (monty@mysql.com, venu@mysql.com)                *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLConnect         (ISO 92)                                           *
 *   SQLDriverConnect   (ODBC)                                             *
 *   SQLDisconnect      (ISO 92)                                           *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"

#ifndef _UNIX_
#include <odbcinst.h>
#include "dialogs.h"
#endif

#ifndef CLIENT_NO_SCHEMA
#define CLIENT_NO_SCHEMA      16
#endif

/*
  @type    : myodbc3 internal
  @purpose : check the option flag and generate a connect flag
*/

ulong get_client_flag(MYSQL *mysql, ulong option_flag,uint connect_timeout,
		      char *init_stmt)
{
  ulong client_flag=CLIENT_ODBC;
  DBUG_ENTER("get_client_flag");

  mysql_init(mysql);
#ifndef DBUG_OFF
  if (option_flag & FLAG_DEBUG && ! _db_on_)
    mysql_debug("d:t:S:O,c::\\myodbc.log");
#endif

  if (option_flag & (FLAG_FOUND_ROWS | FLAG_SAFE))
    client_flag|=   CLIENT_FOUND_ROWS;
  if (option_flag & FLAG_NO_SCHEMA)
    client_flag|=   CLIENT_NO_SCHEMA;
  if (option_flag & (FLAG_BIG_PACKETS | FLAG_SAFE))
    max_allowed_packet=~0L;
  if (option_flag & FLAG_COMPRESSED_PROTO)
    client_flag |=  CLIENT_COMPRESS;
  if (option_flag & FLAG_IGNORE_SPACE)
    client_flag |=  CLIENT_IGNORE_SPACE;
#ifdef __WIN__
  if (option_flag & FLAG_NAMED_PIPE)
    mysql_options(mysql,MYSQL_OPT_NAMED_PIPE,NullS);
#endif
  if (option_flag & FLAG_USE_MYCNF)
    mysql_options(mysql,MYSQL_READ_DEFAULT_GROUP,"odbc");
  if (init_stmt && init_stmt[0])
    mysql_options(mysql,MYSQL_INIT_COMMAND,init_stmt);
  if (connect_timeout)
    mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT,(char*) &connect_timeout);
  DBUG_PRINT("info",("MyODBC: %s,option_flag: %ld,client_flag: %ld",
		     DRIVER_VERSION, option_flag, client_flag));
  DBUG_RETURN(client_flag);
}


/*
  @type    : myodbc3 internal
  @purpose : simple help functions to copy arguments if given
*/

void copy_if_not_empty(char *to,uint max_length, char *from,int length)
{
  if (from)
  {
    if (length == SQL_NTS)
      length=max_length-1;
    strmake(to,from,length);
  }
}


/*
  @type    : ODBC 1.o API
  @purpose : to connect to mysql server
*/

SQLRETURN SQL_API
SQLConnect(SQLHDBC hdbc, SQLCHAR FAR *szDSN,
	   SQLSMALLINT	 cbDSN,
	   SQLCHAR FAR *szUID, SQLSMALLINT cbUID,
	   SQLCHAR FAR *szAuthStr, SQLSMALLINT cbAuthStr)
{
  char host[64],user[64],passwd[64],dsn[NAME_LEN+1],database[NAME_LEN+1];
  char port[10],flag[10],init_stmt[256],*dsn_ptr;
  char szTRACE[FILENAME_MAX+1]= "";
  char socket[256]= "";
  ulong flag_nr,client_flag;
  uint port_nr=0;
  DBC FAR *dbc=(DBC FAR*) hdbc;
  DBUG_ENTER("SQLConnect");

  if (dbc->mysql.net.vio != 0)
  {
    DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_08002,NULL,0));
  }
  /* Reset error state */
  dbc->error.sqlstate[0]=0;

  dsn_ptr=fix_str(dsn,szDSN,cbDSN);
  if (dsn_ptr && !dsn_ptr[0])
  {
    DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_S1000,
				 "Invalid Connection Parameters",0));
  }

  SQLGetPrivateProfileString(dsn_ptr,"user",
			     "", user, sizeof(user),"ODBC.INI");
  SQLGetPrivateProfileString(dsn_ptr,"password",
			     "", passwd, sizeof(passwd),"ODBC.INI");
  SQLGetPrivateProfileString(dsn_ptr,"server",
			     "localhost", host, sizeof(host),"ODBC.INI");
  SQLGetPrivateProfileString(dsn_ptr,"database",
			     dsn_ptr, database, sizeof(database),"ODBC.INI");
  SQLGetPrivateProfileString(dsn_ptr,"port",
			     "0", port, sizeof(port),"ODBC.INI");
  port_nr=(uint) atoi(port);
  SQLGetPrivateProfileString(dsn_ptr,"option",
			     "0", flag, sizeof(flag),"ODBC.INI");
  flag_nr=(ulong) atol(flag);

#ifdef _UNIX_
  SQLGetPrivateProfileString(dsn_ptr,"socket",
			     "", socket, sizeof(socket),"ODBC.INI");
#endif

  SQLGetPrivateProfileString(dsn_ptr,"stmt",
			     "", init_stmt, sizeof(init_stmt),"ODBC.INI");

  SQLGetPrivateProfileString( dsn_ptr, "TRACE", "", szTRACE, sizeof(szTRACE),
			      "ODBC.INI" );
  if (szTRACE[0] == '0' || szTRACE[0] == 'N' || szTRACE[0] == 'n' ||
      ((szTRACE[0] == 'O' || szTRACE[0] == 'o') &&
       (szTRACE[1] == 'N' || szTRACE[1] == 'n')))
  {
    char  szTRACEFILE[FILENAME_MAX+1] = "";
    char  szMYODBC_LOG[FILENAME_MAX+20]= "";
    SQLGetPrivateProfileString( dsn_ptr, "TRACEFILE", "", szTRACEFILE,
				sizeof(szTRACEFILE), "ODBC.INI" );
    if (*szTRACEFILE)
    {
      sprintf(szMYODBC_LOG, "d:t:S:A,%s", szTRACEFILE);
      DBUG_POP();
      DBUG_PUSH(szMYODBC_LOG);
    }
  }
  client_flag=get_client_flag(&dbc->mysql,flag_nr,(uint) dbc->login_timeout,
			      init_stmt);

  copy_if_not_empty(passwd,sizeof(passwd), (char FAR*) szAuthStr,cbAuthStr);
  copy_if_not_empty(user, sizeof(user), (char FAR *) szUID, cbUID);

  /* socket[0] is always 0 if you are not under UNIX */
  if (!mysql_real_connect(&dbc->mysql,host,user,passwd[0] ? passwd : 0,
			  database, port_nr,
			  socket[0] ? socket: NullS,
			  (uint) client_flag))
  {
    set_dbc_error(dbc, "S1000", mysql_error(&dbc->mysql),
		  mysql_errno(&dbc->mysql));
    translate_error(dbc->error.sqlstate,"S1000",mysql_errno(&dbc->mysql));
    DBUG_RETURN(SQL_ERROR);
  }
  dbc->dsn= my_strdup(dsn_ptr ? dsn_ptr : database ,MYF(MY_WME));
  dbc->database=my_strdup(database,MYF(MY_WME));
  dbc->server=my_strdup(host,MYF(MY_WME));
  dbc->user=my_strdup(user,MYF(MY_WME));
  dbc->password=my_strdup(passwd,MYF(MY_WME));
  dbc->port=port_nr;
  dbc->flag=flag_nr;
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : this function as its "normal" behavior is supposed to
	     bring up a dialog box if it isn't given enough information via
	     "szConnStrIn". If it is given enough information, it's supposed
	     to use "szConnStrIn" to  establish a database connection.
	     In either case, it returns a string to the user that is the
	     string that was eventually used to establish the connection
*/

#ifdef _UNIX_
static char *my_strtok(char *s1, pchar separator, char **save)
{
  reg1 char *rtnval,*end;

  rtnval = NULL;
  if (s1 != NULL)
    end = s1;
  else
    end= *save;
  if (end != NULL && *end != 0)
  {
    rtnval=end;
    do
    {
      if (*end++ == separator)
      {
	if (separator)
	  end[-1]=0;
	else
	  end--;      /* \0 as separator */
	break;
      }
    } while (*end != 0);
    *save=end;
  }
  return (rtnval);
}
#endif


/*
  @type    : ODBC 1.0 API
  @purpose : An alternative to SQLConnect. It supports data sources that
	     require more connection information than the three arguments in
	     SQLConnect, dialog boxes to prompt the user for all connection
	     information, and data sources that are not defined in the system
	     information.
*/

SQLRETURN SQL_API
my_SQLDriverConnect(SQLHDBC hdbc,SQLHWND hwnd,
		    SQLCHAR FAR *szConnStrIn,
		    SQLSMALLINT cbConnStrIn,
		    SQLCHAR FAR *szConnStrOut,
		    SQLSMALLINT cbConnStrOutMax,
		    SQLSMALLINT FAR *pcbConnStrOut,
		    SQLUSMALLINT fDriverCompletion)
{
#ifndef _UNIX_
  short   iRet;
  bool	  fPrompt = FALSE,maybe_prompt;
  DBC FAR *dbc=(DBC FAR*) hdbc;
  GLOBALHANDLE hglbAttr;
  LPSETUPDLG lpsetupdlg;
  ulong client_flag;
  DBUG_ENTER("SQLDriverConnect");
  DBUG_PRINT("enter",("fDriverCompletion: %d",fDriverCompletion));

  hglbAttr = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(SETUPDLG));
  if (!hglbAttr)
    DBUG_RETURN(FALSE);
  lpsetupdlg = (LPSETUPDLG)GlobalLock(hglbAttr);

  maybe_prompt=(fDriverCompletion == SQL_DRIVER_COMPLETE ||
		fDriverCompletion == SQL_DRIVER_COMPLETE_REQUIRED);
  if ((szConnStrIn == NULL) || (!cbConnStrIn) ||
      ((cbConnStrIn == SQL_NTS) && (!szConnStrIn[0])))
  {
    DBUG_PRINT("info",("No connection arguments"));
    fPrompt = TRUE;
  }
  else
  {
    ulong flag;
    /* Check connection string for completeness. Prompt if not all params */
    ParseAttributes(szConnStrIn, lpsetupdlg);
    set_attributes(lpsetupdlg);
    flag=(ulong) atol(lpsetupdlg->aAttr[KEY_FLAG].szAttr);
    if (fDriverCompletion == SQL_DRIVER_PROMPT && ! (flag & FLAG_NO_PROMPT) ||
	(maybe_prompt &&
	 (!lpsetupdlg->aAttr[KEY_SERVER].szAttr[0] ||
	  !lpsetupdlg->aAttr[KEY_USER].szAttr[0])))
    {
      DBUG_PRINT("info",("flag: %d  dsn:  '%s'  server: '%s'  user: '%s'",
			 flag,
			 lpsetupdlg->aAttr[KEY_DSN].szAttr,
			 lpsetupdlg->aAttr[KEY_SERVER].szAttr,
			 lpsetupdlg->aAttr[KEY_USER].szAttr));
      fPrompt = TRUE;
    }
  }
retry:

  if (fPrompt)
  {
    iRet = DialogBoxParam(s_hModule, MAKEINTRESOURCE(CONFIGDSN), hwnd,
			  (DLGPROC) DriverConnectProc,
			  (LONG)(LPSTR) lpsetupdlg);
    if ((!iRet) || (iRet == -1))
    {
      GlobalUnlock(hglbAttr);
      GlobalFree(hglbAttr);
      DBUG_RETURN(SQL_NO_DATA_FOUND);
    }
  }
  if (szConnStrOut)
  {
    char *pos,buff[1024];
    long length;
    ulong tmp= ~0L;
    char dsntmp[255];

    if(lpsetupdlg->aAttr[KEY_DSN].szAttr[0])
      strxmov(dsntmp, "DSN=", lpsetupdlg->aAttr[KEY_DSN].szAttr, NullS);
    else
      strxmov(dsntmp, "DRIVER={",DRIVER_NAME,"}", NullS);

    pos=strxmov(buff, dsntmp,
                ";DESC=",lpsetupdlg->aAttr[KEY_DESC].szAttr,
                ";DATABASE=",lpsetupdlg->aAttr[KEY_DB].szAttr,
                ";SERVER=",lpsetupdlg->aAttr[KEY_SERVER].szAttr,
                ";UID=",lpsetupdlg->aAttr[KEY_USER].szAttr,
                ";PASSWORD=",lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
                ";PORT=",lpsetupdlg->aAttr[KEY_PORT].szAttr,
                ";OPTION=",lpsetupdlg->aAttr[KEY_FLAG].szAttr,     
                ";STMT=",lpsetupdlg->aAttr[KEY_STMT].szAttr,
                ";",NullS);
    (void) copy_lresult((DBC FAR*) 0,szConnStrOut,cbConnStrOutMax,
                       &length,buff, (pos-buff),0L,0L,&tmp,0);
    if (pcbConnStrOut)
      *pcbConnStrOut=(SQLSMALLINT) length;
    DBUG_PRINT("info",("Connect string out: %s",szConnStrOut));
  }
  dbc->port=atoi(lpsetupdlg->aAttr[KEY_PORT].szAttr);
  dbc->flag=(uint) atol(lpsetupdlg->aAttr[KEY_FLAG].szAttr);
  client_flag=get_client_flag(&dbc->mysql,dbc->flag,(uint) dbc->login_timeout,
			      lpsetupdlg->aAttr[KEY_STMT].szAttr);

  /* If no DB, use DSN as database name */
  if (!lpsetupdlg->aAttr[KEY_DB].szAttr[0])
  {
    strmov(lpsetupdlg->aAttr[KEY_DB].szAttr,
	   lpsetupdlg->aAttr[KEY_DSN].szAttr);
  }
  if (!mysql_real_connect(&dbc->mysql,lpsetupdlg->aAttr[KEY_SERVER].szAttr,
			  lpsetupdlg->aAttr[KEY_USER].szAttr,
			  (lpsetupdlg->aAttr[KEY_PASSWORD].szAttr[0] ?
			   lpsetupdlg->aAttr[KEY_PASSWORD].szAttr : NullS),
			  lpsetupdlg->aAttr[KEY_DB].szAttr,
			  dbc->port,NullS,(uint) client_flag))
  {
    if (maybe_prompt && !fPrompt)
    {
      char message[256];
      fPrompt=TRUE;
      strxmov(message,"[MySQL][",ODBC_DRIVER,"] ",
	       mysql_error(&dbc->mysql), NullS);
      MessageBox(hwnd,message,"MyODBC Connection",MB_ICONERROR | MB_OK);
      goto retry;
    }
    set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_S1000,mysql_error(&dbc->mysql),
		     mysql_errno(&dbc->mysql));
    GlobalUnlock(hglbAttr);
    GlobalFree(hglbAttr);
    DBUG_RETURN(SQL_ERROR);
  }
  dbc->dsn=my_strdup(lpsetupdlg->aAttr[KEY_DSN].szAttr,MYF(MY_WME));
  dbc->database=my_strdup(lpsetupdlg->aAttr[KEY_DB].szAttr,MYF(MY_WME));
  dbc->server=my_strdup(lpsetupdlg->aAttr[KEY_SERVER].szAttr,MYF(MY_WME));
  dbc->user=my_strdup(lpsetupdlg->aAttr[KEY_USER].szAttr,MYF(MY_WME));
  dbc->password=my_strdup(lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,MYF(MY_WME));
  GlobalUnlock(hglbAttr);
  GlobalFree(hglbAttr);
  DBUG_RETURN(SQL_SUCCESS);
#endif /* IS NOT UNIX */

#ifdef _UNIX_
  SQLCHAR FAR *connstrin=NULL;
  char *KEY_DSN=NULL;
  char *KEY_SERVER=NULL;
  char *KEY_USER=NULL;
  char *KEY_PASSWORD=NULL;
  char *KEY_PORT=NULL;
  char *KEY_OPTION=NULL;
  char *KEY_DB=NULL;
  char *KEY_SOCKET=NULL;
  char *KEY_STMT=0;

  char *tok[9];
  char *tok2=NULL;
  char *tok3=NULL;
  int i;
  uint client_flag=0;

  char host[64],user[64],passwd[64],dsn[NAME_LEN+1],database[NAME_LEN+1],
    port[10],flag[10],socket[256],init_stmt[256],option[64];

  bool	  fPrompt = FALSE;
  DBC FAR *dbc=(DBC FAR*) hdbc;
  DBUG_ENTER("SQLDriverConnect");
  DBUG_PRINT("enter",("fDriverCompletion: %d",fDriverCompletion));

  if ((szConnStrIn == NULL) || (!cbConnStrIn) ||
      ((cbConnStrIn == SQL_NTS) && (!szConnStrIn[0])))
    fPrompt = TRUE;
  else
  {
    char *strtok_save;
    /* Check connection string for completeness. */
    /* Prompt if not all params      */

    connstrin=(SQLCHAR FAR *) my_strdup(szConnStrIn,MYF(MY_WME));

    tok[0]=my_strtok(connstrin,';', &strtok_save);
    for (i=1 ; i <= 7 ; i++)
      tok[i]=my_strtok(NULL,';',&strtok_save);
    tok[i]=NULL;

    for (i=0 ; tok[i] ; i++)
    {
      if ((tok2 = my_strtok(tok[i], '=', &strtok_save)))
      {
	if (strcasecmp("DSN", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0,&strtok_save)) && tok3[0])
	    KEY_DSN = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_DSN);
	    KEY_DSN = NULL;
	  }
	}

	if (strcasecmp("DB", tok2) == 0 ||
	    strcasecmp("DATABASE", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_DB = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_DB);
	    KEY_DB = NULL;
	  }
	}

	if (strcasecmp("UID", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_USER = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_USER);
	    KEY_USER = NULL;
	  }
        }

	if (strcasecmp("PWD", tok2) == 0 ||
	    strcasecmp("PASSWORD", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_PASSWORD = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_PASSWORD);
	    KEY_PASSWORD = NULL;
	  }
	}

	if (strcasecmp("SERVER", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_SERVER = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_SERVER);
	    KEY_SERVER = NULL;
	  }
	}

	if (strcasecmp("PORT", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_PORT = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_PORT);
	    KEY_PORT = NULL;
	  }
	}

	if (strcasecmp("SOCKET", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_SOCKET = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_SOCKET);
	    KEY_PORT = NULL;
	  }
	}

	if (strcasecmp("OPTION", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_OPTION = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_OPTION);
	    KEY_PORT = NULL;
	  }
	}

	if (strcasecmp("STMT", tok2) == 0)
	{
	  if ((tok3 = my_strtok(NULL, 0, &strtok_save)) && tok3[0])
	    KEY_STMT = (char*) my_strdup(tok3, MYF(MY_WME));
	  else
	  {
	    x_free(KEY_STMT);
	    KEY_STMT = NULL;
	  }
	}

      }
    }

    if (KEY_DSN)
    {
      SQLGetPrivateProfileString(KEY_DSN,"server",
				 "localhost", host, sizeof(host),"ODBC.INI");
      if (!KEY_SERVER) KEY_SERVER = (char*) my_strdup(host, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"database",
				 KEY_DSN, database, sizeof(database),
				 "ODBC.INI");
      if (!KEY_DB) KEY_DB = (char*) my_strdup(database, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"option",
				 "0", option, sizeof(option),"ODBC.INI");
      if (!KEY_OPTION) KEY_OPTION = (char*) my_strdup(option, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"port",
				 "3306", port, sizeof(port),"ODBC.INI");
      if (!KEY_PORT) KEY_PORT = (char*) my_strdup(port, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"stmt",
				 "", init_stmt, sizeof(init_stmt),"ODBC.INI");
      if (!KEY_STMT) KEY_STMT = (char*) my_strdup(init_stmt, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"user",
				 "dummy", user, sizeof(user),"ODBC.INI");
      if (!KEY_USER) KEY_USER = (char*) my_strdup(user, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"password",
				 "", passwd, sizeof(passwd),"ODBC.INI");
      if (!KEY_PASSWORD) KEY_PASSWORD = (char*) my_strdup(passwd, MYF(MY_WME));

      SQLGetPrivateProfileString(KEY_DSN,"socket",
				 "", socket, sizeof(socket),"ODBC.INI");
      if (!KEY_SOCKET) KEY_SOCKET = (char*) my_strdup(socket, MYF(MY_WME));
    }

    if (!KEY_DB && KEY_DSN)
      KEY_DB = (char*) my_strdup(KEY_DSN, MYF(MY_WME));

    if (!KEY_SERVER)
      KEY_SERVER = (char*) my_strdup("localhost", MYF(MY_WME));

    if (!KEY_PORT)
      KEY_PORT = (char*) my_strdup("3306", MYF(MY_WME));

    if (!KEY_SOCKET)
      KEY_SOCKET = (char*) my_strdup("", MYF(MY_WME));

    if (!KEY_USER)
      KEY_USER = (char*) my_strdup("dummy", MYF(MY_WME));

    if (!KEY_PASSWORD)
      KEY_PASSWORD = (char*) my_strdup("", MYF(MY_WME));

    if (!KEY_OPTION)
      KEY_OPTION = (char*) my_strdup("0", MYF(MY_WME));

    if (!KEY_STMT)
      KEY_STMT = (char*) my_strdup("", MYF(MY_WME));

    if (fDriverCompletion == SQL_DRIVER_PROMPT ||
	((fDriverCompletion == SQL_DRIVER_COMPLETE ||
	  fDriverCompletion == SQL_DRIVER_COMPLETE_REQUIRED) &&
	 (!KEY_DSN)))
      fPrompt = TRUE;
  }
  if (fPrompt)
  {
    DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_S1000,
				 "No DSN entered",0));
  }

  if (szConnStrOut)
  {
    SQLCHAR FAR  *pos;
    pos=strxmov(szConnStrOut,
		"DSN=",KEY_DSN,
		";DB=",KEY_DB,
		";SERVER=",KEY_SERVER,
		";UID=",KEY_USER,
		";PASSWORD=",KEY_PASSWORD,
		";PORT=",KEY_PORT,
		";SOCKET=",KEY_SOCKET,
		";OPTION=",KEY_OPTION,
		";STMT=",KEY_STMT,
		";",NullS);
    if (pcbConnStrOut)
      *pcbConnStrOut=(int) (pos-szConnStrOut);
  }
  dbc->flag=(uint) atol(KEY_OPTION);
  dbc->port=atoi(KEY_PORT);
  client_flag=get_client_flag(&dbc->mysql,dbc->flag,(uint) dbc->login_timeout,
			      KEY_STMT);

  if (!mysql_real_connect(&dbc->mysql,KEY_SERVER,
			  KEY_USER,
			  KEY_PASSWORD[0] ? KEY_PASSWORD : NullS,
			  KEY_DB,
			  dbc->port,
			  KEY_SOCKET[0] ? KEY_SOCKET : NullS,
			  (uint) client_flag))
  {
    DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_S1000,
				 mysql_error(&dbc->mysql),
				 mysql_errno(&dbc->mysql)));
  }
  dbc->dsn=my_strdup(KEY_DSN,MYF(MY_WME));
  dbc->database=my_strdup(KEY_DB,MYF(MY_WME));
  dbc->server=my_strdup(KEY_SERVER,MYF(MY_WME));
  dbc->user=my_strdup(KEY_USER,MYF(MY_WME));
  dbc->password=my_strdup(KEY_PASSWORD,MYF(MY_WME));

  x_free(KEY_DSN);
  x_free(KEY_DB);
  x_free(KEY_SERVER);
  x_free(KEY_USER);
  x_free(KEY_PASSWORD);
  x_free(KEY_PORT);
  x_free(KEY_SOCKET);
  x_free(KEY_OPTION);
  x_free(KEY_STMT);
  x_free(connstrin);

  DBUG_RETURN(SQL_SUCCESS);
#endif /* IS UNIX */
}


/*
  @type    : ODBC 1.0 API
  @purpose : is an alternative to SQLConnect. It supports data sources that
	     require more connection information than the three arguments in
	     SQLConnect, dialog boxes to prompt the user for all connection
	     information, and data sources that are not defined in the
	     system information.
*/

SQLRETURN SQL_API
SQLDriverConnect(SQLHDBC hdbc,SQLHWND hwnd,
		 SQLCHAR FAR *szConnStrIn,
		 SQLSMALLINT cbConnStrIn,
		 SQLCHAR FAR *szConnStrOut,
		 SQLSMALLINT cbConnStrOutMax,
		 SQLSMALLINT FAR *pcbConnStrOut,
		 SQLUSMALLINT fDriverCompletion)
{
  return my_SQLDriverConnect(hdbc,hwnd,szConnStrIn,cbConnStrIn,
			     szConnStrOut,cbConnStrOutMax,
			     pcbConnStrOut,fDriverCompletion);
}


/*
  @type    : ODBC 1.0 API
  @purpose : supports an iterative method of discovering and enumerating
	     the attributes and attribute values required to connect to a
	     data source
*/

SQLRETURN SQL_API
SQLBrowseConnect(SQLHDBC hdbc,SQLCHAR FAR *szConnStrIn,
		 SQLSMALLINT cbConnStrIn,
		 SQLCHAR FAR *szConnStrOut,
		 SQLSMALLINT cbConnStrOutMax,
		 SQLSMALLINT FAR *pcbConnStrOut)
{
  DBUG_ENTER("SQLBrowseConnect");
  DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_S1000,
			       "Driver Does not support this API",0));
}


/*
  @type    : myodbc3 internal
  @purpose : closes the connection associated with a specific connection handle
*/

SQLRETURN SQL_API my_SQLDisconnect(SQLHDBC hdbc)
{
  LIST *list_element,*next_element;
  DBC FAR *dbc=(DBC FAR*) hdbc;
  DBUG_ENTER("my_SQLDisconnect");

  for (list_element=dbc->statements ; list_element ;
       list_element=next_element)
  {
    next_element=list_element->next;
    my_SQLFreeStmt((SQLHSTMT) list_element->data, SQL_DROP);
  }
  mysql_close(&dbc->mysql);
  my_free(dbc->dsn,MYF(0));
  my_free(dbc->database,MYF(0));
  my_free(dbc->server,MYF(0));
  my_free(dbc->user,MYF(0));
  my_free(dbc->password,MYF(0));
  dbc->dsn=dbc->database=dbc->server=dbc->user=dbc->password=0;
  DBUG_RETURN(SQL_SUCCESS);
}


/*
  @type    : ODBC 1.0 API
  @purpose : closes the connection associated with a specific connection handle
*/

SQLRETURN SQL_API SQLDisconnect(SQLHDBC hdbc)
{
  return my_SQLDisconnect(hdbc);
}
