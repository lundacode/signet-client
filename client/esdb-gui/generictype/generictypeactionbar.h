#ifndef GENERICTYPEACTIONBAR_H
#define GENERICTYPEACTIONBAR_H

#include "esdbactionbar.h"
#include "generic/esdbgenericmodule.h"
#include "esdb.h"
#include <QObject>

class GenericTypeActionBar : public EsdbActionBar
{
	Q_OBJECT
	esdbGenericModule *m_module;
	int esdbType()
	{
		return ESDB_TYPE_GENERIC_TYPE_DESC;
	}
	void defaultAction(esdbEntry *entry);
	void newInstanceUI(int id, const QString &name);
    bool accessEntryComplete(esdbEntry *entry, int intent) override;
    void deleteEntryComplete(esdbEntry *entry) override;
public:
	explicit GenericTypeActionBar(LoggedInWidget *parent, esdbTypeModule *module, bool writeEnabled, bool typeEnabled);

signals:

public slots:
	void deletePressed();
private slots:
	void entryCreated(esdbEntry *entry);
	void openEntryUI();
	void editFinished();
	void deleteConfirmDialogFinished(int rc);
};

#endif // GENERICTYPEACTIONBAR_H
