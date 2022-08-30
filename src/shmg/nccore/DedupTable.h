#ifndef NETWORKCOORDINATOR_DEDUPTABLE_DEDUPTABLE_H_
#define NETWORKCOORDINATOR_DEDUPTABLE_DEDUPTABLE_H_
#include <vector>

using namespace std;

struct DedupElem {
	int macAddress;
	int seqNum;
	int mark;
};

class DedupTable {
	vector<DedupElem> deduplication_table;
public:
	bool checkMessage(int macAddress, int seqNum);
	void clean();
};

#endif /* NETWORKCOORDINATOR_DEDUPTABLE_DEDUPTABLE_H_ */
