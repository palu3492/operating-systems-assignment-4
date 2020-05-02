#include "pagetable.h"
#include <algorithm>

PageTable::PageTable(int page_size) {
    _page_size = page_size;
}

PageTable::~PageTable() {
}

void PageTable::addEntry(uint32_t pid, int page_number) {
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    // If it does not exist yet
    if(_table.count(entry) == 0){
        // Find free frame
        // Start at 0 and increment up until a free frame is found
        int frame = 0;
        sort(_frames.begin(), _frames.end()); // sort _frames from smallest to largest
        // std::cout << "existing number of frames: " << _frames.size() << std::endl;
        // Looks for hole in frames, if non then put at end
        for (int i = 0; i <= _frames.size(); i++) {
            // check if there is an empty frame in _frames
            // if i is equal to size of _frames then all _frames were checked
            // so use the frame that follows last frame
            if (i == _frames.size() || frame != _frames[i]) {
                // found free frame
                break;
            }
            // std::cout << "frame checked: " << _frames[i] << std::endl;
            frame++;
        }
//        std::cout << "page " << page_number << " inserted at frame: " << frame << std::endl;
        _frames.push_back(frame); // store which frame is in use
        _table[entry] = frame; // add frame to table
    }
}

int PageTable::getPhysicalAddress(uint32_t pid, int virtual_address) {
    // Convert virtual address to page_number and page_offset
    int page_number = 0;
    int page_offset = 0;

    page_number = virtual_address / _page_size; // 11 / 5 = 2
    page_offset = virtual_address % _page_size; // left over is offset = 1

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0) {
        int frame = _table[entry];
        address = (frame * _page_size) + page_offset;
    }

    return address;
}

void PageTable::print() {
    std::map<std::string, int>::iterator it;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    for (it = _table.begin(); it != _table.end(); it++) {
        // TODO: this needs to be split
        // key
        std::string key = it->first;
        // value
        int frame = it->second;

        std::string pid;
        std::string page_number;
        splitEntry(&key, &pid, &page_number);

        std::cout << " " << pid << " | ";
        std::cout << std::setw(11) << std::right << page_number << " | ";
        std::cout << std::setw(12) << std::right << frame << std::endl;

    }
}

void PageTable::splitEntry(std::string *entry, std::string *pid, std::string *page_number) {
    std::string::size_type pos;
    pos = entry->find('|', 0);
    if (pos != std::string::npos) {
        *pid = entry->substr(0, pos);
        *page_number = entry->substr(pos + 1);
    }
}
