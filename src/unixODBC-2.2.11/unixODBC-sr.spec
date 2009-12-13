#
# spec file for the unixODBC package
#

Name:		unixODBC
Version:	2.2.11
Release:	1sr
Summary: 	ODBC Driver Manager
License:	LGPL
URL:		http://www.%{name}.org/

%define vpkg odbc
%{!?_sysconfdir: %define _sysconfdir /etc}
# default location for drivers is libdir
%{!?drvlibdir: %define drvlibdir %{_libdir}}

%define configure_options --enable-shared --enable-static
# defaults below can be overridden during build
# e.g. rpmbuild --define 'build_gui_gtk 1'
%{!?build_drivers: %define build_drivers 1}
%{!?build_gui_qt:  %define build_gui_qt  1}
%{!?build_gui_gtk: %define build_gui_gtk 0}

# define macros if they are not defined
%{!?__ldconfig:%define __ldconfig /sbin/ldconfig}
%{!?_pkglibdir:%define _pkglibdir %{_libdir}/%{name}}
%{!?__libtool:%define __libtool /usr/bin/libtool}
%{!?__cut:%define __cut /usr/bin/cut}
%{!?__cat:%define __cat /bin/cat}

# --- Multi-Vendor specifics ---
%if "%{_vendor}" == "suse"
  %define group Productivity/Databases/Tools
  %define group_devel Development/Libraries/C and C++
  %define conflicts libiodbc-devel
  # special _sysconfdir defined on SuSE for co-existence with iODBC (includes conflict remains)
  %define _sysconfdir /etc/%{name}
  # SuSE puts driver and setup libraries into pkglibdir (usually /usr/lib/unixODBC)
  %define drvlibdir %{_pkglibdir}
%elseif "%{_vendor}" == "redhat"
  %define group System Environment/Libraries
  %define group_devel Development/Libraries
%endif

Group:		%{?group}%{!?group:Applications/Databases}
Provides:	%{vpkg}, %{name}
# conflicts with iODBC (same includes and sysconfdir not compatible)
Conflicts:	%{?conflicts}%{!?conflicts:iodbc}
Source:		%{?url}%{name}-%{version}.tar.gz
#Patch:		%{name}-xxxx.patch
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-buildroot
# do not change prefix below but use "rpm -i --prefix=<prefix>" to relocate
Prefix:		%{_prefix}
%description

unixODBC is an Open Source ODBC sub-system and ODBC SDK for UNIX

Install unixODBC if you want to access databases through ODBC.

Drivers bundled with %{name} can be found in the %{name}-drivers packages.
Links to further drivers can be found under %{url}drivers.html

If you want to develop programs that will access data through ODBC,
you need to install the %{name}-devel package.

The unixODBC Project goals are to develop and promote unixODBC to be the
definitive standard for ODBC on non MS Windows platforms.
This is to include GUI support for both KDE and GNOME.

This package was built for the %{_target_platform} platform.
If you want to run it on a different architecture and platform,
it is recommended that you rebuild it using the Source RPM.
# --- Sub-Packages ---

%package devel
Summary:	Includes and static libraries for ODBC development
Group:		%{?group_devel}%{!?group_devel:Development/Libraries}
Requires:	%{name} = %{version} 
Provides:	%{name}-devel, %{vpkg}-devel
%description devel
This package contains the include files and static libraries
for ODBC development.

%if %{build_gui_qt}
%package gui-qt
Summary:	ODBC configurator, Data Source browser and ODBC test tool based on Qt
Group: 		%{?group_gui}%{!?group_gui:Applications/Databases}
#BuildPrereq:	qt[23]-devel
Requires:	%{name} = %{version}
#Requires:	%{name} = %{version}, qt[23]
Provides:	%{name}-gui, %{vpkg}-gui
%description gui-qt

This package contains Qt based GUI programs for unixODBC:
- ODBCConfig      graphical user interface for configuring the ODBC framework
- DataManager     graphical user interface for browsing databases
  DataManagerII   (next generation)
- odbctest        graphical user interface for testing ODBC drivers (developer)
%endif # build_gui_qt

%if %{build_gui_gtk}
%package gui-gtk
Summary:	ODBC configurator based on GTK+ and GTK+ widget for gnome-db
Group: 		%{?group_gui}%{!?group_gui:Applications/Databases}
#BuildPrereq:	gnome-libs-devel, gtk-devel
Requires:	%{name} = %{version}
#Requires:	%{name} = %{version}, gnome-libs, gtk
Provides:	%{name}-gui, %{vpkg}-gui
%description gui-gtk
This package contains one GTK+ based GUI program for unixODBC: 
- gODBCConfig: a graphical user interface for configuring DSNs
%endif # build_gui_gtk

%if %{build_drivers}
%package drivers
Summary:	Database Drivers and Setup libraries for unixODBC
Group:		%{?group}%{!?group:Applications/Databases}
Requires:	%{name} = %{version} 
Provides:	%{name}-drivers, %{vpkg}-drivers
%description drivers
This package contains unixODBC drivers and setup libraries
for various database systems
%package drivers-devel
Summary:	Static library versions of Drivers and Setup libraries for unixODBC
Group:		%{?group}%{!?group:Applications/Databases}
Requires:	%{name}-devel = %{version} 
Provides:	%{name}-drivers-devel, %{vpkg}-drivers-devel
%description drivers
This package contains unixODBC drivers and setup libraries
for various database systems
%description drivers-devel
This package contains static versions of unixODBC drivers and setup libraries
%endif # build_drivers

%prep
# --- prep scriptlet ---
%if %{build_gui_qt}
# for building the Qt GUI the QTDIR variable has to be set
if test -z "$QTDIR"; then
  %{__cat} <<'!'

ERROR - environment variable QTDIR is not set!

In order to build the Qt GUI for %{name} this variable
needs to point to the root of the Qt installation (e.g. /usr/lib/qt3)
If you do not intend to build the gui-qt subpackage, you just need to
call rpmbuild with: --define 'build_gui_qt 0'

Build aborted.
!
  exit 1
fi
%endif # build_gui_qt

# unpack original sources
%setup
# and apply patch(es) if any
#%patch
%if "%{_vendor}" == "suse"
  # SuSE puts the drivers in pkglibdir (needs a Makefile.in editing)
  %{__perl} -pi.orig -e 's/\$\(DESTDIR\)\$\(libdir\)/\$\(DESTDIR\)\$\(pkglibdir\)/' \
    Drivers/*/Makefile.in DRVConfig/*/Makefile.in
%endif
%if %{build_gui_gtk}
  # fix depcomp in GTK GUI (libtool needs it for relinking)
  ( cd gODBCConfig; if test ! -f ./depcomp; then ln -s ../depcomp .; fi )
%endif
# --- end of prep scriptlet ---
%build
# --- build scriptlet ---

# configure autoconf package
%configure \
%if %{build_drivers}
  --enable-drivers \
%else
  --disable-drivers \
%endif
%if %{build_gui_qt}%{build_gui_gtk}
  --enable-gui \
%else
  --disable-gui \
%endif
%if %{build_gui_qt}
  --with-qt-dir="${QTDIR}" \
%endif
  %{?configure_options}
# and compile it
%{__make} all
# fix relink statement in .la files created by buggy bundled libtool
# prepend temporary install lib path to aid in relinking
for la in `%{__grep} '^relink_command=' */*.la | %{__cut} -d: -f1`; do
  %{__sed} "s,^\(relink_command=.*\.lo \),\1-L${RPM_BUILD_ROOT}%{_libdir} ," $la >$la.fixed
  %{__mv} $la.fixed $la
done
# above fix can be removed once unixODBC bundles
# ltmain.sh with an appropriate relink patch (>=1.5.x)

# --- end of build scriptlet ---
%install
# --- install scriptlet ---

# remove old build root directory
%{__rm} -rf $RPM_BUILD_ROOT
# install into fresh build root
%makeinstall

%if %{build_gui_gtk}
  # GTK GUI must be configured and built after installing the main unixODBC parts
  cd gODBCConfig
  %configure --enable-shared --with-odbc=${RPM_BUILD_ROOT}%{_prefix}
  # does not like makeinstall - do manually
  %{__make} DESTDIR=${RPM_BUILD_ROOT} install
  cd ..
%endif # build_gui_gtk

# remove empty configuration files from buildroot
# (they will be created on-the-fly by the post scriptlet)
%{__rm}	-f ${RPM_BUILD_ROOT}%{_sysconfdir}/odbcinst.ini
%{__rm} -f ${RPM_BUILD_ROOT}%{_sysconfdir}/odbc.ini

%if "%{_vendor}" == "suse"
  # rename odbctest to ODBCTest in SuSE to avoid clash with iODBC
  mv ${RPM_BUILD_ROOT}%{_bindir}/odbctest ${RPM_BUILD_ROOT}%{_bindir}/ODBCTest
%endif

# --- end of install scriptlet ---
%clean
# --- clean scriptlet ---
# remove build root directory
%{__rm} -rf $RPM_BUILD_ROOT

# --- end of clean scriptlet ---
%files
# --- files section ---
%defattr(-, root, root)
%doc	AUTHORS COPYING ChangeLog NEWS README README.* doc
%dir	%{_sysconfdir}/ODBCDataSources
# system config files not included - created on the fly by post
#config	%{_sysconfdir}/odbcinst.ini
#config	%{_sysconfdir}/odbc.ini
%{_bindir}/isql
%{_bindir}/dltest
%{_bindir}/odbcinst
%{_bindir}/iusql
%{_bindir}/odbc_config
%{_libdir}/libodbc.so*
# driver manager configuration
%{_libdir}/libodbcinst.so*
# generic cursors library
%{_libdir}/libodbccr.so*
# testing
%{_libdir}/libgtrtst.so*
%{_libdir}/libboundparam.so*

%files devel
%defattr(-, root, root)
%{_includedir}/*.h
%{_libdir}/*.a
%{_libdir}/*.la

%if %{build_gui_qt}
%files gui-qt
%defattr(-, root, root)
%{_bindir}/ODBCConfig
%{_bindir}/DataManager
%{_bindir}/DataManagerII
%if "%{_vendor}" == "suse"
%{_bindir}/ODBCTest
%else
%{_bindir}/odbctest
%endif
%{_libdir}/libodbcinstQ.so*
%endif

%if %{build_gui_gtk}
%defattr(-, root, root)
%files gui-gtk
%{_bindir}/gODBCConfig
%{_libdir}/libgtkodbcconfig.so*
%endif

%if %{build_drivers}
%files drivers
%defattr(-, root, root)
%{drvlibdir}/libesoobS.so*
%{drvlibdir}/libmimerS.so.*
%{drvlibdir}/libnn.so*
%{drvlibdir}/libodbcdrvcfg1S.so*
%{drvlibdir}/libodbcdrvcfg2S.so*
%{drvlibdir}/libodbcminiS.so*
%{drvlibdir}/libodbcmyS.so*
%{drvlibdir}/libodbcnnS.so*
%{drvlibdir}/libodbcpsql.so*
%{drvlibdir}/libodbcpsqlS.so*
%{drvlibdir}/libodbctxt.so*
%{drvlibdir}/libodbctxtS.so*
%{drvlibdir}/liboplodbcS.so*
%{drvlibdir}/liboraodbcS.so*
%{drvlibdir}/libsapdbS.so*
%{drvlibdir}/libtdsS.so*
%{drvlibdir}/libtemplate.so.*
%files drivers-devel
%defattr(-, root, root)
%{drvlibdir}/libesoobS.*a
%{drvlibdir}/libmimerS.*a
%{drvlibdir}/libnn.*a
%{drvlibdir}/libodbcdrvcfg1S.*a
%{drvlibdir}/libodbcdrvcfg2S.*a
%{drvlibdir}/libodbcminiS.*a
%{drvlibdir}/libodbcmyS.*a
%{drvlibdir}/libodbcnnS.*a
%{drvlibdir}/libodbcpsql.*a
%{drvlibdir}/libodbcpsqlS.*a
%{drvlibdir}/libodbctxt.*a
%{drvlibdir}/libodbctxtS.*a
%{drvlibdir}/liboplodbcS.*a
%{drvlibdir}/liboraodbcS.*a
%{drvlibdir}/libsapdbS.*a
%{drvlibdir}/libtdsS.*a
%{drvlibdir}/libtemplate.*a
%endif # build_drivers

# --- Pre/Post/Un/Install/Verify scripts ---

%post
%{__ldconfig}
if test "$1" = 1 ; then # first instance of the package
  # find configuration utility
  odbcinst=odbcinst # last resort - rely on path
  for dir in ${RPM_INSTALL_PREFIX}/bin %{_bindir}; do
    test -x $dir/$odbcinst || continue # not here
    odbcinst=$dir/$odbcinst # found 
    break # out of loop 
  done
  # create/remove default entries in configuration files
  # if they do not exist (creates config files):
  # driver configuration file odbcinst.ini
  $odbcinst -q -d -n 'ODBC' 1>&- 2>&- # exists ?
  if test $? -gt 0 ; then # no, create
    echo '[ODBC]' | $odbcinst  -i -d -r 1>&- 2>&- # install
    $odbcinst -u -d -n 'ODBC' 1>&- 2>&- # remove
  fi
  # system data source configuration file odbc.ini
  $odbcinst -q -s -l -n 'DEFAULT' 1>&- 2>&- # exists ?
  if test $? -gt 0 ; then # no, create
    echo '[DEFAULT]' | $odbcinst  -i -s -l -r 1>&- 2>&- # install
    $odbcinst -u -s -l -n 'DEFAULT' 1>&- 2>&- # remove
  fi
fi
%preun
# configuration files in %{_sysconfdir} stay in place (even if empty)
# they might be needed by another driver manager

%postun -p %{__ldconfig}

%changelog
* Wed Sep 08 2004 Nick Gorham <nick@easysoft.com> 2.2.10-1sr
- Update version number
* Tue Jul 27 2004 Stefan Radman <stefan.radman@ctbto.org> 2.2.9-2sr
- do not build gui-gtk package by default (tendency to fail)
- support override of build options (drivers,gui_qt,gui_gtk)
- removed some vendor/version specific hacks, some cosmetics ;-)
* Tue Jun 29 2004 Nick Gorham <nick@lurcher.org> 2.2.10
  Moved to new version and added to main CVS
* Fri Apr 25 2004 Stefan Radman <stefan.radman@ctbto.org> 2.2.9-1
- updated to 2.2.9
- removed obsolete readlines patch
- added odbc_config
* Thu Apr 08 2004 Stefan Radman <stefan.radman@ctbto.org> 2.2.8-1
- updated to 2.2.8
- enabled drivers, enabled threads
- fixed relinking problem by inserting temporary install lib path
  into relink_command of .la files on the fly. now builds standalone
- added drivers and drivers-devel packages
- move drivers into lib/unixODBC on SuSE
- added -readlines patch
- with-qt-dir is not enough, added QTDIR environment variable
  and a warning if its not set and gui-qt building is requested
- made subpackages configurable
- added Mimer to files
* Thu Jan 15 2004 Stefan Radman <stefan.radman@ctbto.org> 2.2.7-2
- fixed incomplete driver property parsing in libodbcinst (patch)
- added drivers again as separate package
* Wed Dec  2 2003 Stefan Radman <stefan.radman@ctbto.org> 2.2.7-1
- upgraded to 2.2.7
* Wed Nov  5 2003 Stefan Radman <stefan.radman@ctbto.org> 2.2.6-3
- renamed for first internal release
- changed pre: cannot use rpm to determine path of odbcinst
- creates driver subpackage (setup library needed for Oracle)
- disabled drivers, disabled threads
* Wed Jul 23 2003 Stefan Radman <stefan.radman@ctbto.org> 2.2.6-1
- initial version derived from several spec files and HOWTOs
- credits: unixODBC, RedHat, SuSE, Edas
- News Server driver not integrated to keep license GPL
- install DM libs in %{_libdir} but drivers in %{_libdir}/%{name}
- need nicer options to enable/disable building of gui and driver packages
- dont install odbc*.ini, will be created by post scriptlet
- sysconfdir is /etc and /etc/unixODBC in SuSE
- added --with-qt-dir to compile with qt3 specifically
- added Requires and BuildPrereq to gui-qt package
- added fix for relinking during install (prepend temporary install lib path)
