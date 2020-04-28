#include "mmu.h"

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
    proc->end_of_memory = 0;
    proc->last_page = 0;
}

Mmu::~Mmu()
{
    // destructor
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid; // Assign a PID

    _processes.push_back(proc); // Push process onto back of processes Vector

    _next_pid++; // increment pid for next process
    return proc->pid;
}

int Mmu::addVariableToProcess(int pid, std::string name, int size){
    for (int i = 0; i < _processes.size(); i++){
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

std::vector<Variable*> Mmu::getVariablesFromProcess(int pid){
    for (int i = 0; i < _processes.size(); i++){
        if(_processes[i]->pid == pid){
            return _processes[i]->variables;
        }
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

/*
 * Print pid of each process
 * initiated by command 'print processes'
 */
void Mmu::printProcesses()
{
    for (i = 0; i < _processes.size(); i++)
    {
        std::cout << _processes[i]->pid << std::endl;
    }
}
