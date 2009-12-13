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
 * SETUP.C                                                                 *
 *                                                                         *
 * @description: Sample myodbc3 setup                                      *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

#include  "myodbc3.h"
#undef VOID         /* Becasue of ctl3d.h */
#define VOID void
#include <ctl3d.h>
#include "odbcinst.h"
#include <string.h>
#include "dialogs.h"
#include <winuser.h>

/* Constants */
const char EMPTYSTR  []= "";
const char OPTIONON  []= "Yes";
const char OPTIONOFF []= "No";

/* ODBC.INI keywords */
const char ODBC_INI    []="ODBC.INI";   /* ODBC initialization file */
const char INI_KDESC   []="Description";  /* Data source description */
const char INI_KDB     []="Database";
const char INI_KSERVER []="Server";
const char INI_KUSER   []="User";   /* Second option */
const char INI_KPASSWORD[] = "Password";
const char INI_KPORT   []  = "Port";
const char INI_KFLAG   []  = "Option";
const char INI_KSTMT   []  = "Stmt";
const char INI_SDEFAULT[]= "Default"; 
const char szTranslateName[] = "TranslationName";
const char szTranslateDLL[] = "TranslationDLL";
const char szTranslateOption[] = "TranslationOption";
const char szUnkTrans[] = "Unknown Translator";

/* Attribute string look-up table (maps keys to associated indexes) */
static struct {
  char  szKey[MAXKEYLEN];
  int  iKey;
} s_aLookup[] = { "DSN",  KEY_DSN,
      "DESC", KEY_DESC,
      "Description",KEY_DESC,
      "Database", KEY_DB,
      "DB",   KEY_DB,
      "Server", KEY_SERVER,
      "UID",  KEY_USER,
      "User", KEY_USER,
      "Password", KEY_PASSWORD,
      "PWD",  KEY_PASSWORD,
      "Port", KEY_PORT,
      "Option", KEY_FLAG,
      "",0
};

/*
  @type    : myodbc3 internal
  @purpose : sets the connection dialog attributes
*/
void set_attributes(LPSETUPDLG lpsetupdlg)
{
  LPCSTR lpszDSN=lpsetupdlg->aAttr[KEY_DSN].szAttr;

  if (!lpsetupdlg->aAttr[KEY_DESC].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KDESC,
       EMPTYSTR,
       lpsetupdlg->aAttr[KEY_DESC].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_DESC].szAttr),
       ODBC_INI);
  if (!lpsetupdlg->aAttr[KEY_DB].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KDB,
       lpszDSN,
       lpsetupdlg->aAttr[KEY_DB].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_DB].szAttr),
       ODBC_INI);
  if (!lpsetupdlg->aAttr[KEY_SERVER].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KSERVER,
       EMPTYSTR,
       lpsetupdlg->aAttr[KEY_SERVER].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_SERVER].szAttr),
       ODBC_INI);
  if (!lpsetupdlg->aAttr[KEY_USER].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KUSER,
       EMPTYSTR,
       lpsetupdlg->aAttr[KEY_USER].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_USER].szAttr),
       ODBC_INI);
  if (!lpsetupdlg->aAttr[KEY_PASSWORD].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KPASSWORD,
       EMPTYSTR,
       lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_PASSWORD].
        szAttr),
       ODBC_INI);
  if (!lpsetupdlg->aAttr[KEY_PORT].fSupplied)
  {
    char buff[10];
    int2str(MYSQL_PORT,buff,10);
    SQLGetPrivateProfileString(lpszDSN, INI_KPORT,
       buff,
       lpsetupdlg->aAttr[KEY_PORT].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_PORT].
        szAttr),
       ODBC_INI);
  }
  if (!lpsetupdlg->aAttr[KEY_FLAG].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KFLAG,
       "0",
       lpsetupdlg->aAttr[KEY_FLAG].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_FLAG].
        szAttr),
       ODBC_INI);
  if (!lpsetupdlg->aAttr[KEY_STMT].fSupplied)
    SQLGetPrivateProfileString(lpszDSN, INI_KSTMT,
       EMPTYSTR,
       lpsetupdlg->aAttr[KEY_STMT].szAttr,
       sizeof(lpsetupdlg->aAttr[KEY_STMT].
        szAttr),
       ODBC_INI);
  
  /* set the default configuration for new DSN */
  if(lpsetupdlg->fNewDSN)
  {	
	strcpy(lpsetupdlg->aAttr[KEY_DESC].szAttr,
		   "MySQL ODBC 3.51 Driver DSN");			
	strcpy(lpsetupdlg->aAttr[KEY_DB].szAttr,
		   "test");		
	strcpy(lpsetupdlg->aAttr[KEY_SERVER].szAttr,
		   "localhost");		
	strcpy(lpsetupdlg->aAttr[KEY_PORT].szAttr,
		   "3306");		
	strcpy(lpsetupdlg->aAttr[KEY_FLAG].szAttr,"3");		
  }
}

/*
  @type    : myodbc3 internal
  @purpose : sets the connection dialog items
*/

void set_dlg_items(HWND hdlg ,LPSETUPDLG lpsetupdlg)
{
  uint i;
  ulong flag;
  set_attributes(lpsetupdlg);  
  SetDlgItemText(hdlg, IDC_DSNAME,  lpsetupdlg->aAttr[KEY_DSN].szAttr);        
  SetDlgItemText(hdlg, IDC_DESC,  lpsetupdlg->aAttr[KEY_DESC].szAttr);
  SetDlgItemText(hdlg, IDC_DB,    lpsetupdlg->aAttr[KEY_DB].szAttr);
  SetDlgItemText(hdlg, IDC_SERVER,  lpsetupdlg->aAttr[KEY_SERVER].szAttr);
  SetDlgItemText(hdlg, IDC_USER,  lpsetupdlg->aAttr[KEY_USER].szAttr);
  SetDlgItemText(hdlg, IDC_PASSWORD,  lpsetupdlg->aAttr[KEY_PASSWORD].szAttr);
  SetDlgItemText(hdlg, IDC_PORT,  lpsetupdlg->aAttr[KEY_PORT].szAttr);
  SetDlgItemText(hdlg, IDC_STMT,  lpsetupdlg->aAttr[KEY_STMT].szAttr);
  flag=(ulong) atol(lpsetupdlg->aAttr[KEY_FLAG].szAttr);
#ifdef DBUG_OFF
  flag&= ~FLAG_DEBUG;
#endif
  for (i=0 ; i < NUMOFBITS ; i++)
    CheckDlgButton(hdlg, IDC_CHECK1+i,
       flag & (1 << i) ? BST_CHECKED : BST_UNCHECKED);
}


static void INTFUNC CenterDialog (HWND   hdlg);
int  CALLBACK ConfigDlgProc (HWND hdlg,WORD wMsg,WPARAM wParam, LPARAM lParam);
int CALLBACK SetDSNAttributes(HWND hwnd, LPSETUPDLG lpsetupdlg);

/*
  @type    : ODBC 1.0 API
  @purpose : adds, modifies, or deletes data sources from the system
       information. It may prompt the user for connection
       information. It can be in the driver DLL or a separate
       setup DLL
*/

int EXPFUNC
ConfigDSN (HWND  hwnd,
    WORD  fRequest,
    LPCSTR  lpszDriver,
    LPCSTR  lpszAttributes)
{
  bool  fSuccess; /* Success/fail flag */
  GLOBALHANDLE hglbAttr;
  LPSETUPDLG lpsetupdlg;

  /* Allocate attribute array */
  hglbAttr = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(SETUPDLG));
  if (!hglbAttr)
    return FALSE;
  lpsetupdlg = (LPSETUPDLG)GlobalLock(hglbAttr);

  /* Parse attribute string */
  if (lpszAttributes)
    ParseAttributes(lpszAttributes, lpsetupdlg);

  /* Save original data source name */
  if (lpsetupdlg->aAttr[KEY_DSN].fSupplied)
    lstrcpy(lpsetupdlg->szDSN, lpsetupdlg->aAttr[KEY_DSN].szAttr);
  else
    lpsetupdlg->szDSN[0] = '\0';

  /* Remove data source */
  if (ODBC_REMOVE_DSN == fRequest) {
    /* Fail if no data source name was supplied */
    if (!lpsetupdlg->aAttr[KEY_DSN].fSupplied)
      fSuccess = FALSE;

    /* Otherwise remove data source from ODBC.INI */
    else
      fSuccess = SQLRemoveDSNFromIni(lpsetupdlg->aAttr[KEY_DSN].szAttr);
  }

  /* Add or Configure data source */
  else
  {
    /* Save passed variables for global access (e.g., dialog access) */
    lpsetupdlg->hwndParent = hwnd;
    lpsetupdlg->lpszDrvr   = lpszDriver;
    lpsetupdlg->fNewDSN    = (ODBC_ADD_DSN == fRequest);
    lpsetupdlg->fDefault   = !lstrcmpi(lpsetupdlg->aAttr[KEY_DSN].szAttr,
               INI_SDEFAULT);

    /* Display the appropriate dialog (if parent window handle supplied) */
    if (hwnd)
    {
      /* Display dialog(s) */
      fSuccess = (IDOK == DialogBoxParam(s_hModule,
           MAKEINTRESOURCE(CONFIGDSN),
           hwnd,
           (DLGPROC) ConfigDlgProc,
           (LONG)(LPSTR)lpsetupdlg));
    }

    else if (lpsetupdlg->aAttr[KEY_DSN].fSupplied)
      fSuccess = SetDSNAttributes(hwnd, lpsetupdlg);
    else
      fSuccess = FALSE;
  }

  GlobalUnlock(hglbAttr);
  GlobalFree(hglbAttr);
  return fSuccess;
}

/*
  @type    : internal
  @purpose : Center the dialog over the frame window
*/

static void INTFUNC CenterDialog(HWND hdlg)
{
  HWND  hwndFrame;
  RECT  rcDlg, rcScr, rcFrame;
  int cx, cy;

  hwndFrame = GetParent(hdlg);

  GetWindowRect(hdlg, &rcDlg);
  cx = rcDlg.right  - rcDlg.left;
  cy = rcDlg.bottom - rcDlg.top;

  GetClientRect(hwndFrame, &rcFrame);
  ClientToScreen(hwndFrame, (LPPOINT)(&rcFrame.left));
  ClientToScreen(hwndFrame, (LPPOINT)(&rcFrame.right));
  rcDlg.top    = rcFrame.top  + (((rcFrame.bottom - rcFrame.top) - cy) >> 1);
  rcDlg.left   = rcFrame.left + (((rcFrame.right - rcFrame.left) - cx) >> 1);
  rcDlg.bottom = rcDlg.top  + cy;
  rcDlg.right  = rcDlg.left + cx;

  GetWindowRect(GetDesktopWindow(), &rcScr);
  if (rcDlg.bottom > rcScr.bottom)
  {
    rcDlg.bottom = rcScr.bottom;
    rcDlg.top  = rcDlg.bottom - cy;
  }
  if (rcDlg.right  > rcScr.right)
  {
    rcDlg.right = rcScr.right;
    rcDlg.left  = rcDlg.right - cx;
  }

  if (rcDlg.left < 0) rcDlg.left = 0;
  if (rcDlg.top  < 0) rcDlg.top  = 0;

  MoveWindow(hdlg, rcDlg.left, rcDlg.top, cx, cy, TRUE);
  return;
}

/*
  @type    : myodbc3 internal
  @purpose : test DSN configuration parameters
*/

int my_test_datasource(HWND hdlg,LPSETUPDLG lpsetupdlg)
{
  SQLRETURN rc=SQL_ERROR;
  SQLHENV   henv=NULL;
  SQLHDBC   hdbc=NULL;
  char      message[255];

  
  if (!(my_SQLAllocEnv(&henv)) &&  
      !(my_SQLAllocConnect(henv,&hdbc))) 
  {  
    char connection[255];
      
    strxmov(connection,
            "DSN=",lpsetupdlg->aAttr[KEY_DSN].szAttr,
            ";SERVER=",lpsetupdlg->aAttr[KEY_SERVER].szAttr,
            ";DB=",lpsetupdlg->aAttr[KEY_DB].szAttr,
            ";USER=",lpsetupdlg->aAttr[KEY_USER].szAttr,
            ";PASSWORD=",lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
            ";PORT=",lpsetupdlg->aAttr[KEY_PORT].szAttr,";",NullS);
       
    rc = my_SQLDriverConnect(hdbc,hdlg,connection,255,
                             NULL,0,NULL,SQL_DRIVER_NOPROMPT);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
    {
      strxmov(message,"MySQL ODBC 3.51 Driver : ",
              DRIVER_VERSION, "\n\nDataSource '",
              ((DBC FAR *)hdbc)->dsn, 
              "' Connected Successfully to 'mysqld-",
              ((DBC FAR *)hdbc)->mysql.server_version,
              "' !!!\n",NullS);
      my_SQLDisconnect(hdbc);
      my_SQLFreeConnect(hdbc);
      my_SQLFreeEnv(henv); 

      MessageBox(hdlg,message,"MyODBC 3.51 DataSource Test",
                 MB_ICONINFORMATION | MB_OK);
      return(0);
    }
  }
  if (hdbc)
  {
    strxmov(message,"MySQL ODBC 3.51 Driver : ",
            DRIVER_VERSION, "\n\n",
      ((DBC FAR *)hdbc)->error.message,"\n", NullS);

    my_SQLFreeConnect(hdbc);
  }
  else 
  {
    strxmov(message,"MySQL ODBC 3.51 Driver : ",
      DRIVER_VERSION,
      "\n\n"," Failed to allocate the handles\n", NullS);
  }
  henv ? my_SQLFreeEnv(henv) : 0; 
  MessageBox(hdlg,message,"MyODBC 3.51 DataSource Test",MB_ICONERROR | MB_OK);
  return(rc);
}

/*
  @type    : internal
  @purpose : Manage add data source name dialog
*/

int CALLBACK ConfigDlgProc (HWND hdlg,WORD wMsg,WPARAM wParam,LPARAM lParam)
{
  switch (wMsg) {
    /* Initialize the dialog */
  case WM_INITDIALOG:
  {
    LPSETUPDLG lpsetupdlg;

    Ctl3dRegister (s_hModule);
    SetWindowLong(hdlg, DWL_USER, lParam);
#ifdef _WIN32
    Ctl3dSubclassDlg(hdlg, CTL3D_ALL);
#else
    Ctl3dSubclassDlgEx(hdlg, CTL3D_ALL);
#endif
    CenterDialog(hdlg);

    lpsetupdlg = (LPSETUPDLG) lParam;
    set_attributes(lpsetupdlg);
    set_dlg_items(hdlg,lpsetupdlg);

    if (lpsetupdlg->fDefault)
    {
      EnableWindow(GetDlgItem(hdlg, IDC_DSNAME), FALSE);
      EnableWindow(GetDlgItem(hdlg, IDC_DSNAMETEXT), FALSE);
    }
    else
    {
      SendDlgItemMessage(hdlg, IDC_DSNAME,
       EM_LIMITTEXT, (WPARAM)(MAXDSNAME-1), 0L);
      SendDlgItemMessage(hdlg, IDC_DESC,
       EM_LIMITTEXT, (WPARAM)(MAXDESC-1), 0L);
      SendDlgItemMessage(hdlg, IDC_DB,
       EM_LIMITTEXT, (WORD)(NAME_LEN), 0L);
      SendDlgItemMessage(hdlg, IDC_SERVER,
       EM_LIMITTEXT, (WORD)(MAXSERVER-1), 0L);
      SendDlgItemMessage(hdlg, IDC_USER,
       EM_LIMITTEXT, (WORD)(MAXUSER-1), 0L);
      SendDlgItemMessage(hdlg, IDC_PASSWORD,
       EM_LIMITTEXT, (WORD)(MAXPASSWORD-1), 0L);
      SendDlgItemMessage(hdlg, IDC_PORT,
       EM_LIMITTEXT, (WORD) 5, 0L);
      SendDlgItemMessage(hdlg, IDC_STMT,
       EM_LIMITTEXT, (WORD) (MAXSTMT-1), 0L);
#if defined(DBUG_OFF) && defined(NOT_YET)
      /* HELP: How can one disable this button ? */
      SendDlgItemMessage(hdlg, IDC_CHECK3,
       EM_SETREADONLY, (WORD) TRUE, 0L);
#endif
    }
    return TRUE;  /* Focus was not set */
  }

#ifdef _WIN32
  case WM_CTLCOLORBTN:
  case WM_CTLCOLORDLG:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLORMSGBOX:
  case WM_CTLCOLORSCROLLBAR:
  case WM_CTLCOLORSTATIC:
    return (int) Ctl3dCtlColorEx(wMsg, wParam, lParam);

  case WM_SETTEXT:
  case WM_NCPAINT:
  case WM_NCACTIVATE:
    SetWindowLong(hdlg, DWL_MSGRESULT,
      Ctl3dDlgFramePaint(hdlg, wMsg, wParam, lParam));
    return TRUE;
#endif

  case WM_SYSCOLORCHANGE:
    return Ctl3dColorChange();

    /* Process buttons */
  case WM_COMMAND:
    switch (GET_WM_COMMAND_ID(wParam, lParam)) {
      /*
  Ensure the OK button is enabled only when a data source name
  is entered
      */
    case IDC_DSNAME:
      if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
      {
  char  szItem[MAXDSNAME];    /* Edit control text */

  /* Enable/disable the OK button */
  EnableWindow(GetDlgItem(hdlg, IDOK),
         GetDlgItemText(hdlg, IDC_DSNAME,
            szItem, sizeof(szItem)));
  return TRUE;
      }
      break;

    case IDHELP:
    {
      HKEY hKey;
      char szHelpFileName[255];
      DWORD dwBufLen;

      RegOpenKeyEx( HKEY_LOCAL_MACHINE,
             "SOFTWARE\\ODBC\\ODBCINST.INI\\MySQL ODBC 3.51 Driver",
             0, KEY_QUERY_VALUE, &hKey );
      RegQueryValueEx( hKey, "DSNHelpFile", NULL, NULL,
             (LPBYTE) szHelpFileName, &dwBufLen);
      RegCloseKey( hKey );

      WinHelp(hdlg, szHelpFileName, HELP_INDEX, 0L);
    }
    break;

    case IDTEST:
    {
      LPSETUPDLG lpsetupdlg;

      lpsetupdlg = (LPSETUPDLG)GetWindowLong(hdlg, DWL_USER);

      /* Retrieve dialog values */
      if (!lpsetupdlg->fDefault)
      GetDlgItemText(hdlg, IDC_DSNAME,
           lpsetupdlg->aAttr[KEY_DSN].szAttr,
           sizeof(lpsetupdlg->aAttr[KEY_DSN].szAttr));
      GetDlgItemText(hdlg, IDC_DB,
         lpsetupdlg->aAttr[KEY_DB].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_DB].szAttr));
      GetDlgItemText(hdlg, IDC_SERVER,
         lpsetupdlg->aAttr[KEY_SERVER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_SERVER].szAttr));
      GetDlgItemText(hdlg, IDC_USER,
         lpsetupdlg->aAttr[KEY_USER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_USER].szAttr));
      GetDlgItemText(hdlg, IDC_PASSWORD,
         lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PASSWORD].szAttr));
      GetDlgItemText(hdlg, IDC_PORT,
         lpsetupdlg->aAttr[KEY_PORT].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PORT].szAttr));

      my_test_datasource(hdlg,lpsetupdlg);
      break;
    }

    /* Accept results */
    case IDOK:
    {
      LPSETUPDLG lpsetupdlg;
      ulong flag;
      uint i;

      lpsetupdlg = (LPSETUPDLG)GetWindowLong(hdlg, DWL_USER);

      /* Retrieve dialog values */
      if (!lpsetupdlg->fDefault)
      GetDlgItemText(hdlg, IDC_DSNAME,
           lpsetupdlg->aAttr[KEY_DSN].szAttr,
           sizeof(lpsetupdlg->aAttr[KEY_DSN].szAttr));
      GetDlgItemText(hdlg, IDC_DESC,
         lpsetupdlg->aAttr[KEY_DESC].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_DESC].szAttr));
      GetDlgItemText(hdlg, IDC_DB,
         lpsetupdlg->aAttr[KEY_DB].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_DB].szAttr));
      GetDlgItemText(hdlg, IDC_SERVER,
         lpsetupdlg->aAttr[KEY_SERVER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_SERVER].szAttr));
      GetDlgItemText(hdlg, IDC_USER,
         lpsetupdlg->aAttr[KEY_USER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_USER].szAttr));
      GetDlgItemText(hdlg, IDC_PASSWORD,
         lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PASSWORD].szAttr));
      GetDlgItemText(hdlg, IDC_PORT,
         lpsetupdlg->aAttr[KEY_PORT].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PORT].szAttr));
      GetDlgItemText(hdlg, IDC_STMT,
         lpsetupdlg->aAttr[KEY_STMT].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_STMT].szAttr));

      flag=0;
      for (i=0 ; i < NUMOFBITS ; i++)
  if (IsDlgButtonChecked(hdlg, IDC_CHECK1+i) == BST_CHECKED)
    flag|= 1 << i;
      sprintf(lpsetupdlg->aAttr[KEY_FLAG].szAttr,"%lu",flag);

      SetDSNAttributes(hdlg, lpsetupdlg);
    }

    /* Return to caller */
    case IDCANCEL:
      Ctl3dUnregister (s_hModule);
      EndDialog(hdlg, wParam);
      return TRUE;
    }

    break;
  }

  /* Message not processed */
  return FALSE;
}

/*
  @type    : internal
  @purpose : Parse attribute string moving values into the aAttr array
*/

void INTFUNC ParseAttributes(LPCSTR lpszAttributes, LPSETUPDLG lpsetupdlg)
{
  LPCSTR  lpsz;
  LPCSTR  lpszStart;
  char  aszKey[MAXKEYLEN];
  int iElement;
  int cbKey;
  DBUG_ENTER("ParseAttributes");
  DBUG_PRINT("enter",("attributes: %s",lpszAttributes));

  for (lpsz=lpszAttributes; *lpsz;)
  {
    /*  Extract key name (e.g., DSN), it must be terminated by an equals */
    lpszStart = lpsz;
    if (!(lpsz=strchr(lpsz,'=')))
      DBUG_VOID_RETURN;
    /* Determine the keys index in the key table (-1 if not found) */
    iElement = -1;
    cbKey  = lpsz - lpszStart;
    if (cbKey < sizeof(aszKey))
    {
      register int j;

      _fmemcpy(aszKey, lpszStart, cbKey);
      aszKey[cbKey] = '\0';
      for (j = 0; *s_aLookup[j].szKey; j++)
      {
  if (!lstrcmpi(s_aLookup[j].szKey, aszKey))
  {
    iElement = s_aLookup[j].iKey;
    break;
  }
      }
    }

    /* Locate end of key value */
    lpszStart = ++lpsz;
    lpsz=strcend(lpsz,';');

    /*
      Save value if key is known
      NOTE: This code assumes the szAttr buffers in aAttr have been
      zero initialized
    */
    if (iElement >= 0)
    {
      lpsetupdlg->aAttr[iElement].fSupplied = TRUE;
      strmake(lpsetupdlg->aAttr[iElement].szAttr,lpszStart,
        min(lpsz-lpszStart, sizeof(lpsetupdlg->aAttr[0].szAttr)-1));
    }
    lpsz++;     /* Skipp ';' or end 0 */
  }
  DBUG_VOID_RETURN;
}

/*
  @type    : internal
  @purpose : Write data source attributes to ODBC.INI
*/

int INTFUNC SetDSNAttributes(HWND hwndParent, LPSETUPDLG lpsetupdlg)
{
  LPCSTR lpszDSN=lpsetupdlg->aAttr[KEY_DSN].szAttr;  /* Pointer to DSN */

  /* Validate arguments */
  if (lpsetupdlg->fNewDSN && !*lpsetupdlg->aAttr[KEY_DSN].szAttr)
    return FALSE;

  /* Write the data source name */
  if (!SQLWriteDSNToIni(lpszDSN, lpsetupdlg->lpszDrvr))
  {
    if (hwndParent)
    {
      char  szBuf[MAXPATHLEN];
      char  szMsg[MAXPATHLEN];

      LoadString(s_hModule, IDS_BADDSN, szBuf, sizeof(szBuf));
      wsprintf(szMsg, szBuf, lpszDSN);
      LoadString(s_hModule, IDS_MSGTITLE, szBuf, sizeof(szBuf));
      MessageBox(hwndParent, szMsg, szBuf, MB_ICONEXCLAMATION | MB_OK);
    }
    return FALSE;
  }

  /*
    Update ODBC.INI
    Save the value if the data source is new, if it was edited, or if
    it was explicitly supplied
  */
  if (hwndParent || lpsetupdlg->aAttr[KEY_DESC].fSupplied )
    SQLWritePrivateProfileString(lpszDSN,
         INI_KDESC,
         lpsetupdlg->aAttr[KEY_DESC].szAttr,
         ODBC_INI);
  if (hwndParent || lpsetupdlg->aAttr[KEY_DB].fSupplied )
    SQLWritePrivateProfileString(lpszDSN,
         INI_KDB,
         lpsetupdlg->aAttr[KEY_DB].szAttr,
         ODBC_INI);
  if (hwndParent || lpsetupdlg->aAttr[KEY_SERVER].fSupplied )
    SQLWritePrivateProfileString(lpszDSN,
         INI_KSERVER,
         lpsetupdlg->aAttr[KEY_SERVER].szAttr,
         ODBC_INI);
  if (hwndParent || lpsetupdlg->aAttr[KEY_USER].fSupplied )
    SQLWritePrivateProfileString(lpszDSN,
         INI_KUSER,
         lpsetupdlg->aAttr[KEY_USER].szAttr,
         ODBC_INI);
  if (hwndParent || lpsetupdlg->aAttr[KEY_PASSWORD].fSupplied )
    SQLWritePrivateProfileString(lpszDSN,
         INI_KPASSWORD,
         lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
         ODBC_INI);
  if (hwndParent || lpsetupdlg->aAttr[KEY_PORT].fSupplied )
  {
    SQLWritePrivateProfileString(lpszDSN,
         INI_KPORT,
         lpsetupdlg->aAttr[KEY_PORT].szAttr,
         ODBC_INI);
  }
  if (hwndParent || lpsetupdlg->aAttr[KEY_FLAG].fSupplied )
  {
    SQLWritePrivateProfileString(lpszDSN,
         INI_KFLAG,
         lpsetupdlg->aAttr[KEY_FLAG].szAttr,
         ODBC_INI);
  }
  if (hwndParent || lpsetupdlg->aAttr[KEY_STMT].fSupplied )
  {
    SQLWritePrivateProfileString(lpszDSN,
         INI_KSTMT,
         lpsetupdlg->aAttr[KEY_STMT].szAttr,
         ODBC_INI);
  }

  /* If the data source name has changed, remove the old name */
  if (lpsetupdlg->aAttr[KEY_DSN].fSupplied &&
      lstrcmpi(lpsetupdlg->szDSN, lpsetupdlg->aAttr[KEY_DSN].szAttr))
  {
    SQLRemoveDSNFromIni(lpsetupdlg->szDSN);
  }
  return TRUE;
}

/*
  @type    : internal
  @purpose : Connect dialog for driver connect
*/

int FAR PASCAL
DriverConnectProc(HWND hdlg, WORD wMsg, WPARAM wParam, LPARAM lParam)
{
  LPSETUPDLG lpsetupdlg;
  switch (wMsg) {
  case WM_INITDIALOG:
    Ctl3dRegister (s_hModule);
    SetWindowLong(hdlg, DWL_USER, lParam);
#ifdef _WIN32
    Ctl3dSubclassDlg(hdlg, CTL3D_ALL);
#else
    Ctl3dSubclassDlgEx(hdlg, CTL3D_ALL);
#endif
    CenterDialog(hdlg);
    lpsetupdlg = (LPSETUPDLG) lParam;
    set_dlg_items(hdlg,lpsetupdlg);    

    SendDlgItemMessage(hdlg, IDC_DSNAME,
           EM_LIMITTEXT, (WPARAM)(MAXDSNAME-1), 0L);
    SendDlgItemMessage(hdlg, IDC_DESC,
           EM_LIMITTEXT, (WPARAM)(MAXDESC-1), 0L);
    SendDlgItemMessage(hdlg, IDC_DB,
           EM_LIMITTEXT, (WORD)(NAME_LEN), 0L);
    SendDlgItemMessage(hdlg, IDC_SERVER,
           EM_LIMITTEXT, (WORD)(MAXSERVER-1), 0L);
    SendDlgItemMessage(hdlg, IDC_USER,
           EM_LIMITTEXT, (WORD)(MAXUSER-1), 0L);
    SendDlgItemMessage(hdlg, IDC_PASSWORD,
           EM_LIMITTEXT, (WORD)(MAXPASSWORD-1), 0L);
    SendDlgItemMessage(hdlg, IDC_PORT,
           EM_LIMITTEXT, (WORD)5, 0L);
    SendDlgItemMessage(hdlg, IDC_STMT,
           EM_LIMITTEXT, (WORD) (MAXSTMT-1), 0L);    
    /* 
       Disable DSN change for DriverConnect related ...
       so that the FileDSN will be validated correctly, 
       as user can't set/change the DSN 
    */
    EnableWindow(GetDlgItem(hdlg, IDC_DSNAME), FALSE);
    return TRUE;

#ifdef _WIN32
  case WM_CTLCOLORBTN:
  case WM_CTLCOLORDLG:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLORMSGBOX:
  case WM_CTLCOLORSCROLLBAR:
  case WM_CTLCOLORSTATIC:
    return (int)Ctl3dCtlColorEx(wMsg, wParam, lParam);

  case WM_SETTEXT:
  case WM_NCPAINT:
  case WM_NCACTIVATE:
    SetWindowLong(hdlg, DWL_MSGRESULT,
      Ctl3dDlgFramePaint(hdlg, wMsg, wParam, lParam));
    return TRUE;
#endif

  case WM_SYSCOLORCHANGE:
    return Ctl3dColorChange();

  case WM_COMMAND:
    switch (GET_WM_COMMAND_ID(wParam, lParam)) {

    case IDTEST:
    {
      LPSETUPDLG lpsetupdlg;

      lpsetupdlg = (LPSETUPDLG)GetWindowLong(hdlg, DWL_USER);

      /* Retrieve dialog values */
      if (!lpsetupdlg->fDefault)
      GetDlgItemText(hdlg, IDC_DSNAME,
           lpsetupdlg->aAttr[KEY_DSN].szAttr,
           sizeof(lpsetupdlg->aAttr[KEY_DSN].szAttr));
      GetDlgItemText(hdlg, IDC_DB,
         lpsetupdlg->aAttr[KEY_DB].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_DB].szAttr));
      GetDlgItemText(hdlg, IDC_SERVER,
         lpsetupdlg->aAttr[KEY_SERVER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_SERVER].szAttr));
      GetDlgItemText(hdlg, IDC_USER,
         lpsetupdlg->aAttr[KEY_USER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_USER].szAttr));
      GetDlgItemText(hdlg, IDC_PASSWORD,
         lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PASSWORD].szAttr));
      GetDlgItemText(hdlg, IDC_PORT,
         lpsetupdlg->aAttr[KEY_PORT].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PORT].szAttr));

      my_test_datasource(hdlg,lpsetupdlg);
      break;
    }

    case IDHELP:
    {
      HKEY hKey;
      char szHelpFileName[255];
      DWORD dwBufLen;

      RegOpenKeyEx( HKEY_LOCAL_MACHINE,
             "SOFTWARE\\ODBC\\ODBCINST.INI\\MySQL ODBC 3.51 Driver",
             0, KEY_QUERY_VALUE, &hKey );
      RegQueryValueEx( hKey, "DSNHelpFile", NULL, NULL,
             (LPBYTE) szHelpFileName, &dwBufLen);
      RegCloseKey( hKey );

      WinHelp(hdlg, szHelpFileName, HELP_INDEX, 0L);
    }
    break;

    case IDOK:
    {
      LPSETUPDLG lpsetupdlg;
      ulong flag;
      uint i;

      lpsetupdlg = (LPSETUPDLG)GetWindowLong(hdlg, DWL_USER);

      /* Retrieve dialog values */
      if (!lpsetupdlg->fDefault)
      GetDlgItemText(hdlg, IDC_DSNAME,
           lpsetupdlg->aAttr[KEY_DSN].szAttr,
           sizeof(lpsetupdlg->aAttr[KEY_DSN].szAttr));
      GetDlgItemText(hdlg, IDC_DESC,
         lpsetupdlg->aAttr[KEY_DESC].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_DESC].szAttr));
      GetDlgItemText(hdlg, IDC_DB,
         lpsetupdlg->aAttr[KEY_DB].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_DB].szAttr));
      GetDlgItemText(hdlg, IDC_SERVER,
         lpsetupdlg->aAttr[KEY_SERVER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_SERVER].szAttr));
      GetDlgItemText(hdlg, IDC_USER,
         lpsetupdlg->aAttr[KEY_USER].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_USER].szAttr));
      GetDlgItemText(hdlg, IDC_PASSWORD,
         lpsetupdlg->aAttr[KEY_PASSWORD].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PASSWORD].szAttr));
      GetDlgItemText(hdlg, IDC_PORT,
         lpsetupdlg->aAttr[KEY_PORT].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_PORT].szAttr));
      GetDlgItemText(hdlg, IDC_STMT,
         lpsetupdlg->aAttr[KEY_STMT].szAttr,
         sizeof(lpsetupdlg->aAttr[KEY_STMT].szAttr));

      flag=0;
      for (i=0 ; i < NUMOFBITS ; i++)
  if (IsDlgButtonChecked(hdlg, IDC_CHECK1+i) == BST_CHECKED)
    flag|= 1 << i;
      sprintf(lpsetupdlg->aAttr[KEY_FLAG].szAttr,"%lu",flag);
    }
    case IDCANCEL:
      Ctl3dUnregister (s_hModule);
      EndDialog(hdlg, GET_WM_COMMAND_ID(wParam, lParam) == IDOK);
      return TRUE;
    }
  }
  return FALSE;
}
