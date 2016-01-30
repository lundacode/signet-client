#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QIcon>
#include "esdb.h"

struct block;

struct account_0 : public esdbEntry_1 {
	QString acct_name;
	QString user_name;
	QString password;
	void fromBlock(block *blk);
	account_0(int id_) : esdbEntry_1(id_, ESDB_TYPE_ACCOUNT, 0)
	{
	}
	~account_0() {}
};

struct account_1 : public esdbEntry_1 {
	QString acct_name;
	QString user_name;
	QString password;
	QString url;
	void fromBlock(block *blk);
	account_1(int id_) : esdbEntry_1(id_, ESDB_TYPE_ACCOUNT, 1)
	{
	}

	void upgrade(account_0 &prev)
	{
		acct_name = prev.acct_name;
		user_name = prev.user_name;
		password = prev.password;
	}

	~account_1() {}
};

bool is_email(const QString &s);

struct account_2 : public esdbEntry_1 {
	QString acct_name;
	QString user_name;
	QString password;
	QString url;
	QString email;
	QIcon icon;
	bool hasIcon;
	void fromBlock(block *blk);
	account_2(int id_) : esdbEntry_1(id_, ESDB_TYPE_ACCOUNT, 2),
		hasIcon(false)
	{
	}

	void upgrade(account_1 &prev)
	{
		acct_name = prev.acct_name;
		user_name = prev.user_name;
		password = prev.password;
		url = prev.url;
		if (is_email(prev.user_name)) {
			email = prev.user_name;
		}
	}

	~account_2() {}
};

struct account : public esdbEntry {
	QString acctName;
	QString userName;
	QString password;
	QString url;
	QString email;
	void fromBlock(block *blk);
	void toBlock(block *blk) const;
	account(int id_) : esdbEntry(id_, ESDB_TYPE_ACCOUNT, 3, id_, 1)
	{
	}

	QString getTitle() const;
	QString getUrl() const;
	int matchQuality(const QString &search) const;

	void upgrade(account_2 &prev)
	{
		acctName = prev.acct_name;
		userName = prev.user_name;
		password = prev.password;
		email = prev.email;
		url = prev.url;
		uid = id;
	}

	~account() {}
};

#endif // ACCOUNT_H
