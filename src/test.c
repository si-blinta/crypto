#include "../include/merkle_tree.h"
#include "../include/utils.h"
#include <assert.h>
void test1(){
    printf(ANSI_COLOR_MAGENTA"\n################## TEST 1 STARTS ################\n");
    char buffer[100];
    size_t target_index;
    printf(ANSI_COLOR_RESET"type file name\n");
    fscanf(stdin,"%s",buffer);
    size_t nb_blocks = 0;
    uint8_t** data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## EXTRACTING BLOCKS FROM FILE ################\n");
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    printf(ANSI_COLOR_MAGENTA"\n################## BUILDING MERKLE TREE ################\n");
    uint8_t*** tree = merkle_tree_build(data_blocks,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    printf(ANSI_COLOR_RESET"which data block you want to validate (between 0-%ld)\n",nb_blocks-1);
    fscanf(stdin,"%ld",&target_index);
    assert(target_index >= 0 && target_index < nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## GENERATING MERKLE PROOF ################\n");
    merkle_tree_proof(target_index,data_blocks[target_index],tree,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## TEST FINISHED ################\n");
}
void test2(){
    printf(ANSI_COLOR_MAGENTA"\n################## TEST 2 STARTS ################\n");
    char buffer[100];
    uint8_t target_data[DATA_BLOCK_SIZE];
    size_t target_index;
    printf(ANSI_COLOR_RESET"type file name\n");
    fscanf(stdin,"%s",buffer);
    size_t nb_blocks = 0;
    printf(ANSI_COLOR_MAGENTA"\n################## EXTRACTING BLOCKS FROM FILE ################\n");
    uint8_t** data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    
    printf(ANSI_COLOR_MAGENTA"\n################## BUILDING MERKLE TREE ################\n");
    uint8_t*** tree = merkle_tree_build(data_blocks,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    printf(ANSI_COLOR_RESET"which data block you want to validate (between 0-%ld)\n",nb_blocks-1);
    fscanf(stdin,"%ld",&target_index);
    assert(target_index >= 0 && target_index < nb_blocks);
    
    printf(ANSI_COLOR_MAGENTA"\n################## GENERATING MERKLE PROOF ################\n");
    merkle_tree_proof(target_index,data_blocks[target_index],tree,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    getc(stdin);
    printf(ANSI_COLOR_MAGENTA"\n################## CORRUPTION OCCURED FOR DATA #%ld  ################\n",target_index);
    for(int i = 0 ; i < DATA_BLOCK_SIZE;i++){
        target_data[i] = rand()%64;
    }
    print_hash(data_blocks[target_index],DATA_BLOCK_SIZE,ANSI_COLOR_GREEN"ORIGINAL");
    print_hash(target_data,DATA_BLOCK_SIZE,ANSI_COLOR_RED"CORRUPT");   
    printf(ANSI_COLOR_MAGENTA"\n################## GENERATING MERKLE PROOF ################\n");
    merkle_tree_proof(target_index,target_data,tree,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## TEST FINISHED ################\n");
}
#if !MODIFIED
void proof_collision(){
    printf(ANSI_COLOR_MAGENTA"\n################## TEST 3 STARTS ################\n");
    size_t nb_blocks = 0;
    uint8_t target_data[DATA_BLOCK_SIZE];
    printf(ANSI_COLOR_MAGENTA"\n################## EXTRACTING BLOCKS FROM FILE ################\n");
    uint8_t** data_blocks = exctract_blocks_from_file("collision.txt",&nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    printf(ANSI_COLOR_MAGENTA"\n################## BUILDING MERKLE TREE ################\n");
    uint8_t*** tree = merkle_tree_build(data_blocks,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
     printf(ANSI_COLOR_MAGENTA"\n################## CORRUPTION OCCURED FOR DATA #%dd  ################\n",0);
    for(int i = 0 ; i < DATA_BLOCK_SIZE;i++){
        target_data[i] = rand()%64;
    }
    print_hash(data_blocks[0],DATA_BLOCK_SIZE,ANSI_COLOR_GREEN"ORIGINAL");
    print_hash(target_data,DATA_BLOCK_SIZE,ANSI_COLOR_RED"CORRUPT");  
    merkle_tree_proof(0,target_data,tree,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"#################FOUND COLLISION##############\n"ANSI_COLOR_RESET);
    uint8_t concat1[HASH_SIZE*2]={0x1f,0x4,0x1c,0x15,0x2c,0x25,0x10,0x1f,0x14,0x18};
	uint8_t concat2[HASH_SIZE*2]={0x3f,0x4,0x3c,0x15,0x2c,0x25,0x10,0x1f,0x14,0x18};
	uint8_t h1[HASH_SIZE];
	uint8_t h2[HASH_SIZE];
	tth_t_calc_hash(h1,concat1,HASH_SIZE*2);
	tth_t_calc_hash(h2,concat2,HASH_SIZE*2);
	printf("H1 :\n");
    print_hash(h1,HASH_SIZE,ANSI_COLOR_YELLOW"1f "ANSI_COLOR_RESET"04 "ANSI_COLOR_YELLOW"1c "ANSI_COLOR_RESET
        "15 2c 25 10 1f 14 18"ANSI_COLOR_MAGENTA);
	printf(ANSI_COLOR_RESET"\nH2 :\n");
    print_hash(h2,HASH_SIZE,ANSI_COLOR_YELLOW"3f "ANSI_COLOR_RESET"04 "ANSI_COLOR_YELLOW"3c "ANSI_COLOR_RESET
        "15 2c 25 10 1f 14 18"ANSI_COLOR_MAGENTA);
    printf(ANSI_COLOR_MAGENTA"\n################## TEST FINISHED ################\n");
}
#endif 



int main(){
    //test1();
#if MODIFIED
    //test2();
    size_t nb_blocks = 0;  
    uint8_t** data_blocks = exctract_blocks_from_file("src/utils.c",&nb_blocks);
    uint8_t*** auth = merkle_tree_build(data_blocks,nb_blocks);
    data_blocks[3][0]=0;
    uint8_t*** check = merkle_tree_build(data_blocks,nb_blocks);
    merkle_tree_find_corrupt_data(auth,check,nb_blocks);
#else
    proof_collision();
#endif
}