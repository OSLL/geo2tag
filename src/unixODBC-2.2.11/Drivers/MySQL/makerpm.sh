#!/bin/sh

SPEC_FILE=myodbc3.spec
ARCH=`uname -m | perl -p -e 's/i[0-9]{3}/i386/'`
RELEASE=`grep AM_INIT_AUTOMAKE configure.in | perl -p -e 's/^AM_INIT_AUTOMAKE\(myodbc\,(.+)\)$/$1/'`
SOURCE=MyODBC

if [ -d /usr/src/redhat ]
then
    RPM_BUILD_AREA=/usr/src/redhat
else if [ -d /usr/src/packages ]
then
    RPM_BUILD_AREA=/usr/src/packages
fi
fi

replace @MYODBC_VERSION@ $RELEASE < myodbc3.spec.sh > myodbc3.spec

aclocal; autoheader; aclocal; automake; autoconf
make distclean		# Just for safety;  This will probably fail

# Remove symbolic links
if test -f myodbc3.c
then
  for i in $SOURCE*.tar.gz $SOURCE*.rpm
  do
    if test -L $i
    then
      echo "removing $i"
      rm $i
    fi
  done
fi

cp ${SPEC_FILE} ${RPM_BUILD_AREA}/SPECS
rm -rf ../$SOURCE-${RELEASE}
bk export . ../$SOURCE-${RELEASE}
cd ../$SOURCE-${RELEASE}
aclocal; autoheader; aclocal; automake; autoconf
chmod u+rw *
cd ..
tar czfh ${RPM_BUILD_AREA}/SOURCES/$SOURCE-${RELEASE}.tar.gz $SOURCE-${RELEASE}
ln -s ${RPM_BUILD_AREA}/SOURCES/$SOURCE-${RELEASE}.tar.gz myodbc3
rm -rf $SOURCE-${RELEASE}
cd ${RPM_BUILD_AREA}/SPECS

rpm -ba ${SPEC_FILE}
if test $? -eq 0 
then
  cd -; cd myodbc3
  ln -s ${RPM_BUILD_AREA}/RPMS/${ARCH}/$SOURCE-${RELEASE}-1.${ARCH}.rpm $SOURCE-${RELEASE}-1.${ARCH}.rpm
  ln -s ${RPM_BUILD_AREA}/SRPMS/$SOURCE-${RELEASE}-1.src.rpm $SOURCE-${RELEASE}-1.src.rpm
else
  echo "ERROR:  rpm failed"
fi
