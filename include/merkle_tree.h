#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"
#include <math.h>
struct merkle_tree{
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

uint8_t*** merkle_tree_build(uint8_t** data_blocks,uint32_t nb_blocks);







#endif // MERKLE_TREE_H