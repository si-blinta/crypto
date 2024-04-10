#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("s.png",&number_blocks);
	if(data_blocks == NULL)
		return 0;
	
	/*uint8_t tab[3][4][5];
	//first hash
	for ( size_t i = 0 ; i < 4 ; i++){
	
		tth_t_calc_hash(tab[0][i],data_blocks[i],DATA_BLOCK_SIZE);
		print_hash(tab[0][i],HASH_SIZE,"height 0");
	}
	//Second hash
	for ( size_t i = 0 ; i < 2 ; i++){
	
		uint8_t concat1[HASH_SIZE*2];
		memcpy(concat1,tab[0][i],HASH_SIZE);
		memcpy(concat1+HASH_SIZE,tab[0][i+1],HASH_SIZE);
		print_hash(concat1,HASH_SIZE*2,"concat1");
		
		tth_t_calc_hash(tab[1][i],concat1,HASH_SIZE*2);
		print_hash(tab[1][i],HASH_SIZE,"height 1");
		
	}
	//Final hash
		uint8_t concat1[HASH_SIZE*2];
		memcpy(concat1,tab[1][0],HASH_SIZE);
		memcpy(concat1+HASH_SIZE,tab[1][1],HASH_SIZE);
		print_hash(concat1,HASH_SIZE*2,"concat2");
		
		tth_t_calc_hash(tab[2][0],concat1,HASH_SIZE*2);
		print_hash(tab[2][0],HASH_SIZE,"height 2");*/
		uint8_t *** t = merkle_tree_build(data_blocks,number_blocks);
	
	
	
	return 0;
}