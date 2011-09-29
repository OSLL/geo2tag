#Define package name
%define app_name wikigps-service

Name:           wikigps-service
Version:        0.1.7
Release:        1%{?dist}
Summary:        Libs and Geo2Tag platform

Group:          System Tools
License:        GPL
URL:            http://osll.spb.ru/projects/geo2tag
Source0: 	%{app_name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{app_name}-%{version}-%{release}-root
BuildRequires:  qt-devel libxml2 qjson qjson-devel make fcgi-devel qt3-PostgreSQL libpqxx-devel

%description
This is Geo2Tag platform

%prep
%setup -q

%build
cd src/
qmake-qt4 PREFIX=%{_builddir} "CONFIG += maemo" -after *.pro
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir $RPM_BUILD_ROOT
cd src/
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
/opt/geo2tag/*
/etc/lighttpd/conf-enabled/wikigps-service.conf

%changelog
* Mon Sep 26 2011 Mark Zaslavskiy <mark.zaslavskiy@gmail.com>
- Initial relise
