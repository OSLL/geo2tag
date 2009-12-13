%define myodbc_version		@MYODBC_VERSION@
%define release			1

Name:		MyODBC
Summary:	An ODBC 3.51 driver for MySQL
Group:		Applications/Databases
Version:	%{myodbc_version}
Release:	%{release}
Copyright:	Public Domain, GPL
Source0:	MyODBC-%{myodbc_version}.tar.gz
URL:		http://www.mysql.com/
Vendor:		MySQL AB
Packager:	Venu Anuganti<venu@mysql.com>

# Think about what you use here since the first step is to
# run a rm -rf
BuildRoot:	/var/tmp/myodbc3

# From the manual
%description
MyODBC 3.51 is a enhanced version of MyODBC 2.50, with the ODBC 3.51 compliant from MySQL AB to access MySQL. The driver is commonly referred as 'MySQL ODBC 3.51 Driver'.

%prep
%setup -n MyODBC-%{myodbc_version}

%build
./configure \
	--prefix=${RPM_BUILD_ROOT}/usr/local \
	--with-iodbc=/usr/local
make

%clean 
rm -rf $RPM_BUILD_ROOT

%install
make PREFIX=$RPM_BUILD_ROOT install

find ${RPM_BUILD_ROOT}/usr/local -type f -print | sed "s@^${RPM_BUILD_ROOT}@@g" > myodbc-filelist
find ${RPM_BUILD_ROOT}/usr/local -type l -print | sed "s@^${RPM_BUILD_ROOT}@@g" >> myodbc-filelist

%files -f myodbc-filelist
%defattr(-,root,root)

%doc INSTALL
%doc ChangeLog
%doc odbc.ini
%doc README
%doc RELEASE-NOTES
