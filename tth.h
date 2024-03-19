#ifndef TTH_H
#define TTH_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#define BLOCK_SIZE 25
#define HASH_SIZE 5
#define DEBUG 1
#define MESSAGE_SIZE 5000
/**
*@brief This function simply Pads a message.
*			This function must be called only after checking that the padding is needed.
*@param message : The message to pad 
*@param message_length : The size of the message
*@param padded_length  : The adress to store the length of the padded message. 
*@return : Returns a dynamically allocated padded message.
*/
uint8_t*  tth_t_padding(uint8_t* message, size_t message_length,size_t* padded_length);

/**
*@brief This function prints the message
*
*/
void tth_t_print_message(uint8_t* message,size_t message_length,char* info);

/**
*@brief This function generates matrices : an array of blocks ( a block is an array of size 25)
*@param padded_message : The padded_message 
*@param padded_length  : The size of the padded message
*@return : Returns a dynamically allocated matrices
*/
uint8_t** tth_t_generate_blocks(uint8_t* padded_message,size_t padded_length);
/**
*@brief This function prints blocks.
*/
void tth_t_print_blocks(uint8_t** blocks, size_t total_size);

/**
*@brief This function calculates an empreinte for a given block
*@param block : The block to operate on
*@return : Returns a dynamically allocated empreinte.
*/
uint8_t* tth_t_calc_empreinte_block(uint8_t * block);

/**
 * @brief Apply a circular right shift on all blocks.
 * @param blocks The array of blocks.
 * @param size   The size of the array. (number of blocks it contains).
*/
void tth_t_right_shift_blocks(uint8_t** blocks, size_t size);

/**
 * @brief Toy Te-tragraph Hash function.
 * 	copies the hash value in hash.
 * @param hash    Where to store the hash.
 * @param message The message to hash.
 * @param message_length the lenght of the message.
 * 
*/
void tth_t_calc_hash(uint8_t* hash,uint8_t* message, size_t message_length);


int floyd_collision();

#endif //TTH_H
