#include "../include/merkle_tree.h"
#include "../include/utils.h"
#include <assert.h>
void test(){
    char buffer[100];
    uint8_t corrupt_block[HASH_SIZE];
    size_t corrupt_index;
    printf("type file name\n");
    fscanf(stdin,"%s",buffer);
    size_t nb_blocks = 0;
    uint8_t** data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    uint8_t*** tree = merkle_tree_build(data_blocks,nb_blocks);
    printf(ANSI_COLOR_RESET"which data block you want to modify (between 0-%ld)\n",nb_blocks-1);
    fscanf(stdin,"%ld",&corrupt_index);
    assert(corrupt_index >= 0 && corrupt_index < nb_blocks);
    printf("give the new hash block(5 numbers between 0-63 sperated by space : 32 0 5 41 2)");
    fscanf(stdin,"%hhu %hhu %hhu %hhu %hhu",corrupt_block,corrupt_block+1,corrupt_block+2,corrupt_block+3,corrupt_block+4);
    memcpy(tree[0][corrupt_index],corrupt_block,HASH_SIZE);//TODO change data block not hash.
    merkle_tree_proof(corrupt_index,tree,nb_blocks);
}





int main(){
    test();

    
}