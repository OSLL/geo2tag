#Define package name
%define app_name wikigps-libs

Name:           wikigps-libs
Version:        0.1.7
Release:        1%{?dist}
Summary:        Libs for Geo2Tag platform

Group:          System Tools
License:        GPL
URL:            http://osll.spb.ru/projects/geo2tag
Source0: 	%{app_name}-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{app_name}-%{version}-%{release}-root
BuildRequires:  qt-devel libxml2 libqt4-sql-psql qjson qjson-devel libxml2-devel

%description
This libs provide main functions for Geo2Tag platform

%prep
%setup -q

%build
qmake PREFIX=%{_builddir} "CONFIG += maemo" -after *.pro
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir $RPM_BUILD_ROOT

make install INSTALL_ROOT=$RPM_BUILD_ROOT

%post
mkdir -p /etc/xdg/osll
chmod 777 /etc/xdg/osllt
ldconfig

%clean
rm -rf %{_builddir}

%postun
ldconfig


%files
%defattr(-,root,root,-)
%doc
/usr/lib/lib*

%changelog
* Mon Sep 26 2011 Mark Zaslavskiy <mark.zaslavskiy@gmail.com>
- Initial relise
