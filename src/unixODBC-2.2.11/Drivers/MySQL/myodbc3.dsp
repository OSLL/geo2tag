# Microsoft Developer Studio Project File - Name="myodbc3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=myodbc3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "myodbc3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "myodbc3.mak" CFG="myodbc3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "myodbc3 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "myodbc3 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "myodbc3 - Win32 Share" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "myodbc3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MYODBC3_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "c:\mysql\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MYODBC3_EXPORTS" /D "DONT_DEFINE_VOID" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ctl3d32.lib mysqlclient.lib zlib.lib /nologo /dll /machine:I386 /libpath:"c:\mysql\lib\opt"
# Begin Custom Build
InputPath=.\Release\myodbc3.dll
SOURCE="$(InputPath)"

BuildCmds= \
	copy .\Release\myodbc3.dll c:\winnt\system32\myodbc3.dll \
	copy .\Release\myodbc3.lib c:\winnt\system32\myodbc3.lib \
	

"c:\winnt\system32\myodbc3.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"c:\winnt\system32\myodbc3.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "myodbc3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "myodbc3___Win32_Debug"
# PROP BASE Intermediate_Dir "myodbc3___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MYODBC3_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MTd /W3 /GX /ZI /I "F:\MyODBC3\mysql-3.23.41\include" /D "DBUG_ON" /D "DEBUG" /D "_WIN32" /D "ENGLISH" /D "DONT_DEFINE_VOID" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ctl3d32.lib mysqlclient.lib zlib.lib /nologo /dll /pdb:none /debug /machine:I386 /libpath:"F:\MyODBC3\mysql-3.23.41\lib_debug"
# SUBTRACT LINK32 /verbose
# Begin Custom Build
InputPath=.\Debug\myodbc3.dll
SOURCE="$(InputPath)"

BuildCmds= \
	copy .\Debug\myodbc3.dll c:\winnt\system32\myodbc3.dll \
	copy .\Debug\myodbc3.lib c:\winnt\system32\myodbc3.lib \
	

"c:\winnt\system32\myodbc3.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"c:\winnt\system32\myodbc3.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "myodbc3 - Win32 Share"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "myodbc3___Win32_Share"
# PROP BASE Intermediate_Dir "myodbc3___Win32_Share"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Share"
# PROP Intermediate_Dir "Share"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MYODBC3_EXPORTS" /D "DONT_DEFINE_VOID" /FR /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /Ob2 /I "c:\mysql\include" /D "DBUG_OFF" /D "NDEBUG" /D "_WIN32" /D "ENGLISH" /D "DONT_DEFINE_VOID" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "c:\mysql\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ctl3d32.lib mysqlclient.lib zlib.lib /nologo /dll /machine:I386 /nodefaultlib:"LIBCMT.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ctl3d32.lib mysqlclient.lib /nologo /dll /machine:I386 /libpath:"c:\mysql\lib\opt"
# SUBTRACT LINK32 /debug
# Begin Custom Build
InputPath=.\Share\myodbc3.dll
SOURCE="$(InputPath)"

BuildCmds= \
	copy .\Share\myodbc3.dll c:\winnt\system32\myodbc3.dll \
	copy .\Share\myodbc3.lib c:\winnt\system32\myodbc3.lib \
	

"c:\winnt\system32\myodbc3.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"c:\winnt\system32\myodbc3.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# Begin Target

# Name "myodbc3 - Win32 Release"
# Name "myodbc3 - Win32 Debug"
# Name "myodbc3 - Win32 Share"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\catalog.c
# End Source File
# Begin Source File

SOURCE=.\connect.c
# End Source File
# Begin Source File

SOURCE=.\cursor.c
# End Source File
# Begin Source File

SOURCE=.\dll.c
# End Source File
# Begin Source File

SOURCE=.\error.c
# End Source File
# Begin Source File

SOURCE=.\execute.c
# End Source File
# Begin Source File

SOURCE=.\handle.c
# End Source File
# Begin Source File

SOURCE=.\info.c
# End Source File
# Begin Source File

SOURCE=.\misc.c
# End Source File
# Begin Source File

SOURCE=.\myodbc3.c
# End Source File
# Begin Source File

SOURCE=.\myodbc3.def
# End Source File
# Begin Source File

SOURCE=.\options.c
# End Source File
# Begin Source File

SOURCE=.\prepare.c
# End Source File
# Begin Source File

SOURCE=.\results.c
# End Source File
# Begin Source File

SOURCE=.\setup.c
# End Source File
# Begin Source File

SOURCE=.\transact.c
# End Source File
# Begin Source File

SOURCE=.\utility.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\dialogs.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\myodbc3.h
# End Source File
# Begin Source File

SOURCE=.\myutil.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\myhelp.bmp
# End Source File
# Begin Source File

SOURCE=.\myhelp.ico
# End Source File
# Begin Source File

SOURCE=.\myimage.bmp
# End Source File
# Begin Source File

SOURCE=.\myodbc3.rc

!IF  "$(CFG)" == "myodbc3 - Win32 Release"

!ELSEIF  "$(CFG)" == "myodbc3 - Win32 Debug"

!ELSEIF  "$(CFG)" == "myodbc3 - Win32 Share"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mypower.bmp
# End Source File
# Begin Source File

SOURCE=".\mysql-long.bmp"
# End Source File
# Begin Source File

SOURCE=.\mysql.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\index.html
# End Source File
# End Target
# End Project
