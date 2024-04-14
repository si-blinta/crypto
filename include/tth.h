#ifndef TTH_H
#define TTH_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include "utils.h"
#define BLOCK_SIZE 25   
#define HASH_SIZE 5
#define DATA_BLOCK_SIZE 512
#define DEBUG 0
#define HEXA 1
#define MODIFIED 1  // rotation starts from index 0 .
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
*@brief This function generates matrices : an array of blocks ( a block is an array of size 25)
*@param padded_message : The padded_message 
*@param padded_length  : The size of the padded message
*@return : Returns a dynamically allocated matrices.YOU MUST FREE
*/
uint8_t** tth_t_generate_blocks(uint8_t* padded_message,size_t padded_length);

/**
*@brief This function calculates an empreinte for a given block
*@param block : The block to operate on
*@return : Returns a dynamically allocated empreinte.YOU MUST FREE
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

/**
 * @brief Floyd collision detection algorithm
 * @param start_message: <-
 * @param message_length: <-
 * @return: ignore.
*/
int floyd_collision(uint8_t* start_message,size_t message_length);

/**
 * @brief compares two hashes.
 * @param h1: output of TTH.
 * @param h2: output of TTH.
 * @return: 0 if equal,-1 if h1 > h2 , 1 if h2 > h1.
 * @warning: use it only with hashes of size 5.
 * 
*/
int tth_t_compare(uint8_t* h1,uint8_t* h2);
#endif //TTH_H
