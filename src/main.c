#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("src/merkle_tree.c",&number_blocks);
	uint8_t *** t = merkle_tree_build(data_blocks,number_blocks);
	uint8_t corrupt[HASH_SIZE]={0,2,9,8,4};
	tth_t_calc_hash(corrupt,corrupt,HASH_SIZE);
	memcpy(t[0][1],corrupt,HASH_SIZE);
	merkle_tree_proof(90,t,number_blocks);
	//merkle_tree_print(t,number_blocks);
	//print_blocks(data_blocks,DATA_BLOCK_SIZE,10,number_blocks,"BLOCK");
	return 0;
}