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
                // TODO: remove variable if size == 0
                // update free space size
                variables[i]->size -= size;
                return virtual_address;
            }
        }
    }
    // If there is no <FREE_SPACE> variable with enough room for variable
//    Variable* last_free_space_var = findLastFreeSpaceVar(variables);
//    int virtual_address = last_free_space_var->virtual_address;
//    free_space_var->virtual_address += size;
//    int offset = _max_size % free_space_var->virtual_address;
//    free_space_var->size = _max_size - offset; // should be page_size
//    return virtual_address;
}

/*
Variable* Mmu::findLastFreeSpaceVar(std::vector<Variable *> variables){
    Variable* last_free_space_var = NULL;
    for (int i = 0; i < variables.size(); i++) {
        if (variables[i]->name == "<FREE_SPACE>") {
            if (last_free_space_var == NULL || variables[i]->virtual_address > last_free_space_var->virtual_address) {
                last_free_space_var = variables[i];
            }
        }
    }
    return last_free_space_var;
}
*/

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
//            std::cout << "returning " << variables[i]->name << std::endl;
            return variables[i];
        }
    }
}

void Mmu::print() {
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++) {
        for (j = 0; j < _processes[i]->variables.size(); j++) {
            // TODO: print all variables (excluding <FREE_SPACE> entries)
            std::string name = _processes[i]->variables[j]->name;
            if (name != "<FREE_SPACE>") {
                std::cout << " " << _processes[i]->pid << " | ";
                std::cout << std::setw(13) << std::left << name << " | ";
                // TODO: convert virtual address to hex
                std::cout << std::setw(12) << std::right << _processes[i]->variables[j]->virtual_address << " | ";
                std::cout << std::setw(10) << std::right << _processes[i]->variables[j]->size << std::endl;
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
