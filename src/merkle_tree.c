#include "merkle_tree.h"

mt* mt_init(){
    mt* tree = malloc(sizeof (mt));
    if(tree == NULL){
        perror("[mt_init][malloc]");
        return NULL;
    }
    memset(tree->hash,0,HASH_SIZE);
    tree->left= NULL;
    tree->right=NULL;
    return tree;
}
void mt_print_node(mt* node){
    if(node == NULL){
        printf("merkle tree is NULL\n");
        return ;
    }
    print_hash(node->hash,HASH_SIZE,"hash");
}
void mt_print_tree(mt* tree){
    mt_print_node(tree);
    if(tree->left != NULL){
        mt_print_tree(tree->left);
    }
    if(tree->right != NULL){
        mt_print_tree(tree->right);
    }
}
uint8_t*** mt_build(uint8_t** data_blocks,size_t nb_blocks){
    assert(data_blocks != NULL && nb_blocks > 1);
    size_t depth = (size_t) ceil(log2(nb_blocks)) +1  ;
    size_t nodes_per_level = nb_blocks;
    uint8_t concat[HASH_SIZE*2];
    uint8_t hash[HASH_SIZE];
    //allocating
    uint8_t*** tree=malloc(depth*sizeof(uint8_t**));
    if(tree == NULL){
        perror("[mt_build][malloc]");
        exit(EXIT_FAILURE);
    }
    printf(ANSI_COLOR_RED"\n############# DATA BLOCKS TOTAL = %ld##################\n",nb_blocks);
#if TEST
    getc(stdin); 
#endif
    
    for(size_t i = 0 ; i < depth ; i++){
        if(nodes_per_level % 2 != 0 && i != depth -1)
            nodes_per_level++;
        tree[i] = malloc(nodes_per_level* sizeof(uint8_t*));
        if(tree[i] == NULL){
            
            perror("[mt_build][malloc]");
            exit(EXIT_FAILURE);
        }
        for(size_t j = 0 ; j < nodes_per_level; j++){
            tree[i][j] = malloc(5 * sizeof (uint8_t));
            if(tree[i][j] == NULL){
                perror("[mt_build][malloc]");
                exit(EXIT_FAILURE);
            }
            if(i == 0 ){
                if(j != nb_blocks){
#if TEST
                    printf(ANSI_COLOR_RESET"\ntype to continue...\n");
                    getc(stdin);       
#endif //TEST
                    printf(ANSI_COLOR_RED"\n--------DATA BLOCK %ld--------\n",j);
                    print_hash(data_blocks[j],DATA_BLOCK_SIZE,ANSI_COLOR_RESET); 
                }
            }
        }
        nodes_per_level= nodes_per_level/2;
    }
    nodes_per_level = nb_blocks;
    //Hashing
    for(size_t i = 0; i < depth-1; i++){
        size_t index = 0;
        size_t guard=0;
        int duplicate = 0;
        if(nodes_per_level % 2 !=0){
            nodes_per_level++;
            duplicate = 1;
        }
        while(index < nodes_per_level){
#if TEST
                    printf(ANSI_COLOR_RESET"\ntype to continue...\n");
                    getc(stdin);       
#endif //TEST
            if(i == 0){                                                         // If first level , then just hash the data.
                if((index == nodes_per_level-1) && duplicate)                   // If its last block , copy the content if the previous one.
                   memcpy(tree[i][index], tree[i][index-1],HASH_SIZE); 
                else{
                    tth_t_calc_hash(hash,data_blocks[index],DATA_BLOCK_SIZE);
                    memcpy(tree[i][index], hash,HASH_SIZE); 
                }
                PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,FIRST_LEVEL);
                print_hash(hash,HASH_SIZE,ANSI_COLOR_GREEN);
                index++;
                continue;
            }
            
            if(duplicate && index == nodes_per_level -1){
                PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,DUPLICATE);

                memcpy(tree[i][index],tree[i][index-1],HASH_SIZE);
            
                print_hash(hash,HASH_SIZE,ANSI_COLOR_GREEN);
                guard+=2;
                index++;
                continue;
            }
                
            PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,NORMAL);
            PRINT_HASH_PAIR(i,guard);
            
            memcpy(concat,tree[i-1][guard],HASH_SIZE);
		    memcpy(concat+HASH_SIZE,tree[i-1][guard+1],HASH_SIZE);

            print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE);
            tth_t_calc_hash(hash,concat,HASH_SIZE*2);
            memcpy(tree[i][index],hash,HASH_SIZE);
            
            print_hash(hash,HASH_SIZE,ANSI_COLOR_GREEN);
            guard+=2;
            index++;
        }
    nodes_per_level = nodes_per_level/2;
    }
#if TEST
    printf(ANSI_COLOR_RESET"\ntype to continue...\n");
    getc(stdin);       
#endif //TEST
    PRINT_LEVEL_INFO(depth,0,0,0,ROOT);
    PRINT_HASH_PAIR(depth-1,0);
    
    memcpy(concat,tree[depth-2][0],HASH_SIZE);
	memcpy(concat+HASH_SIZE,tree[depth-2][1],HASH_SIZE);
    print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE);

    tth_t_calc_hash(hash,concat,HASH_SIZE*2);
    memcpy(tree[depth-1][0],hash,HASH_SIZE);

    print_hash(hash,HASH_SIZE,ANSI_COLOR_GREEN);
    return tree;
}

void mt_print(uint8_t*** tree,size_t nb_blocks){
    size_t depth = (size_t) ceil(log2(nb_blocks)) +1  ;
    size_t nodes_per_level = nb_blocks;
    uint8_t concat[HASH_SIZE*2];
     for(size_t i = 0; i < depth-1; i++){
        size_t index = 0;
        size_t guard=0;
        int duplicate = 0;
        if(nodes_per_level % 2 !=0){
            nodes_per_level++;
            duplicate = 1;
        }
        while(index < nodes_per_level){
            if(i == 0){                                                         // If first level , then just hash the data.
                PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,FIRST_LEVEL);
                print_hash(tree[i][index],HASH_SIZE,ANSI_COLOR_GREEN);
                index++;
                continue;
            }
            
            if(duplicate && index == nodes_per_level -1){
                PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,DUPLICATE);
                print_hash(tree[i][index],HASH_SIZE,ANSI_COLOR_GREEN);
                guard+=2;
                index++;
                continue;
            }
                
            PRINT_LEVEL_INFO(i,index,guard,nodes_per_level,NORMAL);
            PRINT_HASH_PAIR(i,guard);
            
            memcpy(concat,tree[i-1][guard],HASH_SIZE);
		    memcpy(concat+HASH_SIZE,tree[i-1][guard+1],HASH_SIZE);
            print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE);
            print_hash(tree[i][index],HASH_SIZE,ANSI_COLOR_GREEN);
            guard+=2;
            index++;
        }
    nodes_per_level = nodes_per_level/2;
    }
    PRINT_LEVEL_INFO(depth,0,0,0,ROOT);
    PRINT_HASH_PAIR(depth-1,0);
    
    memcpy(concat,tree[depth-2][0],HASH_SIZE);
	memcpy(concat+HASH_SIZE,tree[depth-2][1],HASH_SIZE);
    print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE);
    print_hash(tree[depth-1][0],HASH_SIZE,ANSI_COLOR_GREEN);
}
int mt_proof(size_t data_index,uint8_t data_block[DATA_BLOCK_SIZE],uint8_t*** tree,size_t nb_blocks){
    if(data_index > nb_blocks){
        printf("\n"ANSI_COLOR_RED"[merkle_tree_proof][input_error] data block does not exist (usage : 0 <= data_index < nb_blocks)\n");
        exit(EXIT_FAILURE);
    }
    printf(ANSI_COLOR_YELLOW"\nGENERATING MERKLE PROOF FOR DATA #%ld\n",data_index);
#if TEST
                    getc(stdin);       
#endif //TEST
    size_t depth = (size_t) ceil(log2(nb_blocks)) +1  ;
    uint8_t merkle_root_hash[HASH_SIZE];
    uint8_t concat[HASH_SIZE*2];
    size_t current_level = 0;
    size_t current_index = data_index;
    uint8_t current_hash[HASH_SIZE];
    memcpy(merkle_root_hash,tree[depth-1][0],HASH_SIZE);                //Save the starting merkle root hash
    print_hash(merkle_root_hash,HASH_SIZE,ANSI_COLOR_RESET"\nAUTHENTC MERKLE ROOT\n");
    // Update the hash of the data (level 0)
    tth_t_calc_hash(current_hash,data_block,DATA_BLOCK_SIZE);
    memcpy(tree[0][data_index],current_hash,HASH_SIZE);

    while(current_level < depth-1){
#if TEST
                    printf(ANSI_COLOR_RESET"\ntype to continue...\n");
                    getc(stdin);       
#endif //TEST
        printf(ANSI_COLOR_RED"-----[%ld][%ld]-----\n",current_level+1,current_index/2);
        if(current_index %2 == 0){                                               //if index is pair : concatenate index.index+1
            memcpy(concat,tree[current_level][current_index],HASH_SIZE);
            memcpy(concat+HASH_SIZE,tree[current_level][current_index+1],HASH_SIZE);
            PRINT_HASH_PAIR(current_level+1,current_index);
            print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE"");
        }
        else{                                                                   //if index is odd : concatenate index-1.index
            memcpy(concat,tree[current_level][current_index-1],HASH_SIZE);
            memcpy(concat+HASH_SIZE,tree[current_level][current_index],HASH_SIZE);
            PRINT_HASH_PAIR(current_level+1,current_index-1);
            print_hash(concat,HASH_SIZE*2,ANSI_COLOR_BLUE"");
        }
        
        tth_t_calc_hash(current_hash,concat,HASH_SIZE*2);                               //calculate the hash of concatenated hashes.
        memcpy(tree[current_level+1][current_index/2],current_hash,HASH_SIZE);  //Update it in the merkle tree's next level
        print_hash(current_hash,HASH_SIZE,ANSI_COLOR_RESET"HASH:");                     //So we use the correct value for next iteration.
        current_level++;
        current_index/=2;
    }
#if TEST
                    printf(ANSI_COLOR_RESET"\ntype to continue...\n");
                    getc(stdin);       
#endif //TEST   
    printf(ANSI_COLOR_RESET"\n################################\n");
    print_hash(merkle_root_hash,HASH_SIZE,ANSI_COLOR_YELLOW"VALID MERKLE ROOT");
    print_hash(current_hash,HASH_SIZE,ANSI_COLOR_RESET"MERKLE ROOT FOUND");
    if(tth_t_compare(current_hash,merkle_root_hash) == 0){                              //Compare the merkle root hash found with the starting one.
        printf(ANSI_COLOR_GREEN"VALID\n");
        printf(ANSI_COLOR_RESET"################################\n");
        return 1;
    }
    printf(ANSI_COLOR_RED"CORRUPTED\n");
    printf(ANSI_COLOR_RESET"################################\n");
    return 0;
    
}

size_t mt_find_corrupt_data(uint8_t*** authentic,uint8_t*** check,size_t nb_blocks){
    size_t depth = (size_t) ceil(log2(nb_blocks)) +1  ;
    size_t current_index = 0;
    for(int level = depth-2 ;level >= 0 ; level--){
        printf("level = %ld\n",level);
        for(size_t i = 0 ; i<2; i++){
            current_index+=i;
            if(tth_t_compare(authentic[level][current_index],check[level][current_index]) != 0){
                current_index =current_index*2;
                print_hash(authentic[level][current_index],HASH_SIZE,"auth");
                print_hash(check[level][current_index],HASH_SIZE,"check");
               break;
            }
        }
    }
    return 0;

}
//uint8_t ** mt_get_proof_of_inclusion
