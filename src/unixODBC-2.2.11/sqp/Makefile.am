if HAVE_FLEX

noinst_LTLIBRARIES = libsqplc.la

INCLUDES = -I@top_srcdir@/include

libsqplc_la_LIBADD = @LEXLIB@

CLEANFILES = \
	yac.c \
	lex.c

libsqplc_la_SOURCES = \
	sqpAdoptParsedSQL.c \
	sqpClose.c \
	sqpFreeAssignment.c \
	sqpFreeColumn.c \
	sqpFreeColumnDef.c \
	sqpFreeDataType.c \
	sqpFreeComparison.c \
	sqpFreeCond.c \
	sqpFreeCreateTable.c \
	sqpFreeDelete.c \
	sqpFreeDropTable.c \
	sqpFreeInsert.c \
	sqpFreeParam.c \
	sqpFreeParsedSQL.c \
	sqpFreeSelect.c \
	sqpFreeUpdate.c \
	sqpOpen.c \
	sqpParse.c \
	sqpError.c \
	sqpStoreAssignment.c \
	sqpStoreColumn.c \
	sqpStoreColumnDef.c \
	sqpStoreDataType.c \
	sqpStoreComparison.c \
	sqpStoreCond.c \
	sqpStoreCreateTable.c \
	sqpStoreDelete.c \
	sqpStoreDropTable.c \
	sqpStoreInsert.c \
	sqpStorePositioned.c \
	sqpStoreSelect.c \
	sqpStoreTable.c \
	sqpStoreUpdate.c \
	sqpStoreValue.c \
	yac.y \
	lex.l 

endif

EXTRA_DIST = \
	yac.h


