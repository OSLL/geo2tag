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
 * DIALOGS.H                                                               *
 *                                                                         *
 * @description: Definations needed for dialogboxes                        *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

#include "resource.h"

#define MAXPATHLEN      (255+1)           /* Max path length */
#define MAXKEYLEN       (15+1)            /* Max keyword length */
#define MAXDESC         (255+1)           /* Max description length */
#define MAXDSNAME       (64+1)            /* Max data source name length */
#define MAXSERVER (64+1)                  /* Max server name length */         
#define MAXUSER   (32+1)                  /* Max user name length */
#define MAXPASSWORD (32+1)                /* Max password length */
#define MAXSTMT   (255+1)                 /* Max initial statement length */

/* Attribute key indexes (into an array of Attr structs, see below */

#define KEY_DSN      0
#define KEY_DESC     1
#define KEY_DB       2
#define KEY_SERVER   3
#define KEY_USER     4
#define KEY_PASSWORD 5
#define KEY_PORT     6
#define KEY_STMT     7
#define KEY_FLAG     8
#define NUMOFKEYS    9   /* Number of keys supported */
#define NUMOFBITS    19  /* Number of different options */


typedef struct tagAttr {
  my_bool fSupplied;
  char  szAttr[MAXPATHLEN];
} Attr, FAR * LPAttr;


typedef struct tagSETUPDLG {
  SQLHWND hwndParent;       /* Parent window handle */
  LPCSTR  lpszDrvr;         /* Driver description */
  Attr  aAttr[NUMOFKEYS];   /* Attribute array */
  char  szDSN[MAXDSNAME];   /* Original data source name */
  my_bool fNewDSN;          /* New data source flag */
  my_bool fDefault;         /* Default data source flag */
  ulong config_flags;       /* Flags to affect myodbc behavour */
} SETUPDLG, FAR *LPSETUPDLG;


int FAR PASCAL DriverConnectProc(HWND hdlg,WORD wMsg,WPARAM wParam,
				 LPARAM lParam);
int FAR PASCAL  HelpDlgProc(HWND, WORD, WPARAM, LPARAM);
void INTFUNC ParseAttributes (LPCSTR lpszAttributes, LPSETUPDLG lpsetupdlg);
void set_attributes(LPSETUPDLG lpsetupdlg);


