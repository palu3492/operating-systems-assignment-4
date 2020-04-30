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
    // the last byte of used virtual memory taken up by variables
    // start at 0
    int end_of_memory;
    int last_page;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    int _max_size;
    std::vector<Process*> _processes;
    Variable* createVariable(std::string name, int address, int size);

public:
    Mmu(int memory_size);
    ~Mmu();

    uint32_t createProcess();
    int addVariableToProcess(int pid, std::string name, int size, int bytes_used);
    std::vector<Variable*> getVariablesFromProcess(int pid);
    void print();
    void printProcesses();
    Process *getProcess(int pid);
};

#endif // __MMU_H_
