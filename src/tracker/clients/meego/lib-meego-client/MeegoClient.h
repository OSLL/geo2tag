#ifndef MEEGOCLIENT_H_
#define MEEGOCLIENT_H_

#include <QTimer>
#include <QSharedPointer>
#include <QString>
#include <QNetworkConfigurationManager>

#include "LoginQuery.h"
#include "AddNewMarkQuery.h"

#include "markshistory.h"

class MeegoClient: public QObject
{
Q_OBJECT

	int m_trackInterval;
	bool m_authentificated;

	QString m_lastError;
        QTimer * m_timer;

	LoginQuery * m_loginQuery;
	AddNewMarkQuery * m_addNewMarkQuery;
        MarksHistory * m_history;

        QSharedPointer<common::User> m_user;
        QNetworkConfigurationManager * m_netManager;
        void pause(int msecs);
public:
	MeegoClient(QObject * parent=0);
	// stub for authentification mechanizm
	void auth(QString user, QString pass);

	void startTrack();
	void stopTrack();
	bool isTracking();

	void setTrackInterval(int sec);
	int getTrackInterval();

	// default 50, maximum 200;
	void setHistoryLimit(int maxMarks);	
	int getHistoryLimit();
	void sendHistory();



        MarksHistory * getAllMarks();

	QString getLastError();

	bool isOnline();


private slots:
	void onError(QString error);
	void onAuthentificated();
	void onMarkAdded();
        void track();

public slots:
	// Network going down|up
        void onNetworkEvent(bool state);
	// on closed || on low battery
        void onGoOffEvent();
        // Send last added coordinate
        void sendLastCoordinate();
        // When history is full
        void onHistoryFull();
signals:
	void errorOccured(QString error);
};

#endif
