#ifndef GENERATORDAEMON_H_
#define GENERATORDAEMON_H_

#include <QThread>
#include <QList>
#include <QPointF>
#include "LoginQuery.h"
#include "AddNewMarkQuery.h"
class GeneratorDaemon: public QThread
{
	QOBJECT

	// User gotten after auth
	QSharedPointer<User> m_user;

	// Track filename
	QString m_fileName;

	// List, where marks are stored
	QList<QPointF> m_marks;

	// Read all marks from file, specified in constructor, to m_marks
	void readMarks();

	// This function will run thread
	void run();

	// Query needed to recieve auth_token
	LoginQuery * m_loginQuery;

	// Query needed to send marks
	AddNewMarkQuery * m_tagQuery;

	bool m_isConnected;
public:
	GeneratorDaemon(const QString& fileName);
	~GeneratorDaemon();
private slots:
	void onTagAdded();
	void onConnected();
};
#endif
