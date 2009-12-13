%define name     unixODBC
%define ver      2.2.11
%define prefix   /usr
%define sysconfdir	/etc

Summary: ODBC driver manager and drivers for PostgreSQL, MySQL, etc.
Name: %{name}
Version: %ver
Release: 1
Copyright: LGPL and GPL
Group: Applications/Databases
Source: %{name}-%{ver}.tar.gz
BuildRoot: /var/tmp/%{name}-%{ver}-root
URL: http://www.unixodbc.org/
Docdir: %{prefix}/doc
Prefix: %prefix

%description
unixODBC aims to provide a complete ODBC solution for the Linux platform.
All programs are GPL.
All libs are LGPL (except nn which is GPL?).

%package devel
Summary: Includes and static libraries for ODBC development
Group: Development/Libraries
Requires: %{name} = %{ver}

%description devel
unixODBC aims to provide a complete ODBC solution for the Linux platform.
All programs are GPL.
All libs are LGPL (except nn which is GPL?).
This package contains the include files and static libraries
for development.

%package gui-qt
Summary: ODBC configurator, Data Source browser and ODBC test tool based on Qt
Group: Applications/Databases
Requires: %{name} = %{ver}

%description gui-qt
unixODBC aims to provide a complete ODBC solution for the Linux platform.
All programs are GPL.
All libs are LGPL (except nn which is GPL?).
This package contains two Qt based GUI programs for unixODBC:
ODBCConfig and DataManager

%package gui-gtk
Summary: ODBC configurator based on GTK+ and GTK+ widget for gnome-db
Group: Applications/Databases
Requires: %{name} = %{ver}

%description gui-gtk
unixODBC aims to provide a complete ODBC solution for the Linux platform.
All programs are GPL.
All libs are LGPL (except nn which is GPL?).
This package contains one GTK+ based GUI program for unixODBC: gODBCConfig

%prep
%setup

%ifarch alpha
  ARCH_FLAGS="--host=alpha-redhat-linux"
%endif

export -n LANG LINGUAS LC_ALL 
if [ ! -f configure ]; then
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh $ARCH_FLAGS --prefix=%{prefix} --sysconfdir=%{sysconfdir} 
else
  CFLAGS="$RPM_OPT_FLAGS" ./configure $ARCH_FLAGS --prefix=%{prefix} --sysconfdir=%{sysconfdir} 
fi

%build
export -n LANG LINGUAS LC_ALL 

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

make prefix=$RPM_BUILD_ROOT%{prefix} sysconfdir=$RPM_BUILD_ROOT%{sysconfdir} install-strip

# gODBCConfig must be built after installing the main unixODBC parts
cd gODBCConfig
./autogen.sh --prefix=%{prefix} --sysconfdir=%{sysconfdir} --with-odbc=$RPM_BUILD_ROOT%{prefix}
make
make prefix=$RPM_BUILD_ROOT%{prefix} sysconfdir=$RPM_BUILD_ROOT%{sysconfdir} install-strip
cd ..

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%pre
if [ -f %{sysconfdir}/odbc.ini ]; then
	mv -f %{sysconfdir}/odbc.ini %{sysconfdir}/odbc.ini.rpmsave
fi
if [ -f %{sysconfdir}/odbcinst.ini ]; then
	mv -f %{sysconfdir}/odbcinst.ini %{sysconfdir}/odbcinst.ini.rpmsave
fi

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc AUTHORS COPYING ChangeLog NEWS README README.GTK doc

%{sysconfdir}/odbc.ini
%{sysconfdir}/odbcinst.ini
%{prefix}/bin/dltest
%{prefix}/bin/isql
%{prefix}/bin/iusql
%{prefix}/bin/odbcinst
%{prefix}/lib/libesoobS.so*
%{prefix}/lib/libsapdbS.so*
%{prefix}/lib/libnn.so*
%{prefix}/lib/libodbc.so*
%{prefix}/lib/libodbccr.so*
%{prefix}/lib/libodbcdrvcfg1S.so*
%{prefix}/lib/libodbcdrvcfg2S.so*
%{prefix}/lib/libodbcinst.so*
%{prefix}/lib/libodbcminiS.so*
%{prefix}/lib/libodbcmyS.so*
%{prefix}/lib/libodbcnnS.so*
%{prefix}/lib/libodbcpsql.so*
%{prefix}/lib/libodbcpsqlS.so*
%{prefix}/lib/libodbctxt.so*
%{prefix}/lib/libodbctxtS.so*
%{prefix}/lib/liboplodbcS.so*
%{prefix}/lib/liboraodbcS.so*
%{prefix}/lib/libtdsS.so*
%{prefix}/lib/libtemplate.so*

%files devel
%defattr(-, root, root)

%{prefix}/include/*
%{prefix}/lib/*.la

%files gui-qt
%defattr(-, root, root)

%{prefix}/bin/DataManager
%{prefix}/bin/DataManagerII
%{prefix}/bin/ODBCConfig
%{prefix}/bin/odbctest
%{prefix}/lib/libodbcinstQ.so*

%files gui-gtk
%defattr(-, root, root)

%{prefix}/bin/gODBCConfig
%{prefix}/lib/libgtkodbcconfig.so*

