#include "utils.h"

uint8_t** exctract_blocks_from_file(char* filename,size_t* number_blocks){
	FILE* file = fopen(filename,"rb");
	if(file == NULL){
		perror("[exctract_blocks_from_file][fopen]");
		return NULL;
	}
	struct stat st;
	stat(filename, &st);
	size_t size = st.st_size;
	size_t remaining = size % DATA_BLOCK_SIZE;
	if(remaining != 0){
		size+=DATA_BLOCK_SIZE-remaining;//Padding
	}
	*number_blocks = size / DATA_BLOCK_SIZE;
	uint8_t** data_blocks = malloc(*number_blocks * sizeof(uint8_t*));
	if(data_blocks == NULL){
		perror("[exctract_blocks_from_file][first malloc]");
		return NULL;
	} 
	for(size_t i = 0; i < *number_blocks; i++){
		data_blocks[i] = malloc(DATA_BLOCK_SIZE);
		if(data_blocks[i] == NULL){
			perror("[exctract_blocks_from_file][second malloc]");
			return NULL;
		}
		memset(data_blocks[i],0,DATA_BLOCK_SIZE);
	}
	for(size_t i = 0; i < *number_blocks; i++){
		size_t bytes_read = fread(data_blocks[i],1,DATA_BLOCK_SIZE,file);
		if(bytes_read < DATA_BLOCK_SIZE){
			data_blocks[i][bytes_read] = 32; 
		}
		for(size_t j = 0 ; j < DATA_BLOCK_SIZE ; j++)data_blocks[i][j]%=64;

	}
	fclose(file);
	return data_blocks;

}
void print_hash(uint8_t* message,size_t message_length,char* log_message){
	printf("----------------%s---------------\n",log_message);
	printf("[");
	for(size_t i = 0; i < message_length; i++)
#if HEXA
	printf("%02x ",message[i]);
#else
	printf("%u ",message[i]);
#endif//HEXA
	printf("]\n");
	printf("--------------------------------------\n");

}
void print_blocks(uint8_t** blocks, size_t size_of_a_block,size_t values_per_line,size_t nb_blocks, char* log_message){	
	printf("---------------------%s----------------------------\n",log_message);
	for(size_t i = 0; i < nb_blocks; i++){
		printf("---%s#%ld----\n[",log_message,i);
		for(size_t j =0;j<size_of_a_block;j++){
			if(j%values_per_line == 0)printf("\n");
#if HEXA
			printf("%02x ",blocks[i][j]);
#else
			printf("%u ",blocks[i][j]);
#endif //HEXA			
		}
		printf("\n]\n");
		printf("--------------\n");
	}	
}