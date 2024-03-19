#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"

typedef struct{
    int hash_number;
    uint8_t hash[HASH_SIZE];
    merkle_tree* left;
    merkle_tree* right;
}merkle_tree;

/**
 * @brief This function creates a merkle_tree with empty hash = {0,0,0,0,0}
 * @return A dynamically allocated pointer to the root of merkle tree.
*/
merkle_tree* merkle_tree_init();


/**
 * @brief This function put a hash value in a merkle tree.
 * @param block_number The block number of the hash.
 * @param hash The hash value to store.
 * @param tree The merkle tree.
 * 
*/
void merkle_tree_put(int block_number,uint8_t hash[HASH_SIZE], merkle_tree* tree);












#endif // MERKLE_TREE_H