#include "DedupTable.h"

bool DedupTable::checkMessage(int macAddress, int seqNum) {
	int key = macAddress * 100000 + seqNum;
	if (deduplicationTable.find(key) == deduplicationTable.end()) {
		DedupElem elem;
		elem.macAddress = macAddress;
		elem.seqNum = seqNum;
		elem.toClean = false;
		deduplicationTable[key] = elem;
		return true;
	} else {
		deduplicationTable[key].toClean = false;
		return false;
	}
}

void DedupTable::clean() {
	for (auto it = deduplicationTable.begin(); it != deduplicationTable.end();) {
		if (it->second.toClean) {
			it = deduplicationTable.erase(it);
		} else{
			it->second.toClean = true;
			++it;
		}
	}
}

