#include "kvstore.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
	memTable = new SkipList;
	diskController = new DiskController;
}

KVStore::~KVStore()
{
    std::vector<PAIR> vec;
    memTable->getAll(vec);
    if(!vec.empty())
        diskController->insert(vec);

	delete memTable;
	delete diskController;
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
	if(!memTable->put(key,s)){
	    std::vector<PAIR> vec;
        memTable->getAll(vec);
        diskController->insert(vec);

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
        diskController->get(key,value);
    }

    if(value == "~DELETED~"){
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

    if(!delVal.empty())
    {
        memTable->del(key);
        std::string value = "~DELETED~";
        put(key,value);
        return true;
    }

    return false;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	memTable->reset();
	diskController->reset();
}
