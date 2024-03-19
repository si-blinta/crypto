#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"

struct merkle_tree{
    int hash_number;
    uint8_t hash[HASH_SIZE];
    struct merkle_tree* left;
    struct merkle_tree* right;
};
typedef struct merkle_tree merkle_tree;
/**
 * @brief This function creates a merkle_tree with empty hash = {0,0,0,0,0}
 * @return A dynamically allocated pointer to the root of merkle tree.
*/
merkle_tree* merkle_tree_init();

/**
 *@brief This function prints a content of a merkle tree node; 
 * 
*/
void merkle_tree_print_node(merkle_tree* node);

/**
 *@brief This function prints a content of a merkle tree node; 
 * 
*/
void merkle_tree_print_tree(merkle_tree* tree);

/**
 * @brief This function put a hash value in a merkle tree.
 * @param block_number The block number of the hash.
 * @param hash The hash value to store.
 * @param tree The merkle tree.
 * 
*/
void merkle_tree_put(int block_number,uint8_t* message,size_t message_length, merkle_tree* tree);

/**
 * @brief This function builds a merkle tree from data blocks
*/
merkle_tree* merkle_tree_build(uint8_t** data_blocks,size_t nb_blocks);











#endif // MERKLE_TREE_H