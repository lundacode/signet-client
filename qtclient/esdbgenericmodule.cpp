#include "esdbbookmarkmodule.h"
#include "genericactionbar.h"
#include "esdb.h"
#include "generic.h"
#include "generictypedesc.h"
#include "esdbgenericmodule.h"

class EsdbActionBar;

esdbGenericModule::esdbGenericModule(genericTypeDesc *typeDesc, LoggedInWidget *parent, bool userDefined, bool plural) :
	esdbTypeModule(plural ? typeDesc->name + "s" : typeDesc->name),
	m_parent(parent),
	m_typeDesc(typeDesc),
	m_userDefined(userDefined)
{

}

EsdbActionBar *esdbGenericModule::newActionBar()
{
	return new GenericActionBar(this, m_typeDesc, m_parent);
}

esdbEntry *esdbGenericModule::decodeEntry(int id, int revision, esdbEntry *prev, struct block *blk) const
{
	generic *g = NULL;

	if (!prev) {
		g = new generic(id);
	} else {
		g = static_cast<generic *>(prev);
	}

	switch(revision) {
	case 0:
		g->fromBlock(blk);
		break;
	}

	switch(revision) {
	case 0:
		break;
	default:
		delete g;
		g = NULL;
		break;
	}
	return g;
}
