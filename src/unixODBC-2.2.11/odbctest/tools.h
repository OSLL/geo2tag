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

#ifndef TOOLS_H
#define TOOLS_H

#include <autotest.h>
#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qbuttongroup.h>
#include "odbctest.h"

class dManageTest;
class dManageTestGroup;

class Q_EXPORT MYQListViewItem : public QListViewItem
{
    public:
        MYQListViewItem( QListView * parent, MYQListViewItem *after,
                QString label1, QString label2 = QString::null );
        MYQListViewItem( MYQListViewItem * parent, QString label1, QString label2 = QString::null,
                int test_index = 0, QString test_name = QString::null );
        MYQListViewItem( MYQListViewItem * parent, MYQListViewItem *after, QString label1, 
                QString label2 = QString::null, int test_index = 0, QString test_name = QString::null );
        void set_down( BOOL state );
        MYQListViewItem *firstChild();
        MYQListViewItem *parent();
        MYQListViewItem *nextSibling();
        int index() { return test_index; };
        QString name() { return test_name; };
        QString source() { return test_source; };
        QString group() { return test_group; };

    protected:
        void activate();

    private:
        int test_index;
        QString test_name;
        QString test_group;
        QString test_source;
};

class dNewSource : public QDialog 
{
    Q_OBJECT

    public:
		dNewSource( OdbcTest *parent, QString name, dManageTest *ptest );
        ~dNewSource();

	protected:
		QPushButton *ok, *cancel;
		OdbcTest *odbctest;
		QLineEdit *source;
        QLabel *l_dsn;
        dManageTest *parent_test;

	protected slots:
		void Ok();
};

class dNewGroup : public QDialog 
{
    Q_OBJECT

    public:
		dNewGroup( OdbcTest *parent, QString name, dManageTestGroup *ptest );
        ~dNewGroup();

	protected:
		QPushButton *ok, *cancel;
		OdbcTest *odbctest;
		QLineEdit *source;
        QLabel *l_dsn;
        dManageTestGroup *parent_test;

	protected slots:
		void Ok();
};

class dManageTest : public QDialog 
{
    Q_OBJECT

    public:
		dManageTest( OdbcTest *parent, QString name );
        ~dManageTest();

    friend class dNewSource;

	protected:
		QPushButton *close, *nw, *del;
		OdbcTest *odbctest;
		QComboBox *test_source;
        QComboBox*dsn;
		QLabel *l_dsn, *l_ts, *l_uid, *l_pwd, *l_kw;
		QLineEdit *uid, *pwd, *kw;
		QButtonGroup *param;


	protected slots:
		void Ok();
        void NewSource();
        void DelSource();
		void Activated( int index );
		void Activated( const QString &str );
};

class dManageAutoTest : public QDialog 
{
    Q_OBJECT

    public:
		dManageAutoTest( OdbcTest *parent, QString name );
        ~dManageAutoTest();

	protected:
		QPushButton *close, *add, *remove, *from;
		OdbcTest *odbctest;
		QLabel *l_avail, *l_name, *l_lib, *l_so, *l_installed, *s_from, *s_name, *s_lib;
		QListBox *lib_list, *test_list;
        char curr_dir[ 256 ];

        void SetPath( QString &str );
        void SetPath( const char *str );

	protected slots:
		void Ok();
        void From();
        void Add();
        void ListSelect( const QString &name );
        void Remove();
};

class dManageTestGroup : public QDialog 
{
    Q_OBJECT

    public:
		dManageTestGroup( OdbcTest *parent, QString name );
        ~dManageTestGroup();

    friend class dNewGroup;

	protected:
		QPushButton *close, *nw, *del, *add, *remove;
		OdbcTest *odbctest;
		QComboBox *test_source;
        QComboBox *group;
		QLabel *l_group, *l_auto, *l_sauto;
        QListBox *auto_list, *sauto_list;

        void update_test_lists( void );

	protected slots:
		void Activated( int index );
		void Activated( const QString &str );
		void Ok();
        void Add();
        void Remove();
        void New();
        void Delete();
};

class dRunAutoTests : public QDialog 
{
    Q_OBJECT

    public:
		dRunAutoTests( OdbcTest *parent, QString name );
        ~dRunAutoTests();

	protected:
		QPushButton *run_list, *ok, *cancel, *log_file;
		OdbcTest *odbctest;
		QComboBox *rlist;
		QLabel *l_tests, *l_sources, *l_log;
		QButtonGroup *output, *options;
        QListBox *sources;
        QListView *tests;
        QCheckBox *b_log_file, *b_screen, *b_debug, *b_isolate, *b_cursor;

        void add_auto_test( const char * test_name, MYQListViewItem *top, MYQListViewItem **last_test );

	protected slots:
		void Ok();
        void Log();
        void LogChanged(int);
        void CursorChanged(int);
        void TestsChanged();

    private:
        int cursor_state;
};


#endif
