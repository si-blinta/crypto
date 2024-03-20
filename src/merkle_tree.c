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

/*merkle_tree* merkle_tree_build(uint8_t** data_blocks,size_t nb_blocks){
    merkle_tree* root = merkle_tree_init();
    merkle_tree* hashes[nb_blocks];
    uint8_t hash_concat[HASH_SIZE*2];
    if(nb_blocks % 2 != 0)  // If the number of blocks is not pair add 1
        nb_blocks++;
    //First data blocks hash
    for(size_t i = 0; i < nb_blocks; i++){
        hashes[i] = merkle_tree_init();
        if(i == nb_blocks-1)    // Since we added 1 block , we need to check if it is the last one to use the same hash ( data_blocks[i] doesnt exist)
            merkle_tree_put(i,data_blocks[i-1],DATA_BLOCK_SIZE,hashes[i]);
        else
            merkle_tree_put(i,data_blocks[i],DATA_BLOCK_SIZE,hashes[i]);
    }
    //Concatenates every data block hash and put it in the parent node
    for(size_t i = 0; i < nb_blocks; i++){                  
        //skip odd numbers to have form pairs : i i+1 like [0 1] [2 3] [4 5] [6 7] 
        if(i%2 == 1)continue;
        memcpy(hash_concat,hashes[i]->hash,HASH_SIZE);   
        memcpy(hash_concat + HASH_SIZE,hashes[i+1]->hash,HASH_SIZE);
#if DEBUG
        print_hash(hashes[i]->hash,HASH_SIZE,"hash 1 "); 
        print_hash(hashes[i+1]->hash,HASH_SIZE,"hash 2 ");
        print_hash(hash_concat,HASH_SIZE*2,"concat hash");
        
#endif //DEBUG   
        //Now we need to link each two neighboors to a parent node
        
    }

}*/

merkle_tree* merkle_tree_build(uint8_t** data_blocks,size_t nb_blocks ){
    if(nb_blocks == 1){
        merkle_tree* tree = merkle_tree_init();
        tth_t_calc_hash(tree->hash,data_blocks[0],DATA_BLOCK_SIZE);
        return tree; 
    }
    merkle_tree* tree = merkle_tree_init();
    size_t left_blocks = nb_blocks / 2;
    size_t right_blocks = nb_blocks - left_blocks;
    tree->left = merkle_tree_build(data_blocks, left_blocks);
    tree->right = merkle_tree_build(data_blocks + left_blocks, right_blocks);
    uint8_t concat_hash[HASH_SIZE*2];
    memcpy(concat_hash,tree->left->hash,HASH_SIZE);
    memcpy(concat_hash+HASH_SIZE,tree->right->hash,HASH_SIZE);
    tth_t_calc_hash(tree->hash,concat_hash,HASH_SIZE*2);
    return tree;
}







