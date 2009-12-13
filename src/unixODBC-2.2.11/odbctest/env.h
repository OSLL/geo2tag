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

#ifndef ENV_H
#define ENV_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include "odbctest.h"

class dAllocHandle : public QDialog 
{
    Q_OBJECT

    public:
		dAllocHandle( OdbcTest *parent, QString name );
        ~dAllocHandle();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *types;
		QCheckBox *valid;
		QLabel *l_handle, *l_types;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void out_handle_ptr_clkd();
		void Ok();
};

class dFreeHandle : public QDialog 
{
    Q_OBJECT

    public:
        dFreeHandle( OdbcTest *parent, QString name );
        ~dFreeHandle();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *types;
		QCheckBox *valid;
		QLabel *l_handle, *l_types;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dDataSources : public QDialog 
{
    Q_OBJECT

    public:
        dDataSources( OdbcTest *parent, QString name );
        ~dDataSources();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *direction;
		QCheckBox *server_valid, *nlp1_valid, *description_valid, *nlp2_valid;
		QLabel *l_handles, *l_direction, *l_server_len, *l_description_len;
		QLineEdit *server_len, *description_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void server_clkd();
        void description_clkd();
        void nlp1_clkd();
        void nlp2_clkd();
};

class dDrivers : public QDialog 
{
    Q_OBJECT

    public:
        dDrivers( OdbcTest *parent, QString name );
        ~dDrivers();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *direction;
		QCheckBox *server_valid, *nlp1_valid, *description_valid, *nlp2_valid;
		QLabel *l_handles, *l_direction, *l_server_len, *l_description_len;
		QLineEdit *server_len, *description_len;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void server_clkd();
        void description_clkd();
        void nlp1_clkd();
        void nlp2_clkd();
};

class dEndTran : public QDialog 
{
    Q_OBJECT

    public:
        dEndTran( OdbcTest *parent, QString name );
        ~dEndTran();

	protected:
		QPushButton *ok, *cancel, *help;
		QComboBox *handles, *handle_type, *completion_type;
		QLabel *l_handles, *l_handle_type, *l_completion_type;
		QMultiLineEdit *in_win;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void sel_handle( int );
};

class dAllocEnv : public QDialog 
{
    Q_OBJECT

    public:
        dAllocEnv( OdbcTest *parent, QString name );
        ~dAllocEnv();

	protected:
		QPushButton *ok, *cancel, *help;
		QMultiLineEdit *in_win;
		QCheckBox *handle_valid;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
        void handle_clkd();
};

class dFreeEnv : public QDialog 
{
    Q_OBJECT

    public:
        dFreeEnv( OdbcTest *parent, QString name );
        ~dFreeEnv();

	protected:
		QPushButton *ok, *cancel, *help;
		QMultiLineEdit *in_win;
		QComboBox *handles;
		QLabel *l_handles;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

class dTransact : public QDialog 
{
    Q_OBJECT

    public:
        dTransact( OdbcTest *parent, QString name );
        ~dTransact();

	protected:
		QPushButton *ok, *cancel, *help;
		QMultiLineEdit *in_win;
		QComboBox *ehandles, *chandles;
		QLabel *l_ehandles, *l_chandles;
		QComboBox *completion_type;
		QLabel *l_completion_type;
		OutputWin *out_win;
		OdbcTest *odbctest;
		QString txt;

	protected slots:
		void Ok();
};

#endif
