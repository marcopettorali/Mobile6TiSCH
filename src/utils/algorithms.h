#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "utils/point/Point.h"

// FNV-1a hash function
int fnv1a_hash(int input, int seed, int W);

// returns the first number greater (if s = +1) or smaller (if s = -1) than n that is coprime with C
int coprime(int n, int C, int s = +1);

// modulo operation that always returns a positive number
inline int pmod(int a, int b) { return (a % b + b) % b; }

/* takes a mapping between elements and their position and the array of buckets, returns an allocation of the elements in buckets so that the
minimum distance between elements in the same bucket is maximized
if some elements are already allocated in buckets, initAllocation can be used as a vector of vectors of elements
maxElemsPerBucket is the maximum number of elements that can be allocated in each bucket. If -1, there is no limit */
template <typename B, typename E>
std::unordered_map<B, std::vector<E>> maxDistance(std::unordered_map<E, Point> elemPositions, std::vector<B> buckets, int maxElemsPerBucket = -1,
                                                  std::unordered_map<B, std::vector<std::pair<E, Point>>> initAllocation = {}) {
    // create allocationMap as a vector of vectors of elem_id_t
    std::unordered_map<B, std::vector<std::pair<E, Point>>> allocationMap;

    // if initAllocation is not empty, include it in allocationMap
    if (initAllocation.size() > 0) {
        for (auto it = initAllocation.begin(); it != initAllocation.end(); it++) {
            allocationMap[it->first] = it->second;
        }
    }

    // create a copy of elemPositions
    std::unordered_map<E, Point> elemIdPos = elemPositions;

    // extract first elem and remove it from the list
    E origin = elemIdPos.begin()->first;
    Point originPos = elemIdPos.begin()->second;
    elemIdPos.erase(elemIdPos.begin());

    // add first elem to first bucket (arbitrary)
    allocationMap[buckets[0]].push_back({origin, originPos});

    while (elemIdPos.size() > 0) {
        // extract closest elem to origin from elemIdPos as currentElem
        E currentElemId = std::min_element(elemIdPos.begin(), elemIdPos.end(), [originPos](std::pair<E, Point> a, std::pair<E, Point> b) {
                              return originPos.distSq(a.second) < originPos.distSq(b.second);
                          })->first;
        std::pair<E, Point> currentElem = {currentElemId, elemIdPos[currentElemId]};

        // remove closest elem from elemIdPos
        elemIdPos.erase(currentElem.first);

        // find the maximum distance between the current elem in every bucket
        double distanceSelectedBucket = 0;
        int selectedBucketIndex = -1;

        // for each bucket compute the closest elem already allocated
        for (int b = 0; b < buckets.size(); b++) {
            int bucketIndex = buckets[b];

            // if this bucket is empty, use it
            if (allocationMap[bucketIndex].size() == 0) {
                selectedBucketIndex = b;
                break;
            }

            // if this bucket is full, skip it
            if (maxElemsPerBucket != -1 && allocationMap[bucketIndex].size() >= maxElemsPerBucket) {
                continue;
            }

            // find closest element in b-th bucket already allocated
            std::pair<E, Point> closestElemInBucket = *std::min_element(
                allocationMap[bucketIndex].begin(), allocationMap[bucketIndex].end(), [currentElem](std::pair<E, Point> a, std::pair<E, Point> b) {
                    return currentElem.second.distSq(a.second) < currentElem.second.distSq(b.second);
                });

            // compute distance between current elem and closest elem in bucket
            double distance = currentElem.second.distSq(closestElemInBucket.second);

            // if distance is greater than the maximum distance found so far, update it
            if (distance >= distanceSelectedBucket) {
                distanceSelectedBucket = distance;
                selectedBucketIndex = b;
            }
        }

        if (selectedBucketIndex == -1) {
            throw cRuntimeError("No bucket found for elem %d", currentElem.first);
        }

        // allocate current elem to selected bucket
        allocationMap[buckets[selectedBucketIndex]].push_back(currentElem);

        // update origin
        origin = currentElem.first;
        originPos = currentElem.second;
    }

    // build result map
    std::unordered_map<B, std::vector<E>> result;
    for (auto it = allocationMap.begin(); it != allocationMap.end(); it++) {
        std::vector<E> elems;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            elems.push_back(it2->first);
        }
        result[it->first] = elems;
    }

    return result;
}

#endif /* ALGORITHMS_H_ */
