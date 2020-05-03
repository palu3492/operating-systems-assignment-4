#include "mmu.h"
#include <iomanip>

Mmu::Mmu(int memory_size) {
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu() {
    // destructor
}

uint32_t Mmu::createProcess() {
    Process *newProcess = new Process();
    newProcess->pid = _next_pid; // Assign a PID

    // Initialize process with empty FREE_SPACE variable which is the size of memory
    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->virtual_address = 0;
    var->size = _max_size;
    newProcess->variables.push_back(var);

    _processes.push_back(newProcess); // Push process onto back of processes Vector

    _next_pid++; // increment pid for next process
    return newProcess->pid;
}

Process *Mmu::getProcess(int pid) {
    for (int i = 0; i < _processes.size(); i++) {
        if (_processes[i]->pid == pid) {
            return _processes[i];
        }
    }
}

int Mmu::addVariableToProcess(int pid, std::string name, int size, std::string type) {
    Process* process = getProcess(pid);

    Variable *var;
    int virtual_address = calculateVirtualAddress(process, size);
    var = createVariable(name, virtual_address, size, type);

    process->variables.push_back(var);

    return virtual_address;
}

int Mmu::calculateVirtualAddress(Process* process, int size){
    std::vector<Variable *> variables = process->variables;
    for (int i = 0; i < variables.size(); i++) {
        if(variables[i]->name == "<FREE_SPACE>") {
            // Will variable fit in this free space?
            if (variables[i]->size >= size) {
                int virtual_address = variables[i]->virtual_address;
                // change free space address
                variables[i]->virtual_address += size;
                // update free space size
                // TODO: remove variable if size == 0
                variables[i]->size -= size;
                return virtual_address;
            }
        }
    }
}

Variable *Mmu::createVariable(std::string name, int address, int size, std::string type) {
//    std::cout << name << " created at virtual address " << address << std::endl;
    Variable *var = new Variable();
    var->name = name;
    var->virtual_address = address;
    var->size = size;
    var->type = type;
    return var;
}

Variable *Mmu::getVariableFromProcess(int pid, std::string name){
    Process *process = getProcess(pid);
    std::vector<Variable*> variables = process->variables;
    for (int i = 0; i < variables.size(); i++) {
        if (variables[i]->name == name) {
            return variables[i];
        }
    }
}

void Mmu::joinFreeSpace(int pid){
    Process *process = getProcess(pid);
    std::vector<Variable*> variables = process->variables;
    Variable* prev_free_space_var = NULL;
    for (int i = 0; i < variables.size(); i++) {
        if (variables[i]->name == "<FREE_SPACE>") {
            if(prev_free_space_var){
                prev_free_space_var->size += variables[i]->size;
                // TODO: remove this variable instead of setting size to zero
                variables[i]->size = 0;
            } else {
                prev_free_space_var = variables[i];
            }
        }
    }
}

void Mmu::print() {
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++) {
        for (j = 0; j < _processes[i]->variables.size(); j++) {
            std::string name = _processes[i]->variables[j]->name;
            if (name != "<FREE_SPACE>") {
                // pid
                std::cout   << " "
                            << _processes[i]->pid
                            << " | ";
                // name
                std::cout   << std::left
                            << std::setw(13)
                            << name
                            << " | ";
                // virtual address
                std::cout   << std::right
                            << std::setw(4)
                            << "0x"
                            << std::setfill('0')
                            << std::setw(8)
                            << std::hex
                            << _processes[i]->variables[j]->virtual_address
                            << " | ";
                // reset to decimal and fill to nothing
                std::cout   << std::dec
                            << std::setfill(' ');
                // size
                std::cout   << std::right
                            << std::setw(10)
                            << _processes[i]->variables[j]->size;
                // end line
                std::cout<< std::endl;
            }
        }
    }
}

/*
 * Print pid of each process
 * initiated by command 'print processes'
 */
void Mmu::printProcesses() {
    for (int i = 0; i < _processes.size(); i++) {
        std::cout << _processes[i]->pid << std::endl;
    }
}
