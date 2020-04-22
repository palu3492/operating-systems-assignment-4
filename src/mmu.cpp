#include "mmu.h"
// starter code is "nowhere near complete"
Mmu::Mmu(int memory_size, int page_size, uint8_t *memory)
{
    _next_pid = 1024;
    _max_size = memory_size;
    _page_size = page_size;
    _memory = memory;
}

Mmu::~Mmu()
{
    // destructor
}

uint32_t Mmu::createProcess(int text_size, int data_size)
{
    Process *proc = new Process();
    proc->pid = _next_pid; // Assign a PID

    // TODO: Allocate some amount of startup memory for the process
    // use parameters, stack is 65536 bytes
    int stack_size = 65536;
    // so just make an array of size text_size + data_size + stack_size ?


    // Why is this created?
    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->virtual_address = 0;
    var->size = _max_size;
    proc->variables.push_back(var);

    _processes.push_back(proc); // Push process onto back of processes Vector

    _next_pid++; // increment pid for next process
    return proc->pid;
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
        }
    }
}
