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

#ifndef DESC_H
#define DESC_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include "odbctest.h"

class dCopyDesc : public QDialog 
{
    Q_OBJECT

    public:
		dCopyDesc( OdbcTest *parent, QString name );
        ~dCopyDesc();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handle1, *handle2;
		QLabel *l_handle1, *l_handle2;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dGetDescField : public QDialog 
{
    Q_OBJECT

    public:
		dGetDescField( OdbcTest *parent, QString name );
        ~dGetDescField();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *diag_info;
		QLabel *l_handle_type, *l_handles, *l_buffer_len, *l_rec_num, *l_diag_info;
		QCheckBox *ptr_valid, *strlen_valid;
		QLineEdit *buffer_len, *rec_num;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void ptr_clkd();
        void strlen_clkd();
};

class dGetDescRec : public QDialog 
{
    Q_OBJECT

    public:
		dGetDescRec( OdbcTest *parent, QString name );
        ~dGetDescRec();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles;
		QLabel *l_handle_type, *l_handles, *l_buffer_len, *l_rec_num;
		QCheckBox *name_valid, *strlen_valid, *type_valid, *sub_type_valid;
        QCheckBox *length_valid, *precision_valid, *scale_valid, *nullable_valid;
		QLineEdit *buffer_len, *rec_num;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void name_clkd();
        void strlen_clkd();
        void type_clkd();
        void sub_type_clkd();
        void length_clkd();
        void precision_clkd();
        void scale_clkd();
        void nullable_clkd();
};

#endif
