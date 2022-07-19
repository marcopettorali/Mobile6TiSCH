/*
 * rssitable.h
 *
 *  Created on: Nov 18, 2021
 *      Author: marco
 */

#ifndef NETWORKCOORDINATOR_RSSITABLE_RSSITABLE_H_
#define NETWORKCOORDINATOR_RSSITABLE_RSSITABLE_H_
#include <map>

using namespace std;

struct MobileNodeRSSIInfo {
	int bbr_ids[2];
	float rssi_measures[2];
	int dirty;

	void update(int bbr_id, float rssi_measure) {
		this->rssi_measures[1] = this->rssi_measures[0];
		this->bbr_ids[1] = this->bbr_ids[0];
		this->bbr_ids[0] = bbr_id;
		this->rssi_measures[0] = rssi_measure;
		this->dirty = 0;
	}
};

class RssiTable {
	map<int, MobileNodeRSSIInfo> rssitable;
public:
	void insertRSSIMeasure(int bbr_id, int mn_id, float rssi_measure);
	int chooseBestBRForMN(int mn_id);
	void clean();
};

#endif /* NETWORKCOORDINATOR_RSSITABLE_RSSITABLE_H_ */
