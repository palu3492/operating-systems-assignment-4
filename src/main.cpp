#include <iostream>
#include <string>
#include "mmu.h"
#include "pagetable.h"
#include <cmath>

void printStartMessage(int page_size);
void create(int text_size, int data_size, Mmu *mmu, PageTable *pageTable, int page_size);
void allocate(int pid, std::string var_name, std::string data_type, int number_of_elements, Mmu *mmu, PageTable *pageTable, int page_size);

int main(int argc, char **argv) {
    // Ensure user specified page size as a command line parameter
    // ./memsim 1024
    if (argc < 2) {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }


    // page size must be a power of 2 between 1024 and 32768
    int page_size = std::stoi(argv[1]); // 1024 - 32768
    // check if power of 2 and between 1024 and 32768
    // log2(page_size) will be an integer if it is a power of 2
    if(log2(page_size) != int(log2(page_size)) || page_size < 1024 || page_size > 32768) {
        fprintf(stderr, "Error: page size must be a power of 2 between 1024 and 32768\n");
        return 1;
    }

    // Print opening instruction message
    printStartMessage(page_size);

    // Create physical 'memory'
    // You will not actually be spawning processes that consume memory.
    // Rather you will be creating simulated "processes" that each make
    // a series of memory allocations and deallocations.
    // Array of unsigned ints (bytes)
    uint8_t *memory = new uint8_t[67108864]; // 64 MB (64 * 1024 * 1024)

    // Create MMU
    // MMU memory size is 67108864 bytes
    Mmu *mmu = new Mmu(67108864);

    PageTable *pageTable = new PageTable(page_size);

    // Prompt loop
    // Your simulator should continually ask the user to input a command.
    std::string command; // create, allocate, set, free, terminate, print
    std::cout << "> ";
    std::getline(std::cin, command);
    while (command != "exit") {
        // Handle command
        // New function to handle each command
        // TODO: need to get command by splitting on space
        std::cout << "Command: " << command << std::endl;

        if(command == "create"){
            // create <text_size> <data_size>
            int text_size = 2048;
            int data_size = 1024;
            create(text_size, data_size, mmu, pageTable, page_size);
        } else if (command == "allocate") {
            // allocate <PID> <var_name> <data_type> <number_of_elements>
            allocate(1024, "var", "int", 10, mmu, pageTable, page_size);


            // set
            // free
            // terminate
            // print
        } else {
            std::cout << command << " is not a valid command." << std::endl;
        }

        // Get next command
        std::cout << "> ";
        std::getline(std::cin, command);
    }


    return 0;
}

void printStartMessage(int page_size) {
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes."
              << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std::endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)"
              << std::endl;
    std::cout
            << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)"
            << std::endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)"
              << std::endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std::endl;
    std::cout << "  * print <object> (prints data)" << std::endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std::endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std::endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running"
              << std::endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process"
              << std::endl;
    std::cout << std::endl;
}

/*
 * create <text_size> <data_size>
 * - Initializes a new process
 *   - Assign a PID - unique number (start at 1024 and increment up)
 *   - Allocate some amount of startup memory for the process
 *     - Text/Code: user specified number (2048 - 16384 bytes)
 *     - Data/Globals: user specified number (0 - 1024 bytes)
 *     - Stack: constant (65536 bytes)
 * - Prints the PID
 *
 * > create 5992 564
 * return: 1024
 */
void create(int text_size, int data_size, Mmu *mmu, PageTable *pageTable, int page_size) {
    /* TODO:
     * text_size needs to be between 2048 and 16384
     * data_size needs to be between 0 and 1024
    */

    // Create process and get returned the pid
    int process_pid = mmu->createProcess(text_size, data_size); // pid starts at 1024
    std::cout << "pid: " << process_pid << std::endl;

    // Allocate some amount of startup memory for the process
    int stack_size = 65536; // Stack is constant 65536 bytes
    int total_size = stack_size + text_size + data_size;
    // number of pages needed to fit all of total_size
    int number_of_pages = total_size / page_size; // integer division (5/2 = 3 pages)
    std::cout << "setup memory size: " << total_size << std::endl;
    std::cout << "number of pages: " << number_of_pages << std::endl;
    // add a new page for each page needed to fit total_size
    for(int page_number = 0; page_number < number_of_pages; page_number++){
        pageTable->addEntry(process_pid, page_number);
    }
}
/*
Allocate memory on the heap
  - N chars (N bytes)
  - N shorts (N * 2 bytes)
  - N ints / floats (N * 4 bytes)
  - N longs / doubles (N * 8 bytes)

allocate <PID> <var_name> <data_type> <number_of_elements>
        Allocated memory on the heap (how much depends on the data type and the number of elements)
Print the virtual memory address
 */
void allocate(int pid, std::string var_name, std::string data_type, int number_of_elements, Mmu *mmu, PageTable *pageTable, int page_size){

}