#include "kvstore.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
	memTable = new SkipList;
	diskManager = new diskmanager;
}

KVStore::~KVStore()
{
	delete memTable;
	delete diskManager;
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
	if(!memTable->put(key,s)){
	    std::string filePath = ("../level/level0/" + std::to_string(TIMESTAMP) + "-0.sst");
	    SSTable *newTable = new SSTable(filePath,*(memTable->getAll()));
        diskManager->insert(newTable);

	    memTable->reset();
	    memTable->put(key,s);
	}
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string value = memTable->get(key);

    if(value.empty()){
        value = diskManager->get(key);
    }

    if(value == "~DELETE~"){
        value = "";
    }

    return value;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    std::string delVal = get(key);
    if(delVal.empty()){
        std::string value = "~DELETE~";
        put(key,value);
        return false;
    }

    bool res = memTable->del(key);

    std::string value = "~DELETE~";
    put(key,value);

    return true;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
//	memTable->reset();
//	diskManager->reset();
}
