#include "keepassimporter.h"
#include "core/Database.h"
#include "core/Entry.h"
#include "core/Group.h"
#include "keepassunlockdialog.h"
#include "account.h"
#include "signetapplication.h"

#include <QFileDialog>
#include <QStringList>
#include <QFile>

KeePassImporter::KeePassImporter(QWidget *parent) :
	DatabaseImporter(parent),
	m_parent(parent)
{
}

void KeePassImporter::traverse(QString path, Group *g)
{
	const QList<Entry *> &el = g->entries();
	QList<Entry *>::const_iterator eiter;
	for (eiter = el.constBegin(); eiter != el.constEnd(); eiter++) {
		Entry *e = (*eiter);
		account *a = new account(-1);
		a->acctName = e->title();
		a->userName = e->username();
		if (isEmail(a->userName)) {
			a->email = a->userName;
		}
		a->url = e->url();
		a->password = e->password();
		if (e->notes().size()) {
			a->fields.addField(genericField("notes", "text block", e->notes()));
		}
		a->setPath(path);
		m_accountType->append(a);
	}
	const QList<Group*> &gl = g->children();
	for (auto giter = gl.constBegin(); giter != gl.constEnd(); giter++) {
		if (path.size()) {
			QString pathNext = path;
			pathNext.append('/').append((*giter)->name().replace("/", "//"));
			traverse(pathNext,  (*giter));
		} else {
			QString pathNext = path;
			pathNext.append((*giter)->name().replace("/", "//"));
			traverse(pathNext,  (*giter));
		}
	}
}

void KeePassImporter::failedToOpenDatabaseDialogFinished(int rc)
{
	Q_UNUSED(rc);
	done(false);
}
void KeePassImporter::start()
{
	QFileDialog *fd = new QFileDialog(m_parent, databaseTypeName() + " Import");
	QStringList filters;
	filters.append("*.kdbx");
	filters.append("*");
	fd->setNameFilters(filters);
	fd->setFileMode(QFileDialog::AnyFile);
	fd->setAcceptMode(QFileDialog::AcceptOpen);
	if (!fd->exec()) {
		done(false);
		return;
	}
	QStringList sl = fd->selectedFiles();
	if (sl.empty()) {
		done(false);
		return;
	}
	fd->deleteLater();

	QFile *keePassFile = new QFile(sl.first());
	if (!keePassFile->open(QFile::ReadOnly)) {
		keePassFile->deleteLater();
		auto mb = SignetApplication::messageBoxError(QMessageBox::Warning,
		                databaseTypeName()+ " Import",
		                "Failed to open KeePass database file",
		                m_parent);
		connect(mb, SIGNAL(finished(int)), this, SIGNAL(failedToOpenDatabaseDialogFinished(int)));
		return;
	}

	KeePassUnlockDialog *unlockDialog = new KeePassUnlockDialog(keePassFile, m_parent);
	unlockDialog->setWindowTitle(databaseTypeName()+ " Import");
	unlockDialog->setWindowModality(Qt::WindowModal);
	unlockDialog->exec();
	m_keePassDatabase = unlockDialog->database();
	unlockDialog->deleteLater();
	if (!m_keePassDatabase) {
		keePassFile->deleteLater();
		done(false);
		return;
	}
	keePassFile->deleteLater();
	if (!m_keePassDatabase) {
		QMessageBox *msg = new QMessageBox(QMessageBox::Warning,
		                                   databaseTypeName()+ " Import",
		                                   "Invalid credentials for KeePass database",
		                                   QMessageBox::NoButton, m_parent);
		connect(msg, SIGNAL(finished(int)), this, SLOT(invalidCredientialsDialogFinished(int)));
		msg->setWindowModality(Qt::WindowModal);
		msg->setAttribute(Qt::WA_DeleteOnClose);
		msg->show();
	} else {
		m_db = new database();
		m_accountType = new databaseType();
		m_db->insert("Accounts", m_accountType);
		traverse(QString(), m_keePassDatabase->rootGroup());
		m_keePassDatabase->deleteLater();
		done(true);
	}
}

void KeePassImporter::invalidCredientialsDialogFinished(int rc)
{
	Q_UNUSED(rc);
	m_keePassDatabase->deleteLater();
	m_keePassDatabase = nullptr;
	done(false);
}
