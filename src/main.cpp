#include <iostream>
#include <string>
#include "mmu.h"
#include "pagetable.h"
#include <cmath>
#include <cstring>

void printStartMessage(int page_size);

void splitCommand(std::string *first, std::string *second);

std::vector <std::string> splitByDelimiter(std::string data, std::string token);

void create(int text_size, int data_size, Mmu *mmu, PageTable *pageTable, int page_size);

void allocate(int pid, std::string var_name, std::string data_type, int number_of_elements, Mmu *mmu,
              PageTable *pageTable, int page_size);

void set(int pid, std::string var_name, int offset, std::vector <std::string> values, Mmu *mmu, PageTable *pageTable, int page_size, uint8_t *memory);

int addVariable(int pid, std::string var_name, int size, std::string type, Mmu *mmu, PageTable *pageTable, int page_size);

void printVariable(int pid, std::string name, Mmu *mmu, PageTable *pageTable, uint8_t *memory);

template<typename T>
void print_physical_data(int physical_address, int size, int type_size, std::vector<T> values, uint8_t *memory);

void free(int pid, std::string name, Mmu *mmu, PageTable *pageTable, int page_size);

template<typename T>
void set_physical_data(int physical_address, int offset, std::vector<std::string> values, std::vector<T> new_values, std::string type, int bytes, uint8_t *memory);

void terminate(int pid, Mmu *mmu, PageTable *pageTable, int page_size);

/*
You will not actually be spawning processes that consume memory.
Rather you will be creating simulated "processes" that each make
a series of memory allocations and deallocations.
*/
int main(int argc, char **argv) {
    // Ensure user specified page size as a command line parameter
    // ./memsim 1024
    if (argc < 2) {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // byte per page, also frame size
    int page_size = std::stoi(argv[1]); // 1024 - 32768

    // check if power of 2 and between 1024 and 32768
    // log2(page_size) will be an integer if it is a power of 2
    if (log2(page_size) != int(log2(page_size)) || page_size < 1024 || page_size > 32768) {
        fprintf(stderr, "Error: page size must be a power of 2 between 1024 and 32768\n");
        return 1;
    }

    // Print opening instruction message
    printStartMessage(page_size);

    // Create physical 'memory' 64 MB of memory
    // Array of unsigned ints (8-bit ints, bytes)
    uint8_t *memory = new uint8_t[67108864]; // 64 MB (64 * 1024 * 1024)

    // Create MMU
    // MMU memory size is 67108864 bytes (how much memory we have)
    Mmu *mmu = new Mmu(67108864);

    // Create page table using supplied page_size
    PageTable *pageTable = new PageTable(page_size);

    // Prompt loop
    // Your simulator should continually ask the user to input a command.
    std::string command; // create, allocate, set, free, terminate, print
    std::cout << "> ";
    std::getline(std::cin, command); // get line typed and store as command

    // Holds everything after the first space
    std::string command_data;
    // Parse command into command and its arguments
    splitCommand(&command, &command_data);
    // Holds command arguments, different for each command
    std::vector <std::string> arguments;

    // while the user doesn't type 'exit' command, keep asking for commands
    while (command != "exit") {

        arguments = splitByDelimiter(command_data, " ");

        // Each command is handled in its own function
        if (command == "create") {
            // create <text_size> <data_size>
            if (arguments.size() != 2) {
                std::cout << command << " " << command_data << " is not a valid command." << std::endl;
                std::cout << command << " " << command_data << " does not have the correct number of arguments."
                          << std::endl;
            } else {
                int text_size = std::stoi(arguments[0]);
                int data_size = std::stoi(arguments[1]);

                create(text_size, data_size, mmu, pageTable, page_size);
            }
        } else if (command == "allocate") {
            // allocate <PID> <var_name> <data_type> <number_of_elements>
            if (arguments.size() != 4) {
                std::cout << command << " " << command_data << " is not a valid command." << std::endl;
                std::cout << command << " " << command_data << " does not have the correct number of arguments."
                          << std::endl;
            } else {
                int pid = std::stoi(arguments[0]);
                std::string var_name = arguments[1];
                std::string data_type = arguments[2];
                int number_of_elements = std::stoi(arguments[3]);

                allocate(pid, var_name, data_type, number_of_elements, mmu, pageTable, page_size);
                // allocate(1024, "var1", "int", 10, mmu, pageTable, page_size);
            }
        } else if (command == "set") {
            // set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N>
            if (arguments.size() < 4) {
                std::cout << command << " " << command_data << " is not a valid command." << std::endl;
                std::cout << command << " " << command_data << " does not have enough arguments." << std::endl;
            } else {
                int pid = std::stoi(arguments[0]);
                std::string var_name = arguments[1];
                int offset = std::stoi(arguments[2]);
                std::vector <std::string> values;

                for (int i = 3; i < arguments.size(); i++) {
                    values.push_back(arguments[i]);
                }

                set(pid, var_name, offset, values, mmu, pageTable, page_size, memory);
                // set(1024, "var1", 0, values, mmu, pageTable, page_size, memory);
            }
        } else if (command == "print") {
            if (command_data == "mmu") {
                mmu->print();
            } else if (command_data == "page") {
                pageTable->print();
            } else if (command_data == "processes") {
                mmu->printProcesses();
            } else {
                arguments = splitByDelimiter(command_data, ":");
                int pid = std::stoi(arguments[0]);
                std::string var_name = arguments[1];
                printVariable(pid, var_name, mmu, pageTable, memory);
                // <PID>:<var_name>
            }
        } else if (command == "free") {
            // free <PID> <var_name>
            if (arguments.size() != 2) {
                std::cout << command << " " << command_data << " is not a valid command." << std::endl;
                std::cout << command << " " << command_data << " does not have enough arguments." << std::endl;
            } else {
                int pid = std::stoi(arguments[0]);
                std::string var_name = arguments[1];
                free(pid, var_name, mmu, pageTable, page_size);
            }
        } else if (command == "terminate") {
            if (arguments.size() != 1) {
                std::cout << command << " " << command_data << " is not a valid command." << std::endl;
                std::cout << command << " " << command_data << " does not have enough arguments." << std::endl;

            } else {
                int pid = std::stoi(arguments[0]);
                terminate(pid, mmu, pageTable, page_size);
            }
        } else {
            std::cout << command << " is not a valid command." << std::endl;
        }

        // Get next command
        std::cout << "> ";
        std::getline(std::cin, command);

        command_data = "";
        splitCommand(&command, &command_data);
    }

    return 0;
}

// Splits the input command at the first space into a command and its arguments
void splitCommand(std::string *first, std::string *second) {
    std::string::size_type pos;
    pos = first->find(' ', 0);
    if (pos != std::string::npos) {
        *second = first->substr(pos + 1);
        *first = first->substr(0, pos);
    }
}

// Splits the input command arguments with a space delimiter
std::vector <std::string> splitByDelimiter(std::string data, std::string token) {
    std::vector <std::string> result;
    while (data.size()) {
        int index = data.find(token);
        if (index != std::string::npos) {
            result.push_back(data.substr(0, index));
            data = data.substr(index + token.size());
            if (data.size() == 0) result.push_back(data);
        } else {
            result.push_back(data);
            data = "";
        }
    }
    return result;
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

    // text_size needs to be between 2048 and 16384
    // data_size needs to be between 0 and 1024
    // Print error if not
    if (text_size < 2048 || text_size > 16384 || data_size < 0 || data_size > 1024) {
        std::cout << "Text/Code size needs to be between 2048 and 16384 bytes. ";
        std::cout << "Data/Globals size needs to be between 0 and 1024 bytes. " << std::endl;
        return;
    }
    int stack_size = 65536; // Stack is constant 65536 bytes

    // Create process and get returned the pid (process id)
    int pid = mmu->createProcess(); // pid starts at 1024
    std::cout << pid << std::endl;

    // Create <TEXT>, <GLOBALS>, and <STACK> variables
    addVariable(pid, "<TEXT>", text_size, "char", mmu, pageTable, page_size);
    addVariable(pid, "<GLOBALS>", data_size, "char", mmu, pageTable, page_size);
    addVariable(pid, "<STACK>", stack_size, "char", mmu, pageTable, page_size);
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
void allocate(int pid, std::string var_name, std::string data_type, int number_of_elements, Mmu *mmu, PageTable *pageTable, int page_size) {

    std::map<std::string, int> data_type_map = {
            {"char", 1},
            {"short", 2},
            {"int", 4},
            {"float", 4},
            {"long", 8},
            {"double", 8}
    };

    if(data_type_map.count(data_type) == 0){
        std::cout << data_type << " is not a valid data_type." << std::endl;
        return;
    }

    int number_of_bytes = number_of_elements;
    number_of_bytes *= data_type_map[data_type];

    int var_virtual_address = addVariable(pid, var_name, number_of_bytes, data_type, mmu, pageTable, page_size);
    // if can't fit in memory then error
    if(var_virtual_address == -1) {
        std::cout << "Allocation would exceed system memory. No allocation performed." << std::endl;
    } else {
        // print virtual address
        std::cout << var_virtual_address << std::endl;
    }
}

int addVariable(int pid, std::string var_name, int size, std::string type, Mmu *mmu, PageTable *pageTable, int page_size) {
    // Use first fit algorithm within a page when allocating new data

    // Add variable to process
    int var_virtual_address = mmu->addVariableToProcess(pid, var_name, size, type);
    // Allocation would exceed system memory. No allocation performed.
    if(var_virtual_address != -1){
        // Add pages needed to store variable
        int starting_page = var_virtual_address / page_size;
        // will always be on 1 page
        int number_of_pages = 1;
        // how much space it takes up on page that it partially fills
        int reverse_offset = page_size - (var_virtual_address % page_size);
        int size_after_first_page = (size - reverse_offset);
        // number of pages past starting page that it needs to fit whole variable
        if(size_after_first_page > 0){
            number_of_pages += 1;
            number_of_pages += size_after_first_page / page_size;
        }

//        std::cout << "number_of_pages: " << number_of_pages << std::endl;

        for(int i = 0; i < number_of_pages; i++){
            pageTable->addEntry(pid, starting_page + i);
//            std::cout << "page: " << starting_page + i << std::endl;
        }
    }

    return var_virtual_address;
}

void set(int pid, std::string var_name, int offset, std::vector <std::string> values, Mmu *mmu, PageTable *pageTable, int page_size, uint8_t *memory) {

    Variable* variable = mmu->getVariableFromProcess(pid, var_name);

    int virtual_address = variable->virtual_address;
    std::string type = variable->type;
    int physical_address = pageTable->getPhysicalAddress(pid, virtual_address);

    if(type == "char"){
        std::vector<char> new_values;
        set_physical_data(physical_address, offset, values, new_values, type, 1, memory);
    } else if(type == "short"){
        std::vector<short> new_values;
        set_physical_data(physical_address, offset, values, new_values, type, 2, memory);
    } else if(type == "int"){
        std::vector<int> new_values;
        set_physical_data(physical_address, offset, values, new_values, type, 4, memory);
    } else if(type == "float"){
        std::vector<float> new_values;
        set_physical_data(physical_address, offset, values, new_values, type, 4, memory);
    } else if(type == "long"){
        std::vector<long> new_values;
        set_physical_data(physical_address, offset, values, new_values, type, 8, memory);
    } else if(type == "double"){
        std::vector<double> new_values;
        set_physical_data(physical_address, offset, values, new_values, type, 8, memory);
    }
}

template<typename T>
void set_physical_data(int physical_address, int offset, std::vector<std::string> values, std::vector<T> new_values, std::string type, int bytes, uint8_t *memory){
    for(int i = 0; i < values.size(); i++){
        if(type == "char"){
            new_values.push_back(values[i].at(0));
        } else if(type == "short"){
            new_values.push_back(std::stoi(values[i]));
        } else if(type == "int"){
            new_values.push_back(std::stoi(values[i]));
        } else if(type == "float"){
            new_values.push_back(std::stof(values[i]));
        } else if(type == "long"){
            new_values.push_back(std::stol(values[i]));
        } else if(type == "double"){
            new_values.push_back(std::stod(values[i]));
        }
    }
    physical_address += (offset * bytes);
    std::memcpy(&memory[physical_address], new_values.data(), new_values.size() * bytes);
}

void printVariable(int pid, std::string name, Mmu *mmu, PageTable *pageTable, uint8_t *memory){

    Variable* variable = mmu->getVariableFromProcess(pid, name);

    int virtual_address = variable->virtual_address;
    int physical_address = pageTable->getPhysicalAddress(pid, virtual_address);

    int size = variable->size;

    std::string type = variable->type;

    if(type == "char") {
        std::vector<char> values;
        print_physical_data(physical_address, size, 1, values, memory);
    } else if(type == "short"){
        std::vector<short> values;
        print_physical_data(physical_address, size, 2, values, memory);
    } else if(type == "int"){
        std::vector<int> values;
        print_physical_data(physical_address, size, 4, values, memory);
    } else if(type == "float"){
        std::vector<float> values;
        print_physical_data(physical_address, size, 4, values, memory);
    } else if(type == "long"){
        std::vector<long> values;
        print_physical_data(physical_address, size, 8, values, memory);
    } else if(type == "double"){
        std::vector<double> values;
        print_physical_data(physical_address, size, 8, values, memory);
    }
    std::cout << std::endl;
}

template<typename T>
void print_physical_data(int physical_address, int size, int type_size, std::vector<T> values, uint8_t *memory){
    int number_of_values = size / type_size;
    values.resize(number_of_values);
    std::memcpy(&values[0], &memory[physical_address], size);

    for(int i = 0; i < number_of_values; i++){
        if(i > 3){
            std::cout << "... [" << number_of_values << " items]";
            break;
        }
        std::cout << values[i];
        if(i < number_of_values-1){
            std::cout << ", ";
        }
    }
}

void free(int pid, std::string name, Mmu *mmu, PageTable *pageTable, int page_size){
    // Maybe check if variable exists first
    Variable *variable = mmu->getVariableFromProcess(pid, name);
    variable->name = "<FREE_SPACE>";

    int size = variable->size;
    int virtual_address = variable->virtual_address;

    int first_page_number = virtual_address / page_size;
    int last_page_number = (virtual_address + size) / page_size;

    // remove table entries and frames
    // only remove pages between first and last pages
    for(int page = first_page_number+1; page < last_page_number; page++){
        pageTable->removeEntry(pid, page);
        // std::cout << "remove " << page << std::endl;
    }

    // remove first and last pages if there are no other variables on those pages
    Process *process = mmu->getProcess(pid);
    std::vector<Variable*> variables = process->variables;

    bool first_has_variables = false;
    bool last_has_variables = false;

    for (int i = 0; i < variables.size(); i++) {
        if(variables[i]->name != "<FREE_SPACE>") {
            int virtual_address = variables[i]->virtual_address;
            int size = variables[i]->size;
            // first and last pages that this free space variable is on
            int starting_page = virtual_address / page_size;
            int ending_page = (virtual_address + size) / page_size;
            // checks if there is a variable on first or last page of newly freed variable
            if (starting_page == first_page_number || ending_page == first_page_number) {
                first_has_variables = true;
            }
            if (starting_page == last_page_number || ending_page == last_page_number) {
                last_has_variables = true;
            }
        }
    }
    // Remove table entry if there are no variables on that page
    if(!first_has_variables){
        pageTable->removeEntry(pid, first_page_number);
    }
    if(!last_has_variables){
        pageTable->removeEntry(pid, last_page_number);
    }

    // find all free space and join if possible
    mmu->joinFreeSpace(pid);
}

void terminate(int pid, Mmu *mmu, PageTable *pageTable, int page_size){
    mmu->terminateProcess(pid);
    pageTable->removeProcess(pid);
}
