#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"
#include <math.h>
#include <assert.h>
#define TEST 1              //This macro enables step by step printing.

//Logging stuff, burns eyes look away.
#define PRINT_HASH_PAIR(i, guard) \
    do { \
        print_hash(tree[i-1][guard], HASH_SIZE, ANSI_COLOR_CYAN); \
        printf(ANSI_COLOR_RESET "       +"); \
        print_hash(tree[i-1][guard+1], HASH_SIZE, ANSI_COLOR_CYAN); \
        printf(ANSI_COLOR_RESET "       ="); \
    } while(0)
#define PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,type) \
    do{\
        switch(type){\
            case NORMAL:\
                printf(ANSI_COLOR_RESET"\n################################"); \
                printf(ANSI_COLOR_RED"\n--LEVEL %ld--INDEX %ld--TOTAL_NODES %ld--\n",(size_t)i,(size_t)index,(size_t)nodes_per_level); \
                printf(ANSI_COLOR_RESET"[%ld][%ld] + [%ld][%ld] => [%ld][%ld]\n",(size_t)i-1,(size_t)guard,(size_t)i-1,(size_t)guard+1,(size_t)i,(size_t)index); \
                break;\
            case FIRST_LEVEL:\
                printf(ANSI_COLOR_RED"\n--LEVEL %ld--INDEX %ld--TOTAL_NODES %ld--\n",(size_t)i,(size_t)index,(size_t)nodes_per_level);\
                printf(ANSI_COLOR_RESET"HASHED BLOCK :");\
                break;\
            case DUPLICATE:\
                printf(ANSI_COLOR_RESET"\n################################");\
                printf(ANSI_COLOR_RED"\n--LEVEL %ld--INDEX %ld--TOTAL_NODES %ld--\n",(size_t)i,(size_t)index,(size_t)nodes_per_level);\
                printf(ANSI_COLOR_YELLOW"DUPLICATE HASH\n");\
                break;\
            case ROOT:\
                printf(ANSI_COLOR_RESET"\n################################");\
                printf(ANSI_COLOR_RED"\n----------ROOT------------\n");\
                printf(ANSI_COLOR_RESET"[%ld][%ld] + [%ld][%ld] => [%ld][%ld]\n",(size_t)i-2,(size_t)0,(size_t)i-2,(size_t)1,(size_t)i-1,(size_t)0); \
                break;\
        }\
    }while(0)
enum log_type{
    NORMAL,
    FIRST_LEVEL,
    DUPLICATE,
    ROOT,
};
/**
 * @brief this function builds a merkle tree, from data blocks, each step is printed in terminal.
 * @note if you want to use it effeicently you will need to remove all printfs.
 * @param data_blocks: the array of data blocks, each block has size 512.
 * use the function extract blocks from file to generate these blocks.
 * @param nb_blocks: the number of blocks contained in the array of data blocks.
 * @return : a dynamically allocated 3D array.
 * @warning: you must free after usage.
*/
uint8_t*** mt_build(uint8_t** data_blocks,size_t nb_blocks);

/**
 * @brief this function frees merkle tree.
 * @param tree: the merkle tree to free.
 * @param nb_blocks:the number of blocks of the data blocks that the merkle tree is constructed from.
*/
void mt_destroy(uint8_t*** tree,size_t nb_blocks);

/**
 * @brief this function prints a merkle tree.
 * @param tree: the merkle tree.
 * @param nb_blocks: the number of blocks of the data blocks that the merkle tree is constructed from.
 * @note only used to debug.
*/
void mt_print(uint8_t*** tree,size_t nb_blocks);

/**
 * @brief this function checks the validity of a data block, without building the whole tree.
 * complexity Log(n) (exactly lo2(n)), n is the number of blocks of data blocks that the merkle tree is constructed from.
 * @param data_index: the index of data to validate.
 * @param data_block: the content of the block to validate.
 * @param tree: the authentic merkle tree to compare with.
 * @param nb_blocks:the number of blocks of the data blocks that the merkle tree is constructed from.
 * @warning: this function destroys tree, unfortunatly i did not have to update it.
 * @return: 1 if the data is valid, 0 if its corrupted.
*/
int mt_proof_from_merkle_tree(size_t data_index,uint8_t data_block[DATA_BLOCK_SIZE],uint8_t*** tree,size_t nb_blocks);

/**
 * @brief this function returns the index of a corrupted data block in the merkle tree.
 * complexity Log(n) (exactly lo2(n)), n is the number of blocks of data blocks that the merkle tree is constructed from.
 * @note please use when root hashes are different, i dont check it .
 * @param authentic: the authentic merkle tree.
 * @param check    : the merkle tree to check (corrupted).
 * @return: the index of the corrupted data.
*/
size_t mt_find_corrupt_data(uint8_t*** authentic,uint8_t*** check,size_t nb_blocks);

/**
 * @brief this function returns an array of indexes that are needed to validate a data block.
 * @param nb_blocks:  the number of blocks of data blocks that the merkle tree is constructed from.
 * @param data_index: the index of the data to validate.
 * @return : A dynamically allocated array.
 * @warning: you must free after usage. 
*/
size_t * mt_get_proof_of_inclusion(size_t nb_blocks,size_t data_index);

/**
 * @brief this function is similair to mt_proof_from_merkle_tree, but it takes other parameters.
 * @note use it if you cannot afford to access the full merkle tree.(memory / bandwith constraints)
 * @param data_index: the index of the data to validate.
 * @param data_block: the content of the block to validate.
 * @param root_hash:  the authentic root hash.
 * @param add_data:   the data needed to prove the validity of a data block,you must use "mt_get_proof_of_inclusion()",
 * @note :to get the indexes needed,then fill the array "add_data" with data_blocks[indexes].
 * example : data_to_request is returned by mt_get_proof_of_inclusion().
 * for(size_t level = 0 ; level < depth-1;level++){
        memcpy(add_data[level],bob_tree[level][data_to_request[level]],HASH_SIZE);
    }
   @param levels: this must be equal to depth-1. 
   @warning: please read note.
*/
int mt_proof_from_additional_data(size_t data_index,uint8_t data_block[DATA_BLOCK_SIZE],uint8_t* root_hash,uint8_t** add_data,size_t levels);
#endif // MERKLE_TREE_H
