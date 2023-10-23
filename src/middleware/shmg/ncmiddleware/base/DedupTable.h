#ifndef NETWORKCOORDINATOR_DEDUPTABLE_DEDUPTABLE_H_
#define NETWORKCOORDINATOR_DEDUPTABLE_DEDUPTABLE_H_
#include <unordered_map>

struct DedupElem {
    int macAddress;
    int seqNum;
	bool toClean;
};

class DedupTable {
    std::unordered_map<int, DedupElem> deduplicationTable;

   public:
    bool checkMessage(int macAddress, int seqNum);
    void clean();
};

#endif /* NETWORKCOORDINATOR_DEDUPTABLE_DEDUPTABLE_H_ */
