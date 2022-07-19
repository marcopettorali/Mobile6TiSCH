#include "../../networkcoordinator/deduptable/deduptable.h"

bool DedupTable::checkMessage(int macAddress, int seqNum) {

	for (int i = 0; i < deduplication_table.size(); i++) {
		if (deduplication_table[i].macAddress == macAddress && deduplication_table[i].seqNum == seqNum) {
			deduplication_table[i].mark = 0;
			return false;
		}
	}
	DedupElem info;
	info.macAddress = macAddress;
	info.seqNum = seqNum;
	info.mark = 0;
	deduplication_table.push_back(info);
	return true;
}

void DedupTable::clean() {
	for (auto i = deduplication_table.begin(); i != deduplication_table.end(); ) {
		if (i->mark == 1) {
			i = deduplication_table.erase(i);
		} else {
			i->mark = 1;
		}
	}
}

