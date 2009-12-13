/*********************************************************************
 *
 * Written by Nick Gorham
 * (nick@lurcher.org).
 *
 * copyright (c) 1999 Nick Gorham
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 **********************************************************************/

#ifndef CATALOG_H
#define CATALOG_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include "odbctest.h"

class dTables : public QDialog 
{
    Q_OBJECT

    public:
        dTables( OdbcTest *parent, QString name );
        ~dTables();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table, *type;
		QComboBox *schema_len, *catalog_len, *table_len, *type_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table, *l_type;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len, *l_type_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dColumns : public QDialog 
{
    Q_OBJECT

    public:
        dColumns( OdbcTest *parent, QString name );
        ~dColumns();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table, *column;
		QComboBox *schema_len, *catalog_len, *table_len, *column_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table, *l_column;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len, *l_column_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dColumnPrivileges : public QDialog 
{
    Q_OBJECT

    public:
        dColumnPrivileges( OdbcTest *parent, QString name );
        ~dColumnPrivileges();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table, *column;
		QComboBox *schema_len, *catalog_len, *table_len, *column_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table, *l_column;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len, *l_column_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dForeignKeys : public QDialog 
{
    Q_OBJECT

    public:
        dForeignKeys( OdbcTest *parent, QString name );
        ~dForeignKeys();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table;
		QComboBox *fk_schema, *fk_catalog, *fk_table;
		QComboBox *schema_len, *catalog_len, *table_len;
		QComboBox *fk_schema_len, *fk_catalog_len, *fk_table_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table;
		QLabel *fk_l_schema, *fk_l_catalog, *fk_l_table;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len;
		QLabel *fk_l_schema_len, *fk_l_catalog_len, *fk_l_table_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dPrimaryKeys : public QDialog 
{
    Q_OBJECT

    public:
        dPrimaryKeys( OdbcTest *parent, QString name );
        ~dPrimaryKeys();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table;
		QComboBox *schema_len, *catalog_len, *table_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dProcedures : public QDialog 
{
    Q_OBJECT

    public:
        dProcedures( OdbcTest *parent, QString name );
        ~dProcedures();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table;
		QComboBox *schema_len, *catalog_len, *table_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dProcedureColumns : public QDialog 
{
    Q_OBJECT

    public:
        dProcedureColumns( OdbcTest *parent, QString name );
        ~dProcedureColumns();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table, *column;
		QComboBox *schema_len, *catalog_len, *table_len, *column_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table, *l_column;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len, *l_column_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dTablePrivileges : public QDialog 
{
    Q_OBJECT

    public:
        dTablePrivileges( OdbcTest *parent, QString name );
        ~dTablePrivileges();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table;
		QComboBox *schema_len, *catalog_len, *table_len;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dGetTypeInfo : public QDialog 
{
    Q_OBJECT

    public:
        dGetTypeInfo( OdbcTest *parent, QString name );
        ~dGetTypeInfo();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *type;
		QLabel *l_handle, *l_type;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dStatistics : public QDialog 
{
    Q_OBJECT

    public:
        dStatistics( OdbcTest *parent, QString name );
        ~dStatistics();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table;
		QComboBox *schema_len, *catalog_len, *table_len;
		QComboBox *unique, *reserved;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len;
		QLabel *l_unique, *l_reserved;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dSpecialColumns : public QDialog 
{
    Q_OBJECT

    public:
        dSpecialColumns( OdbcTest *parent, QString name );
        ~dSpecialColumns();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *schema, *catalog, *table;
		QComboBox *schema_len, *catalog_len, *table_len;
		QComboBox *ident, *scope, *nullable;
		QLabel *l_handle, *l_schema, *l_catalog, *l_table;
		QLabel *l_schema_len, *l_catalog_len, *l_table_len;
		QLabel *l_ident, *l_scope, *l_nullable;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};
#endif
