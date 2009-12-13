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

#ifndef STMT_H
#define STMT_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include "odbctest.h"

class dPrepare : public QDialog 
{
    Q_OBJECT

    public:
		dPrepare( OdbcTest *parent, QString name );
        ~dPrepare();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *str, *name_len;
		QLabel *l_handle, *l_str, *l_name_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dExecute : public QDialog 
{
    Q_OBJECT

    public:
		dExecute( OdbcTest *parent, QString name );
        ~dExecute();

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

class dExecDirect : public QDialog 
{
    Q_OBJECT

    public:
		dExecDirect( OdbcTest *parent, QString name );
        ~dExecDirect();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *str, *name_len;
		QLabel *l_handle, *l_str, *l_name_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dNumParams : public QDialog 
{
    Q_OBJECT

    public:
        dNumParams( OdbcTest *parent, QString name );
        ~dNumParams();

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
		void Valid();
		void Ok();
};

class dCancel : public QDialog 
{
    Q_OBJECT

    public:
		dCancel( OdbcTest *parent, QString name );
        ~dCancel();

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

class dCloseCursor : public QDialog 
{
    Q_OBJECT

    public:
		dCloseCursor( OdbcTest *parent, QString name );
        ~dCloseCursor();

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

class dGetCursorName : public QDialog 
{
    Q_OBJECT

    public:
		dGetCursorName( OdbcTest *parent, QString name );
        ~dGetCursorName();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *option;
		QLabel *l_handle, *l_option, *l_buffer_len;
		QCheckBox *name_valid, *cursor_valid;
		QLineEdit *buffer_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
		void CursorValid();
		void NameValid();
};

class dFreeStmt : public QDialog 
{
    Q_OBJECT

    public:
		dFreeStmt( OdbcTest *parent, QString name );
        ~dFreeStmt();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *option;
		QLabel *l_handle, *l_option;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dSetCursorName : public QDialog 
{
    Q_OBJECT

    public:
		dSetCursorName( OdbcTest *parent, QString name );
        ~dSetCursorName();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *str, *name_len;
		QLabel *l_handle, *l_str, *l_name_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dDescribeParam : public QDialog 
{
    Q_OBJECT

    public:
        dDescribeParam( OdbcTest *parent, QString name );
        ~dDescribeParam();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLineEdit *param_num;
		QLabel *l_handle, *l_param_num;
		QCheckBox *type_valid;
		QCheckBox *digit_valid;
		QCheckBox *size_valid;
		QCheckBox *null_valid;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;
		const char *data_type_to_str( int );

	protected slots:
		void type_clkd();
		void digit_clkd();
		void size_clkd();
		void null_clkd();
		void Ok();
};

class dParamData : public QDialog 
{
    Q_OBJECT

    public:
        dParamData( OdbcTest *parent, QString name );
        ~dParamData();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLabel *l_handle;
		QCheckBox *value_valid;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void value_clkd();
		void Ok();
};

class dParamOption : public QDialog 
{
    Q_OBJECT

    public:
        dParamOption( OdbcTest *parent, QString name );
        ~dParamOption();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLabel *l_handle;
		QLineEdit *crow_num;
		QLabel *l_param_num, *l_crow_num;
		QCheckBox *pirow_valid;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void pirow_clkd();
		void Ok();
};

class dPutData : public QDialog 
{
    Q_OBJECT

    public:
		dPutData( OdbcTest *parent, QString name );
        ~dPutData();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *data_ptr, *str_len;
		QLabel *l_handle, *l_data_ptr, *l_str_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dBindParameter : public QDialog 
{
    Q_OBJECT

    public:
		dBindParameter( OdbcTest *parent, QString name );
        ~dBindParameter();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *io_type, *value_type;
		QComboBox *param_type, *param_value, *strlen_or_ind;
		QLabel *l_handle, *l_io_type, *l_value_type;
        QLabel *l_param_type, *l_param_value, *l_strlen_or_ind, *l_param_num;
        QLineEdit *column_size, *decimal_digits, *buffer_length, *param_num;
        QLabel *l_column_size, *l_decimal_digits, *l_buffer_length;
		QCheckBox *len_data_at_exec;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dBindParam : public QDialog 
{
    Q_OBJECT

    public:
		dBindParam( OdbcTest *parent, QString name );
        ~dBindParam();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *value_type;
		QComboBox *param_type, *param_value, *strlen_or_ind;
		QLabel *l_handle, *l_value_type;
        QLabel *l_param_type, *l_param_value, *l_strlen_or_ind, *l_param_num;
        QLineEdit *column_size, *decimal_digits, *param_num;
        QLabel *l_column_size, *l_decimal_digits;
		QCheckBox *len_data_at_exec;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dAllocStmt : public QDialog 
{
    Q_OBJECT

    public:
        dAllocStmt( OdbcTest *parent, QString name );
        ~dAllocStmt();

	protected:
		QPushButton *ok, *cancel, *help;
		QMultiLineEdit *in_win;
		QCheckBox *handle_valid;
        QComboBox *handles;
        QLabel *l_handles;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void handle_clkd();
};

#endif
