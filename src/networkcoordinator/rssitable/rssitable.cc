#include "../../networkcoordinator/rssitable/rssitable.h"

void RssiTable::insertRSSIMeasure(int bbr_id, int mn_id, float rssi_measure) {
	auto info = rssitable.find(mn_id);
	if (info == rssitable.end()) {
		MobileNodeRSSIInfo i;
		i.dirty = 0;
		i.update(bbr_id, rssi_measure);
		rssitable.insert( { mn_id, i });
	} else {
		MobileNodeRSSIInfo *i = &info->second;
		i->update(bbr_id, rssi_measure);
	}
}

void RssiTable::clean() {
	for (auto i = rssitable.begin(); i != rssitable.end();) {
		MobileNodeRSSIInfo *info = &i->second;
		if (info->dirty) {
			i = rssitable.erase(i);
		} else {
			info->dirty = 1;
			i++;
		}
	}
}

int RssiTable::chooseBestBRForMN(int mn_id){
	auto info = rssitable.find(mn_id);
	return info->second.bbr_ids[0];
}


