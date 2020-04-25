#ifndef __MMU_H_
#define __MMU_H_ // only import once

#include <iostream>
#include <string>
#include <vector>

// "Might want to add type"
typedef struct Variable {
    std::string name;
    int virtual_address;
    int size;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    int _max_size;
    std::vector<Process*> _processes;
    Variable* Mmu::createVariable(std::string name, address);

public:
    Mmu(int memory_size);
    ~Mmu();

    uint32_t createProcess(int text_size, int data_size);
    void print();
};

#endif // __MMU_H_
