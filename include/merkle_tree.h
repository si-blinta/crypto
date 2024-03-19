#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"

typedef struct{
    uint8_t hash[HASH_SIZE];
    merkle_tree* left;
    merkle_tree* right;
}merkle_tree;

/**
 * @brief This function creates a merkle_tree with empty hash = {0,0,0,0,0}
 * @return A dynamically allocated pointer to the root of merkle tree.
*/
merkle_tree* merkle_tree_init();
















#endif // MERKLE_TREE_H