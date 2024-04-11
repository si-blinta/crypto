#ifndef UTILS_H
#define UTILS_H
#include "tth.h"
// Define ANSI escape codes for colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
/**
 * @brief This function reads a file and store it in a array of blocks of size 512.
 *        This function also make sure the values are between 0 and 63.
 * @param filename The name of the file to read from.
 * @return Returns a dynamically allocated array of blocks of size 512. YOU MUST FREE
*/
uint8_t** exctract_blocks_from_file(char* filename,size_t* number_blocks);

/**
*@brief This function prints blocks.
*@param blocks    , The array of blocks.
*@param size_of_a_block, The size of a block
*@param values_per_line, How many values to print per line.
*@param nb_blocks , The number of blocks that "blocks" contains.
*@param log_message   , log message 
*/
void print_blocks(uint8_t** blocks, size_t size_of_a_block,size_t blocks_per_line,size_t nb_blocks, char* log_message);

/**
*@brief This function prints the message
*@param message The message to print.
*@param message_length The message length.
*@param log_message    The log message.
*/
void print_hash(uint8_t* message,size_t message_length,char* log_message);












#endif //UTILS_H