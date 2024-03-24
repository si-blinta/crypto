#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include "tth.h"
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







/**
 *                  Merkle tree building
 * 
 * Step 1 : Allocate 2d array
 *      - depth = log2(number_of_blocks). take the upper bound: 3.2 -> 4   
 *      - T = malloc( depth +1 ).
 *      for (int i = 0; i < depth ; i++){
 *          if(number_of_blocks % 2 == 1)
 *              T[i] = malloc( number_of_blocks+1/i+1 );
 *          else
 *              T[i] = malloc( number_of_blocks / i);
 *      }
 *      - T[ depth ] = malloc(1);
 * Step 2 : Hashing
 *      //First hash round
 *      for( int = 0; i < number_of_blocks ; i++){
 *          T[0][i] = hash(data[i]);
 *      }
 *      if( size T[0] > number_of_blocks)
 *          T[0][size-1] = T[0][size-2]
 * 
 *      //Hash the rest
 *      for(int i = 0 ; i < depth; i++){
 *          for(int j = 0 ; j < size of T[i] ){
 *              T[i+1][j] = hash ( T[i][j*2] + T[i][ j*2 +1] );
 *          
 *          }
 *          if( size T[i] % 2 == 1){
 *              T[i]         = realloc(size T[i] + 1);
 *              T[i][size-1] = T[i][size-2]
 *          } 
 *      }
 *      T[ depth ] = hash (  T[depth-1][0] + T[depth-1][1]  ) LAST One 
 *            
 *      
 * 
 *
 * 
 * 
 * 
 * 
*/



#endif // MERKLE_TREE_H