#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"
#include <math.h>
#include <assert.h>
#define TEST 1
#define PRINT_HASH_PAIR(i, guard) \
    do { \
        print_hash(merkle_array[i-1][guard], HASH_SIZE, ANSI_COLOR_CYAN); \
        printf(ANSI_COLOR_RESET "       +"); \
        print_hash(merkle_array[i-1][guard+1], HASH_SIZE, ANSI_COLOR_CYAN); \
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
struct merkle_tree{
    uint8_t hash[HASH_SIZE];
    struct merkle_tree* left;
    struct merkle_tree* right;
};
enum log_type{
    NORMAL,
    FIRST_LEVEL,
    DUPLICATE,
    ROOT,
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

uint8_t*** merkle_tree_build(uint8_t** data_blocks,size_t nb_blocks);
void merkle_tree_print(uint8_t*** merkle_array,size_t nb_blocks);
int merkle_tree_proof(size_t data_index,uint8_t data_block[DATA_BLOCK_SIZE],uint8_t*** merkle_array,size_t nb_blocks);




#endif // MERKLE_TREE_H
