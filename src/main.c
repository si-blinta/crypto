#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("x.png",&number_blocks);
	if(data_blocks == NULL)
		return 0;
	uint8_t message[]={1,2,3,4,5,6,7,8,9,10,11,12,13};
	uint8_t hash[HASH_SIZE];
	tth_t_calc_hash(hash,message,13);
	print_hash(hash,HASH_SIZE,"HASH");
	return 0;
}
