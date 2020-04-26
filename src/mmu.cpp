#include "mmu.h"

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
    // destructor
}

uint32_t Mmu::createProcess(int text_size, int data_size)
{
    Process *proc = new Process();
    proc->pid = _next_pid; // Assign a PID
    proc->end_of_memory = 0;

    // Create <TEXT>, <GLOBALS>, and <STACK> variables
    // Are we supposed to create <FREE_SPACE> here and create other 3 vars elsewhere?
    // Could be done in main?
    addVariableToProcess(pid, "<TEXT>", text_size);
    addVariableToProcess(pid, "<GLOBALS>", data_size);
    addVariableToProcess(pid, "<STACK>", 65536);


    _processes.push_back(proc); // Push process onto back of processes Vector

    _next_pid++; // increment pid for next process
    return proc->pid;
}

int Mmu::addVariableToProcess(int pid, std::string name, int size){
    for (i = 0; i < _processes.size(); i++){
        // find process that matches supplied pid
        if(_processes[i]->pid == pid){
            Variable *var;
            // put var in virtual memory at last available byte
            int virtual_address = _processes[i]->end_of_memory;
            var = createVariable(name, virtual_address, size);
            // last available byte will now change to be what it was plus the additional added memory
            _processes[i]->end_of_memory += size;
            _processes[i]->variables.push_back(var);

            return virtual_address;
        }
    }
}

Variable* Mmu::createVariable(std::string name, int address, int size){
    std::cout << name << " created at virtual address " << address << std::endl;
    Variable *var = new Variable();
    var->name = name;
    var->virtual_address = address; // is virtual address calculated here or passed in?
    var->size = size;
    return var;
}

void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            // TODO: print all variables (excluding <FREE_SPACE> entries)
            std::string name = _processes[i]->variables[j]->name;
            if(name != "<FREE_SPACE>"){
                std::cout << " " << _processes[i]->pid << " | ";
                std::cout << name << " | ";
                // TODO: Hex?
                std::cout << _processes[i]->variables[j]->virtual_address << " | ";
                std::cout << _processes[i]->variables[j]->size << std::endl;
            }
        }
    }
}
