#ifndef __MMU_H_
#define __MMU_H_ // only import once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

typedef struct Variable {
    std::string name;
    int virtual_address;
    int size;
    std::string type; // char, short, int/float, long/double
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable *> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    int _max_size;
    std::vector<Process *> _processes;

    Variable *createVariable(std::string name, int address, int size, std::string type);

    int calculateVirtualAddress(Process* process, int size);

//    Variable* findLastFreeSpaceVar(std::vector<Variable *> variables);

public:
    Mmu(int memory_size);

    ~Mmu();

    uint32_t createProcess();

    int addVariableToProcess(int pid, std::string name, int size, std::string type);

    std::vector<Variable *> getVariablesFromProcess(int pid);

    void print();

    void printProcesses();

    Process *getProcess(int pid);

    Variable *getVariableFromProcess(int pid, std::string name);

    void joinFreeSpace(int pid);
    
    void terminateProcess(int term_pid);
};

#endif // __MMU_H_
