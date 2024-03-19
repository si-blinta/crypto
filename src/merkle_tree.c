#include "../include/merkle_tree.h"

merkle_tree* merkle_tree_init(){
    merkle_tree* tree = malloc(sizeof (merkle_tree));
    if(tree == NULL){
        perror("[merkle_tree_init][malloc]");
        return NULL;
    }
    memset(tree->hash,0,HASH_SIZE);
    tree->left= NULL;
    tree->right=NULL;
    tree->hash_number = -1;
    return tree;
}
void merkle_tree_print_node(merkle_tree* node){
    if(node == NULL){
        printf("merkle tree is NULL\n");
        return ;
    }
#if HEXA
    char tmp[20];
    sprintf(tmp,"hash#%02x",node->hash_number);
    print_hash(node->hash,HASH_SIZE,tmp);
#else
    printf("[hash#%u\n",node->hash_number);
#endif//HEXA
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
void merkle_tree_put(int block_number,uint8_t* message,size_t message_length, merkle_tree* tree){
    tth_t_calc_hash(tree->hash,message,message_length);
    tree->hash_number = block_number;
}

merkle_tree* merkle_tree_build(uint8_t** data_blocks,size_t nb_blocks){

    merkle_tree* tree = merkle_tree_init();
    merkle_tree* hashes[nb_blocks];
    uint8_t hash_concat[HASH_SIZE*2];
    if(nb_blocks % 0 != 0)  // If the number of blocks is not pair add 1
        nb_blocks++;
    //First data blocks hash
    for(size_t i = 0; i < nb_blocks; i++){
        hashes[i] = merkle_tree_init();
        if(i == nb_blocks-1)    // Since we added 1 block , we need to check if it is the last one to use the same hash ( data_blocks[i] doesnt exist)
            merkle_tree_put(i,data_blocks[i-1],DATA_BLOCK_SIZE,hashes[i-1]);
        else
            merkle_tree_put(i,data_blocks[i],DATA_BLOCK_SIZE,hashes[i]);
    }
    //Concatenates every data block hash and put it in the parent node
    for(size_t i = 0; i < nb_blocks; i+=2){                 
        memcpy(hash_concat,hashes[i],HASH_SIZE);    
        memcpy(hash_concat + HASH_SIZE,hashes[i+1],HASH_SIZE);  
    }
    


}







