#ifndef DATABASEIMPORTCONTROLLER_H
#define DATABASEIMPORTCONTROLLER_H

#include <QObject>
#include <QSet>

#include "databaseimporter.h"
#include "signetapplication.h"

class LoggedInWidget;
class ButtonWaitWidget;
class esdbGenericModule;
class QDialog;
class QProgressBar;
class QStackedWidget;

class DatabaseImportController : public QObject
{
	Q_OBJECT
	LoggedInWidget *m_loggedInWidget;
	DatabaseImporter *m_importer;
	DatabaseImporter::database *m_db;
	DatabaseImporter::databaseIter m_dbIter;
	DatabaseImporter::databaseIter m_dbIterStart;
	DatabaseImporter::databaseTypeIter m_dbTypeIter;
	ButtonWaitWidget *m_buttonWaitWidget;
	bool m_overwriteAll;
	bool m_skipAll;
	void advanceDbTypeIter();
	bool nextEntry();
	QString progressString();
	int m_signetdevCmdToken;
	bool m_updatePending;
	bool iteratorsAtEnd();
	bool m_useUpdateUids;

	QDialog *m_importProgressDialog;
	QStackedWidget *m_importProgressStack;
	QProgressBar *m_importProgressBar;
	QLabel *m_importProgressLabel;
	QLabel *m_importConflictLabel;

	enum conflictResponse {
		CONFLICT_RESPONSE_NONE,
		CONFLICT_RESPONSE_CANCEL,
		CONFLICT_RESPONSE_OVERWRITE_ALL,
		CONFLICT_RESPONSE_OVERWRITE,
		CONFLICT_RESPONSE_SKIP_ALL,
		CONFLICT_RESPONSE_SKIP,
		CONFLICT_RESPONSE_RENAME
	};
	enum conflictResponse m_conflictResponse;
	void conflictResponse();
	bool m_importCancel;
	QSet<int> m_reservedIds;
	QSet<int> m_reservedTypeIds;
	esdbEntry *m_entry;
	QString m_typeName;
public:
	enum importState {
		IMPORT_STATE_NO_SOURCE,
		IMPORT_STATE_READING,
		IMPORT_STATE_CONFLICT_RESOLUTION,
		IMPORT_STATE_CONFLICT_RESOLUTION_CANCEL,
		IMPORT_STATE_WRITING,
		IMPORT_STATE_WRITE_CANCEL,
		IMPORT_STATE_WRITE_COMPLETE
	};
	enum importState getImportState() {
		return m_importState;
	}
private:
	enum importState m_importState;
	int m_importIndex;
	QList <esdbEntry *> m_importEntries;
	QList <QString> m_importTypenames;
	QList <bool> m_importOverwrite;
	QMap <QString, int> m_typeIdMap;
	bool m_typeIdMapBuilt;
private slots:
	void buttonTimeout();
	void buttonCanceled();
	void importFinished(int);
	void cancelConflictResponse() {
		m_conflictResponse = CONFLICT_RESPONSE_CANCEL;
		conflictResponse();
	}
	void skipConflictResponse() {
		m_conflictResponse = CONFLICT_RESPONSE_SKIP;
		conflictResponse();
	}
	void skipAllConflictResponse() {
		m_conflictResponse = CONFLICT_RESPONSE_SKIP_ALL;
		conflictResponse();
	}
	void overwriteConflictResponse() {
		m_conflictResponse = CONFLICT_RESPONSE_OVERWRITE;
		conflictResponse();
	}
	void overwriteAllConflictResponse() {
		m_conflictResponse = CONFLICT_RESPONSE_OVERWRITE_ALL;
		conflictResponse();
	}
	void renameConflictResponse() {
		m_conflictResponse = CONFLICT_RESPONSE_RENAME;
		conflictResponse();
	}

	void importCancel();
public:
	explicit DatabaseImportController(DatabaseImporter *importer, LoggedInWidget *parent, bool useUpdateUids);
	DatabaseImporter *importer()
	{
		return m_importer;
	}
signals:
	void entryCreated(QString,esdbEntry*);
	void done(bool success);
	void entryChanged(int);
	void abort();

public slots:
	void importDone(bool success);
	void start();
	void signetdevCmdResp(signetdevCmdRespInfo info);
};

#endif // DATABASEIMPORTCONTROLLER_H
