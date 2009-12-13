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

#ifndef RESULTS_H
#define RESULTS_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include "odbctest.h"

class dGetData : public QDialog 
{
    Q_OBJECT

    public:
        dGetData( OdbcTest *parent, QString name );
        ~dGetData();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *type;
		QLineEdit *column_num;
		QLineEdit *buffer_len;
		QCheckBox *target_valid, *strlen_valid;
		QLabel *l_handle, *l_column_num, *l_type, *l_buffer_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void target_clkd();
		void strlen_clkd();
		void Ok();
};

class dDescribeCol : public QDialog 
{
    Q_OBJECT

    public:
        dDescribeCol( OdbcTest *parent, QString name );
        ~dDescribeCol();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLineEdit *column_num;
		QLineEdit *column_len;
		QLabel *l_handle, *l_column_num, *l_column_len;
		QCheckBox *name_valid;
		QCheckBox *type_valid;
		QCheckBox *size_valid;
		QCheckBox *digit_valid;
		QCheckBox *null_valid;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;
		const char *data_type_to_str( int );

	protected slots:
		void name_clkd();
		void type_clkd();
		void size_clkd();
		void digit_clkd();
		void null_clkd();
		void Ok();
};

class dNumResultCols : public QDialog 
{
    Q_OBJECT

    public:
        dNumResultCols( OdbcTest *parent, QString name );
        ~dNumResultCols();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QCheckBox *valid;
		QLabel *l_handle;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void out_handle_ptr_clkd();
		void Ok();
};

class dFetch : public QDialog 
{
    Q_OBJECT

    public:
        dFetch( OdbcTest *parent, QString name );
        ~dFetch();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLabel *l_handle;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dRowCount : public QDialog 
{
    Q_OBJECT

    public:
        dRowCount( OdbcTest *parent, QString name );
        ~dRowCount();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLabel *l_handle;
		QCheckBox *valid;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
		void Valid();
};

class dMoreResults : public QDialog 
{
    Q_OBJECT

    public:
        dMoreResults( OdbcTest *parent, QString name );
        ~dMoreResults();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLabel *l_handle;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dSetPos : public QDialog 
{
    Q_OBJECT

    public:
		dSetPos( OdbcTest *parent, QString name );
        ~dSetPos();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *operation, *lock_type;
		QLabel *l_handle, *l_operation, *l_lock_type, *l_row_number;
		QLineEdit *row_number;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dBulkOperations : public QDialog 
{
    Q_OBJECT

    public:
		dBulkOperations( OdbcTest *parent, QString name );
        ~dBulkOperations();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *operation;
		QLabel *l_handle, *l_operation;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dColAttribute : public QDialog 
{
    Q_OBJECT

    public:
		dColAttribute( OdbcTest *parent, QString name );
        ~dColAttribute();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *field_ident;
		QLabel *l_handles, *l_buffer_len, *l_col_num, *l_diag_info;
        QLabel *l_field_ident;
		QCheckBox *char_ptr_valid, *strlen_valid, *numeric_valid;
		QLineEdit *buffer_len, *col_num;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void char_ptr_clkd();
        void strlen_clkd();
        void numeric_clkd();
};

class dColAttributes : public QDialog 
{
    Q_OBJECT

    public:
		dColAttributes( OdbcTest *parent, QString name );
        ~dColAttributes();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *field_ident;
		QLabel *l_handles, *l_buffer_len, *l_col_num, *l_diag_info;
        QLabel *l_field_ident;
		QCheckBox *char_ptr_valid, *strlen_valid, *numeric_valid;
		QLineEdit *buffer_len, *col_num;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void char_ptr_clkd();
        void strlen_clkd();
        void numeric_clkd();
};

class dFetchScroll : public QDialog 
{
    Q_OBJECT

    public:
		dFetchScroll( OdbcTest *parent, QString name );
        ~dFetchScroll();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *orentation;
		QLabel *l_handle, *l_orentation, *l_offset;
		QLineEdit *offset;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dExtendedFetch : public QDialog 
{
    Q_OBJECT

    public:
		dExtendedFetch( OdbcTest *parent, QString name );
        ~dExtendedFetch();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *orentation;
		QLabel *l_handle, *l_orentation, *l_offset;
		QLineEdit *offset;
		QMultiLineEdit *in_win;
        QCheckBox *count_ptr_valid, *status_ptr_valid;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void count_ptr_clkd();
        void status_ptr_clkd();
};

class dSetScrollOptions : public QDialog 
{
    Q_OBJECT

    public:
		dSetScrollOptions( OdbcTest *parent, QString name );
        ~dSetScrollOptions();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *concurrency, *keyset;
		QLabel *l_handle, *l_concurrency, *l_keyset, *l_rowset;
		QLineEdit *rowset;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

#endif
