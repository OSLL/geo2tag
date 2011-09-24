# no post install
#%define __os_install_post %{nil}
# no strip
#%define __strip /bin/true
# no debug package
#%define debug_package %{nil}
# disable automatic dependency processing
#AutoReqProv: no

Name:           meego-client
Version:0.0.1
Release:        1
License:        <insert license tag>
Summary:        <insert summary tag>
Url:            <insert url tag>
Group:          Other
Source0:        %{name}-%{version}.tar.gz
#BuildRequires:  pkgconfig(QtCore)
BuildRequires:  libqt-devel
BuildRequires:  qjson-devel
BuildRequires:  fakeroot

%description
MeeGo client of geo2tag platform

%prep
%setup -q

%build
# Add here commands to configure the package.
#%qmake
#qmake -makefile -nocache QMAKE_STRIP=: PREFIX=%{_prefix}

# Add here commands to compile the package.
#make %{?jobs:-j%jobs}
#make %{?_smp_mflags}
make

%install
# Add here commands to install the package.
#%qmake_install
make install INSTALL_ROOT=%{buildroot}

%post
ln -f -s /usr/lib/libmeego-client.so.0.0.1 /usr/lib/libmeego-client.so
ln -f -s /usr/lib/libmeego-client.so.0.0.1 /usr/lib/libmeego-client.so.0
ln -f -s /usr/lib/libmeego-client.so.0.0.1 /usr/lib/libmeego-client.so.0.0
fakeroot /sbin/ldconfig

%files
%defattr(-,root,root,-)
/*

