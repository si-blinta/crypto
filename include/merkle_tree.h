#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"
#include <math.h>
#include <assert.h>
#define TEST 0
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
struct mt{
    uint8_t hash[HASH_SIZE];
    struct mt* left;
    struct mt* right;
};
enum log_type{
    NORMAL,
    FIRST_LEVEL,
    DUPLICATE,
    ROOT,
};
typedef struct mt mt;
/**
 * @brief This function creates a mt with empty hash = {0,0,0,0,0}
 * @return A dynamically allocated pointer to the root of merkle tree.
*/
mt* mt_init();

/**
 *@brief This function prints a content of a merkle tree node; 
 * 
*/
void mt_print_node(mt* node);

/**
 *@brief This function prints a content of a merkle tree node; 
 * 
*/
void mt_print_tree(mt* tree);

uint8_t*** mt_build(uint8_t** data_blocks,size_t nb_blocks);
void mt_print(uint8_t*** tree,size_t nb_blocks);
int mt_proof(size_t data_index,uint8_t data_block[DATA_BLOCK_SIZE],uint8_t*** tree,size_t nb_blocks);
size_t mt_find_corrupt_data(uint8_t*** authentic,uint8_t*** check,size_t nb_blocks);
size_t mt_integrity_check(uint8_t*** authentic,uint8_t*** check,size_t nb_blocks);
uint8_t * mt_get_proof_of_inclusion(uint8_t*** authentic,size_t nb_blocks,size_t data_index);

#endif // MERKLE_TREE_H
