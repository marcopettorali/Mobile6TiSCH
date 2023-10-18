#include "IEEE802154TSCHSchedule.h"

#include <math.h>

IEEE802154TSCHSchedule::IEEE802154TSCHSchedule(int slotframeLength, int numChannels) {
    this->slotframeLength = slotframeLength;
    this->numChannels = numChannels;

    // initialize schedule
    for (int ts = 0; ts < slotframeLength; ts++) {
        for (int ch = 0; ch < numChannels; ch++) {
            schedule[ts][ch] = std::vector<cell_t>();
        }
    }
}

void IEEE802154TSCHSchedule::addCell(int timeslot, int channel, mac_address_t tx, mac_address_t rx, bool shared, bool control) {
    if (timeslot >= slotframeLength || timeslot < 0) {
        throw cRuntimeError("IEEE802154TSCHSchedule::setCell: timeslot out of bounds: %d", timeslot);
    }

    if (channel >= numChannels || channel < 0) {
        throw cRuntimeError("IEEE802154TSCHSchedule::setCell: channel out of bounds: %d", channel);
    }

    cell_t newCell;

    newCell.timeslot = timeslot;
    newCell.channel = channel;
    newCell.tx = tx;
    newCell.rx = rx;
    newCell.isShared = shared;
    newCell.isControlCell = control;

    schedule[timeslot][channel].push_back(newCell);
}

void IEEE802154TSCHSchedule::removeCells(int timeslot, int channel) {
    if (timeslot >= slotframeLength || timeslot < 0) {
        throw cRuntimeError("IEEE802154TSCHSchedule::removeCells: timeslot out of bounds: %d", timeslot);
    }

    if (channel >= numChannels || channel < 0) {
        throw cRuntimeError("IEEE802154TSCHSchedule::removeCells: channel out of bounds: %d", channel);
    }

    schedule[timeslot][channel].clear();
}

std::vector<cell_t> IEEE802154TSCHSchedule::getCellsInTimeslot(int currentTs) {
    std::vector<cell_t> ret;

    for (int ch = 0; ch < numChannels; ch++) {
        for (auto cell : schedule[currentTs][ch]) {
            ret.push_back(cell);
        }
    }

    return ret;
}

IEEE802154TSCHSchedule* IEEE802154TSCHSchedule::copy() {
    // create a new schedule
    IEEE802154TSCHSchedule* newSchedule = new IEEE802154TSCHSchedule(slotframeLength, numChannels);

    // copy all the cells
    for (int ts = 0; ts < slotframeLength; ts++) {
        for (int ch = 0; ch < numChannels; ch++) {
            for (auto cell : schedule[ts][ch]) {
                newSchedule->addCell(cell.timeslot, cell.channel, cell.tx, cell.rx, cell.isShared, cell.isControlCell);
            }
        }
    }

    return newSchedule;
}

std::vector<cell_t> IEEE802154TSCHSchedule::query(mac_address_t tx, mac_address_t rx) {
    std::vector<cell_t> ret;
    for (int ch = 0; ch < numChannels; ch++) {
        for (int ts = 0; ts < slotframeLength; ts++) {
            for (auto cell : schedule[ts][ch]) {
                if (rx == NONE_ADDRESS && cell.tx == tx) {
                    ret.push_back(cell);
                } else if (tx == NONE_ADDRESS && cell.rx == rx) {
                    ret.push_back(cell);
                } else if (cell.tx == tx && cell.rx == rx) {
                    ret.push_back(cell);
                }
            }
        }
    }
    return ret;
}

void IEEE802154TSCHSchedule::printSchedule(std::ostream& out) {
    // print schedule as a formatted table, which elements are (tx, rx) couples
    out << "Slotframe size: " << slotframeLength << endl;
    for (int ch = 0; ch < numChannels; ch++) {
        out << "|";
        for (int ts = 0; ts < slotframeLength; ts++) {
            std::vector<cell_t> elem = schedule[ts][ch];
            if (elem.size() == 0) {
                out << std::setw(7) << "/  |";
                continue;
            }

            std::string ctrlStr = "";

            int normalizedAddr = (elem[0].tx > elem[0].rx ? elem[0].tx : -elem[0].rx) %
                                 10000;  // ASSUMPTION: BR MAC ADDRESS START FROM 10000, MN MAC ADDRESS START FROM 20000

            ctrlStr += std::to_string(normalizedAddr);

            if (elem[0].isControlCell) {
                ctrlStr += "C";
            }

            if (elem[0].isShared) {
                ctrlStr += "S";
            }

            if (elem.size() > 1) {
                ctrlStr += "M";
            }

            out << std::setw(6) << ctrlStr << "|";
        }
        out << endl;
    }
}

void IEEE802154TSCHSchedule::adapt(IEEE802154TSCHSchedule* newSchedule) {
    // check if the new schedule has the same size
    if (newSchedule->slotframeLength != slotframeLength || newSchedule->numChannels != numChannels) {
        throw cRuntimeError("IEEE802154TSCHSchedule::adapt: new schedule has different size. Slotframe length: %d, num channels: %d",
                            newSchedule->slotframeLength, newSchedule->numChannels);
    }

    // clear current schedule
    for (int ts = 0; ts < slotframeLength; ts++) {
        for (int ch = 0; ch < numChannels; ch++) {
            schedule[ts][ch].clear();
        }
    }

    // copy new schedule
    for (int ts = 0; ts < slotframeLength; ts++) {
        for (int ch = 0; ch < numChannels; ch++) {
            for (auto cell : newSchedule->schedule[ts][ch]) {
                addCell(cell.timeslot, cell.channel, cell.tx, cell.rx, cell.isShared, cell.isControlCell);
            }
        }
    }
}

void IEEE802154TSCHSchedule::filter(std::vector<mac_address_t> macAddresses) {
    for (int ts = 0; ts < slotframeLength; ts++) {
        for (int ch = 0; ch < numChannels; ch++) {
            // save cells in a temporary vector
            std::vector<cell_t> cells = schedule[ts][ch];

            // remove all cells
            removeCells(ts, ch);

            // reinsert only the cells regarding the given mac addresses
            for (auto cell : cells) {
                if (std::find(macAddresses.begin(), macAddresses.end(), cell.tx) != macAddresses.end() ||
                    std::find(macAddresses.begin(), macAddresses.end(), cell.rx) != macAddresses.end()) {
                    addCell(cell.timeslot, cell.channel, cell.tx, cell.rx, cell.isShared, cell.isControlCell);
                }
            }
        }
    }
}

std::vector<cell_t> IEEE802154TSCHSchedule::diff(IEEE802154TSCHSchedule* newSchedule){

    std::vector<cell_t> diffCells;

    for (int ts = 0; ts < slotframeLength; ts++) {
        for (int ch = 0; ch < numChannels; ch++) {
            
            std::vector<cell_t> cells = schedule[ts][ch];
            std::vector<cell_t> newCells = newSchedule->schedule[ts][ch];

            for (auto cell : cells) {
                if (std::find(newCells.begin(), newCells.end(), cell) == newCells.end()) {
                    diffCells.push_back(cell);
                }
            }

            for (auto cell : newCells) {
                if (std::find(cells.begin(), cells.end(), cell) == cells.end()) {
                    diffCells.push_back(cell);
                }
            }
        }
    }

    return diffCells;
}