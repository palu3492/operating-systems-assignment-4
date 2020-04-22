#include <iostream>
#include <string>
#include "mmu.h"
#include "pagetable.h"

void printStartMessage(int page_size);

int main(int argc, char **argv) {
    // Ensure user specified page size as a command line parameter
    // ./memsim 1024
    if (argc < 2) {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instruction message
    // page size must be a power of 2 (between 1024 and 32768)
    // TODO: check if power of 2 and between 1024 and 32768
    int page_size = std::stoi(argv[1]); // 1024 - 32768
    printStartMessage(page_size);

    // Create physical 'memory'
    // You will not actually be spawning processes that consume memory.
    // Rather you will be creating simulated "processes" that each make
    // a series of memory allocations and deallocations.
    // Array of unsigned ints (bytes)
    uint8_t *memory = new uint8_t[67108864]; // 64 MB (64 * 1024 * 1024)

    // Create MMU
    // MMU memory size is 67108864 bytes
    Mmu mmu = new Mmu(67108864);

    // Prompt loop
    // Your simulator should continually ask the user to input a command.
    std::string command; // create, allocate, set, free, terminate, print
    std::cout << "> ";
    std::getline(std::cin, command);
    while (command != "exit") {
        // Handle command
        // New function to handle each command
        // TODO: need to get command by splitting on space
        switch (command) {
            case "create":
                create();
                break;
            case "allocate":
                allocate();
                break;
            case "set":
                set();
                break;
            case "free": // (To Earn a B or A)
                free();
                break;
            case "terminate": // (To Earn a B or A)
                terminate();
                break;
            case "print":
                print();
                break;
            default:
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
int create() {
    int process_pid = 1024;
    return process_pid;
}