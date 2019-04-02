#include "CompareStorages.h"



CompareStorages::CompareStorages()
{
}


CompareStorages::~CompareStorages()
{
}


bool CompareStorages::operator ( )(const StorageNode* n1, const StorageNode* n2)
{
	return n1->GetF() > n2->GetF();
}
