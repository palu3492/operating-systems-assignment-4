#include "pagetable.h"
#include <algorithm>

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
}

PageTable::~PageTable()
{
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    // Find free frame
    // Start at 0 and increment up until a free frame is found
    int frame = 0;
    sort(_frames.begin(), _frames.end()); // sort _frames from smallest to largest
    std::cout << "existing number of frames: " << _frames.size() << std::endl;
    // Looks for hole in frames, if non then put at end
    for(int i = 0; i <= _frames.size(); i++){
        // check if there is an empty frame in _frames
        // if i is equal to size of _frames then all _frames were checked
        // so use the frame that follows last frame
        if(i == _frames.size() || frame != _frames[i]){
            // found free frame
            break;
        }
        std::cout << "frame checked: " << _frames[i] << std::endl;
        frame++;
    }
    std::cout << "frame: " << frame << std::endl;
    _frames.push_back(frame); // store which frame is in use
    _table[entry] = frame; // add frame to table
}

int PageTable::getPhysicalAddress(uint32_t pid, int virtual_address)
{
    // Convert virtual address to page_number and page_offset
    // TODO: implement this!
    int page_number = 0;
    int page_offset = 0;

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
    }

    return address;
}

void PageTable::print()
{
    std::map<std::string, int>::iterator it;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    for (it = _table.begin(); it != _table.end(); it++)
    {
        // TODO: print all pages
    }
}
