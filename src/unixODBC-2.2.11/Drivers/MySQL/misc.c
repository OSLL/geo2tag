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
 * MISC.C                                                                  *
 *                                                                         *
 * @description: Handling misc routines                                    *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"

#if defined(_UNIX_)

#if !defined(HAVE_SQLGETPRIVATEPROFILESTRING)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>


static void open_ini_files(FILE** sys_file, FILE** user_file)
{
  char* s;
  FILE* system_ini=NULL;
  FILE* user_ini=NULL;

  s=getenv("ODBCINI");
  if(s) {
    user_ini=fopen(s,"r");
  }

  /* only look in $HOME/.odbc.ini if $ODBCINI wasn't there */
  if(!user_ini) {
    /* find the homedir */
    s=getenv("HOME");
    if(!s) {
      /* the following is clearly not threadsafe in
       * most environments
       */
      struct passwd* pwd;
      pwd=getpwuid(getuid());
      if(pwd) {
  s=pwd->pw_dir;
      }
    }

    if(s && s[0]) {
      char tmp[1024];
      tmp[0]=0;

      strcat(tmp,s);
      strcat(tmp,"/.odbc.ini");

      user_ini=fopen(tmp,"r");
    }
  }

  system_ini=fopen(SYSTEM_ODBC_INI,"r");

  *sys_file=system_ini;
  *user_file=user_ini;
}

static void close_ini_files(FILE* sys_file, FILE* user_file)
{
  if(sys_file) {
    fclose(sys_file);
  }

  if(user_file) {
    fclose(user_file);
  }
}

/* this does not do any reallocation */
static void trim_spaces(char* str)
{
  static const char *start_crap=" \t";
  static const char *end_crap=" \t\r\n";
  int start=0, end=strlen(str)-1;
  while(strchr(start_crap,str[start])!=NULL) start++;
  while(end>=0 && strchr(end_crap,str[end])!=NULL) end--;

  if(start<=end) {
    memmove(str,&str[start],end-start+1);
    str[end-start+1]=0;
  } else {
    str[0]=0;
  }
}

/* Searches for a section, returns non-zero on success
 * If successeful, positions on the line after the section
 * name.
 */
static int
find_section(FILE* ini, const char* section)
{
  char buf[1024];
  int found=0;
  int pos;
  char* r;

  while(!found && (r=fgets(buf,1024,ini))!=NULL) {
    /* find first meaningful char */
    pos=0;
    while(buf[pos]==' ' || buf[pos]=='\t') {
      pos++;
    }

    if(buf[pos]==0 || buf[pos]==';' || buf[pos]=='#') {
      /* comment or empty line*/
      continue;
    }

    if(buf[pos]=='[') {
      char* end=strchr(&buf[++pos],']');

      if(end) {
  int len=end-&buf[pos];
  char* sect_name=(char*)malloc(len+1);
  strncpy(sect_name,&buf[pos],len);
  sect_name[len]=0;
  trim_spaces(sect_name);
  found=!strcasecmp(sect_name,section);
  free(sect_name);
      }
    }
  }
  return found;
}

/* Returns NULL if not found, otherwise result must be freed
 * Expects ini to be positioned at the beginning of a section
 */
static char*
find_key(FILE* ini, const char* key)
{
  char buf[1024];
  int pos;
  char* r=NULL;
  char* tmp;
  char* value=NULL;

  /* if any of those starts a line, the line is ignored */
  static const char* skipchars=";#=\r\n";

  while(!value && (r=fgets(buf,1024,ini))!=NULL) {
    pos=0;

    /* skip crap at beginning */
    while(buf[pos]==' ' || buf[pos]=='\t') {
      pos++;
    }

    if(buf[pos]==0 || strchr(skipchars,buf[pos])!=NULL) {
      /* comment or empty line */
      continue;
    }

    if(buf[pos]=='[') {
      /* new section */
      break;
    }

    tmp=strchr(buf,'=');
    if(tmp) {
      char* name=(char*)malloc((tmp-buf+1));
      strncpy(name,buf,tmp-buf);
      name[tmp-buf]=0;
      trim_spaces(name);

      if(!strcasecmp(name,key)) {
  tmp++;
  value=(char*)malloc(strlen(tmp)+1);
  strcpy(value,tmp);
  trim_spaces(value);
      }

      free(name);
    }
  }

  return value;
}

/* First look for key in the user ini file, then in system.
 * Do not mix up values from both.
 */

static char*
find_key_by_dsn(const char* dsn, const char* key)
{
  FILE *sys;
  FILE *user;

  char* ret=NULL;

  open_ini_files(&sys,&user);

  if(user && find_section(user,dsn)) {
    ret=find_key(user,key);
  } else if(sys && find_section(sys,dsn)) {
    ret=find_key(sys,key);
  }

  close_ini_files(sys,user);

  return ret;
}



int _myodbc_SQLGetPrivateProfileString
(char* section, char* entry, char* def_value,
 char* buf,  int buf_len,  char* filename)
{
  char* value;
  char* src;

  if(buf==NULL || buf_len<=1) {
    /* no need to bother */
    return -1;
  }

  if(section==NULL || entry==NULL) {
    return -1;
  }

  if(!strcmp(filename,"ODBC.INI")) {
    value=find_key_by_dsn(section,entry);

    if(value) {
      src=value;
    } else {
      src=def_value;
    }

    if(src) {
      strncpy(buf,src,buf_len-1);
      buf[buf_len-1]=0;
    } else {
      buf[0]=0;
    }

    if(value) {
      free(value);
    }

    return strlen(buf);
  }
  return -1;
}


#endif /* HAVE_IODBCINST */

#endif /* _UNIX_ */
