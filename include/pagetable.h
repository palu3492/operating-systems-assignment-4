#ifndef __PAGETABLE_H_
#define __PAGETABLE_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

class PageTable {
private:
    int _page_size;
    // <"pid|page_number", frame>
    std::map<std::string, int> _table;
    std::vector<int> _frames;

public:
    PageTable(int page_size);

    ~PageTable();

    void addEntry(uint32_t pid, int page_number);

    void removeEntry(uint32_t pid, int page_number);

    int getPhysicalAddress(uint32_t pid, int virtual_address);

    void print();

    void splitEntry(std::string *entry, std::string *pid, std::string *page_number);
};

#endif // __PAGETABLE_H_
