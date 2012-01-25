#Define package name
%define app_name wikigps-service

Name:           wikigps-service
Version:        0.1.7
Release:        1%{?dist}
Summary:        Libs and Geo2Tag platform..

Group:          System Tools
License:        GPL
URL:            http://osll.spb.ru/projects/geo2tag
Source0: 	%{app_name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{app_name}-%{version}-%{release}-root
BuildRequires:  qt-devel libxml2 qjson qjson-devel make fcgi-devel qt3-PostgreSQL postgresql-libs

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
mkdir -p $RPM_BUILD_ROOT/opt/geo2tag
install ../base.sql $RPM_BUILD_ROOT/opt/geo2tag/
make install INSTALL_ROOT=$RPM_BUILD_ROOT

%post
mkdir -p /etc/xdg/osll
chmod 777 /etc/xdg/osll
ldconfig
/etc/init.d/lighttpd restart;
# down will be DB initialization
# check is there geo2tag db and user
isEmpt=`sudo -u postgres psql -A -q -t -c "select datname from pg_database" template1 2>/dev/null | grep geo2tag`;
if [[ -n "$isEmpt" ]] 
then
exit 0;
fi 
sudo -u postgres createuser -s geo2tag
sudo -u postgres createdb -O geo2tag geo2tag
sudo -u postgres psql --command="GRANT ALL privileges on database geo2tag to geo2tag;"
sudo cp -f /opt/geo2tag/pg_hba.conf /var/lib/pgsql/data 
sudo /etc/init.d/postgresql restart
psql geo2tag -U geo2tag < /opt/geo2tag/base.sql
echo "sudo -u postgres psql --command=\"ALTER USER"
sudo -u postgres psql --command="ALTER USER geo2tag WITH PASSWORD 'geo2tag';"
sudo /etc/init.d/postgresql restart
sudo /etc/init.d/lighttpd restart;

%clean
rm -rf %{_builddir}

%postun
ldconfig


%files
%defattr(-,root,root,-)
%doc
/usr/lib/lib*
/opt/geo2tag/fcgi_server
/opt/geo2tag/base.sql
/opt/geo2tag/pg_hba.conf
/etc/lighttpd/conf-enabled/wikigps-service.conf

%changelog
* Mon Sep 26 2011 Mark Zaslavskiy <mark.zaslavskiy@gmail.com>
- Initial relise
