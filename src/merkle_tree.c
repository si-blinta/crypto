#include "merkle_tree.h"

merkle_tree* merkle_tree_init(){
    merkle_tree* tree = malloc(sizeof (merkle_tree));
    if(tree == NULL){
        perror("[merkle_tree_init][malloc]");
        return NULL;
    }
    memset(tree->hash,0,HASH_SIZE);
    tree->left= NULL;
    tree->right=NULL;
    return tree;
}
void merkle_tree_print_node(merkle_tree* node){
    if(node == NULL){
        printf("merkle tree is NULL\n");
        return ;
    }
    print_hash(node->hash,HASH_SIZE,"hash");
}
void merkle_tree_print_tree(merkle_tree* tree){
    merkle_tree_print_node(tree);
    if(tree->left != NULL){
        merkle_tree_print_tree(tree->left);
    }
    if(tree->right != NULL){
        merkle_tree_print_tree(tree->right);
    }
}
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
uint8_t*** merkle_tree_build(uint8_t** data_blocks,uint32_t nb_blocks){
    uint32_t depth = (uint32_t) ceil(log2(nb_blocks))   ;
    uint32_t nodes_per_level = nb_blocks;
#if DEBUG
    printf("blocks = %u depth = %u\n",nb_blocks,depth);
#endif
    //allocating
    uint8_t*** merkle_array=malloc(depth*sizeof(uint8_t**));
    if(merkle_array == NULL){
        perror("[merkle_tree_build][malloc]");
        exit(EXIT_FAILURE);
    }
    for(size_t i = 0 ; i < depth ; i++){
        if(nodes_per_level % 2 != 0 && i != depth -1)
            nodes_per_level++;
        merkle_array[i] = malloc(nodes_per_level* sizeof(uint8_t*));
        if(merkle_array[i] == NULL){
            perror("[merkle_tree_build][malloc]");
            exit(EXIT_FAILURE);
        }
        for(size_t j = 0 ; j < nodes_per_level; j++){
            //printf("[%d][%d]\n",i,j);
            merkle_array[i][j] = malloc(5 * sizeof (uint8_t));
            if(merkle_array[i][j] == NULL){
                perror("[merkle_tree_build][malloc]");
                exit(EXIT_FAILURE);
            }
            uint8_t hash[HASH_SIZE]; 
            if(j == nb_blocks)
                tth_t_calc_hash(hash,data_blocks[j-1],DATA_BLOCK_SIZE);
            else
                tth_t_calc_hash(hash,data_blocks[j],DATA_BLOCK_SIZE);
            memcpy(merkle_array[i][j], hash,HASH_SIZE);  
            if(i == 0 ){
                if(j == nb_blocks){
                     printf(ANSI_COLOR_RED"\n--------DATA BLOCK %ld--------\n",j);
                    print_hash(data_blocks[j-1],DATA_BLOCK_SIZE,ANSI_COLOR_RESET); 
                }
                else{
                    printf(ANSI_COLOR_RED"\n--------DATA BLOCK %ld--------\n",j);
                    print_hash(data_blocks[j],DATA_BLOCK_SIZE,ANSI_COLOR_RESET);     
                }

            }
        }
        nodes_per_level= nodes_per_level/2;
    }
    nodes_per_level = nb_blocks;
    //PRINTING RESULT
    //print_blocks(merkle_array[0],HASH_SIZE,10,nb_blocks,"level0");
    //Hashing
    for(size_t i = 1; i < depth-1; i++){
        nodes_per_level = nodes_per_level/2;
        size_t index = 0;
        int duplicate = 0;
        if(nodes_per_level % 2 !=0){
            nodes_per_level++;
            duplicate = 1;
        }
        for(size_t j = 0; j < nodes_per_level; j++){
            uint8_t concat[HASH_SIZE*2];
            uint8_t hash[HASH_SIZE];
            printf(ANSI_COLOR_RED"\n----------LEVEL %ld------------\n",i);
            printf(ANSI_COLOR_RESET"[%ld][%ld] + [%ld][%ld] => [%ld][%ld]\n",i-1,j,i-1,j+1,i,index);
		    
            print_hash(merkle_array[i-1][j],HASH_SIZE,ANSI_COLOR_CYAN);
            printf(ANSI_COLOR_RESET"       +");
            print_hash(merkle_array[i-1][j+1],HASH_SIZE,ANSI_COLOR_CYAN);
            printf(ANSI_COLOR_RESET"       =");
            
            memcpy(concat,merkle_array[i-1][j],HASH_SIZE);
		    memcpy(concat+HASH_SIZE,merkle_array[i-1][j+1],HASH_SIZE);
            
            print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE);
            tth_t_calc_hash(hash,concat,HASH_SIZE*2);
            memcpy(merkle_array[i][index],hash,HASH_SIZE);
            
            printf(ANSI_COLOR_RESET"HASHED :");
            print_hash(hash,HASH_SIZE,ANSI_COLOR_GREEN);
            j++;
            index++;
        }
    }
    /*
    //last concat
    printf(ANSI_COLOR_RED"\n----------ROOT------------\n");
    printf(ANSI_COLOR_RESET"[%d][%d] + [%d][%d] => [%d][%d]\n",depth-2,0,depth-2,1,depth-1,0);
    
    
    uint8_t concat[HASH_SIZE*2];
    uint8_t hash[HASH_SIZE];
    print_hash(merkle_array[depth-2][0],HASH_SIZE,ANSI_COLOR_CYAN);
    printf(ANSI_COLOR_RESET"       +");
    print_hash(merkle_array[depth-2][1],HASH_SIZE,ANSI_COLOR_CYAN);
    printf(ANSI_COLOR_RESET"       =");
    
    memcpy(concat,merkle_array[depth-2][0],HASH_SIZE);
	memcpy(concat+HASH_SIZE,merkle_array[depth-2][1],HASH_SIZE);
    print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE);

    tth_t_calc_hash(hash,concat,HASH_SIZE*2);
    memcpy(merkle_array[depth-1][0],hash,HASH_SIZE);

    printf(ANSI_COLOR_RESET"HASHED :");
    print_hash(hash,HASH_SIZE,ANSI_COLOR_GREEN);*/
    return merkle_array;
}





