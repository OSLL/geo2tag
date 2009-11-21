AC_INIT([g2t-server], [1.0], [osll@osll.spb.ru])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADER([config.h])

AC_PROG_CXX
AC_PROG_LIBTOOL
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/json/Makefile
 src/service/Makefile
])

fastcgi_found=no
AC_CHECK_LIB(fcgi, FCGI_Accept, [
 AC_CHECK_HEADERS([fastcgi.h fastcgi/fastcgi.h],[
   fastcgi_found=yes
 ])
])

AM_CONDITIONAL(CHECK_WITH_FASTCGI, test "x$fastcgi_found" = xyes)

AC_OUTPUT
