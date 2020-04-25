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

    // Create <TEXT>, <GLOBALS>, and <STACK> variables
    // Are we supposed to create <FREE_SPACE> here and create other 3 vars elsewhere?
    Variable *var;
    var = createVariable("<TEXT>", 0, text_size);
    proc->variables.push_back(var);
    var = createVariable("<GLOBALS>", text_size, data_size);
    proc->variables.push_back(var);
    var = createVariable("<STACK>", text_size+data_size, 65536);
    proc->variables.push_back(var);

    _processes.push_back(proc); // Push process onto back of processes Vector

    _next_pid++; // increment pid for next process
    return proc->pid;
}

Variable* Mmu::createVariable(std::string name, int address, int size){
    Variable *var = new Variable();
    var->name = name;
    var->virtual_address = address;
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
