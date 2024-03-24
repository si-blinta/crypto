#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("s.png",&number_blocks);
	if(data_blocks == NULL)
		return 0;
	print_blocks(data_blocks,DATA_BLOCK_SIZE,50,number_blocks,"blocks");
	//floyd_collision(data_blocks[3],number_blocks);
	uint8_t m1[]={32,32,0,0,0};
	print_hash(m1,HASH_SIZE,"m1");
	tth_t_calc_hash(m1,m1,HASH_SIZE);
	print_hash(m1,HASH_SIZE,"hash of m1");
	uint8_t m2[]={32,0,32,0,0};
	print_hash(m2,HASH_SIZE,"m2");
	tth_t_calc_hash(m2,m2,HASH_SIZE);
	print_hash(m2,HASH_SIZE,"hash of m2");
	return 0;
}
// 32 32 0 0 0 LOOP
//m1 = 32 0 32 0 0 
//m2 = 32 32 0 0 0