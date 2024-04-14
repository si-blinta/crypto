#include "../include/merkle_tree.h"
#include "../include/utils.h"
#include <assert.h>
#if !MODIFIED
void proof_collision(){
    printf(ANSI_COLOR_MAGENTA"\n################## TEST 3 STARTS ################\n");
    size_t nb_blocks = 0;
    uint8_t target_data[DATA_BLOCK_SIZE];
    printf(ANSI_COLOR_MAGENTA"\n################## EXTRACTING BLOCKS FROM FILE ################\n");
    uint8_t** data_blocks = exctract_blocks_from_file("collision.txt",&nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
    printf(ANSI_COLOR_MAGENTA"\n################## BUILDING MERKLE TREE ################\n");
    uint8_t*** tree = mt_build(data_blocks,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## DONE ################\n");
     printf(ANSI_COLOR_MAGENTA"\n################## CORRUPTION OCCURED FOR DATA #%dd  ################\n",0);
    for(int i = 0 ; i < DATA_BLOCK_SIZE;i++){
        target_data[i] = rand()%64;
    }
    print_hash(data_blocks[0],DATA_BLOCK_SIZE,ANSI_COLOR_GREEN"ORIGINAL");
    print_hash(target_data,DATA_BLOCK_SIZE,ANSI_COLOR_RED"CORRUPT");  
    mt_proof_from_merkle_tree(0,target_data,tree,nb_blocks);
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
void alice_bob_file_transfer(){
    printf(ANSI_COLOR_MAGENTA"\n################## ALICE BOB FILE TRANSFER TEST BEGIN ################\n");
    printf(ANSI_COLOR_RESET"\nAlice wants to download a 300GB file from Bob, she wants to ensure that\n"
                            "the file is not corrupted using "ANSI_COLOR_YELLOW"Merkle trees\n"ANSI_COLOR_RESET);
    char buffer[100];
    printf(ANSI_COLOR_RESET"[ALICE] which file you want to download ?\n");
    fscanf(stdin,"%s",buffer);
    size_t target_index;
    size_t nb_blocks = 0;
    uint8_t** bob_data_blocks;  //to free
    uint8_t** alice_data_blocks;//to free
    uint8_t*** bob_tree;        //to free
    uint8_t *** alice_tree;     //to free
    uint8_t bob_root_hash[HASH_SIZE];
    uint8_t alice_root_hash[HASH_SIZE];
    printf(ANSI_COLOR_RESET"[BOB] extracting blocks from file\n");
    bob_data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    size_t depth = (size_t) ceil(log2(nb_blocks)) +1  ;
    printf(ANSI_COLOR_RESET"[BOB] file sent block by block\n");
    getc(stdin);//scanf \n
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    alice_data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    printf(ANSI_COLOR_RED"[MAN IN THE MIDDLE] which data block you want to corrupt [0-%ld] (-1 if none)\n"ANSI_COLOR_RESET,nb_blocks-1);
    scanf("%ld",&target_index);
    assert((target_index < nb_blocks) || (target_index == -1));
    if(target_index != -1){
        for(size_t i = 0 ; i < DATA_BLOCK_SIZE;i++)
            alice_data_blocks[target_index][i]=rand()%64;
    }
    getc(stdin);//scanf \n
    printf("[ALICE] requested the authentic merkle root hash from Bob\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    printf("[BOB] building merkle tree\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    bob_tree = mt_build(bob_data_blocks,nb_blocks);
    memcpy(bob_root_hash,bob_tree[depth-1][0],HASH_SIZE);
    print_hash(bob_root_hash,HASH_SIZE,"\n[ALICE] received merkle root hash");

    printf("[ALICE] verifying the integrity of the file by building merkle tree\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    alice_tree = mt_build(alice_data_blocks,nb_blocks);
    memcpy(alice_root_hash,alice_tree[depth-1][0],HASH_SIZE);
    printf("[ALICE] comparing root hashes\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    if(tth_t_compare(bob_root_hash,alice_root_hash) == 0){
        print_hash(bob_root_hash,HASH_SIZE,ANSI_COLOR_YELLOW"Bob root hash");
        print_hash(alice_root_hash,HASH_SIZE,ANSI_COLOR_GREEN"\nAlice root hash");
        printf(ANSI_COLOR_RESET"[ALICE] root hashes are the same , the file is safe !\n");
    }
    else{
        print_hash(bob_root_hash,HASH_SIZE,ANSI_COLOR_YELLOW"Bob root hash");
        print_hash(alice_root_hash,HASH_SIZE,ANSI_COLOR_RED"\nAlice root hash");
        printf(ANSI_COLOR_RESET"[ALICE] root hashes are different , the file is not safe !\n");
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        printf("[ALICE] lets figure out which data is corrupted\n");
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        printf("[ALICE] requested the full merkle tree from Bob\n");
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        printf("[BOB] merkle tree sent\n");
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        size_t corrupted_data_index = mt_find_corrupt_data(bob_tree,alice_tree,nb_blocks);
        printf("[ALICE] requested Data #%ld from Bob\n",corrupted_data_index);
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        printf("[BOB] Data #%ld sent\n",corrupted_data_index);
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        memcpy(alice_data_blocks[corrupted_data_index],bob_data_blocks[corrupted_data_index],DATA_BLOCK_SIZE);
        printf("[ALICE] replaced the corrupted Data, lets check if its valid\n");
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        mt_proof_from_merkle_tree(corrupted_data_index,alice_data_blocks[corrupted_data_index],bob_tree,nb_blocks);
    }
    for(size_t i = 0;i < nb_blocks;i++){
        free(alice_data_blocks[i]);
        free(bob_data_blocks[i]);
    }
    free(alice_data_blocks);
    free(bob_data_blocks);
    printf("################ DESTROYING MERKLE TREES ####################\n");
    printf("destroy Alice merkle tree:\n");
    mt_destroy(alice_tree,nb_blocks);
    printf("\ndestroy Bob merkle tree:\n");
    mt_destroy(bob_tree,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## TEST FINISHED ################\n");
}
void alice_bob_file_update(){
    printf(ANSI_COLOR_MAGENTA"\n################## ALICE BOB FILE UPDATE TEST BEGIN ################\n");
    printf(ANSI_COLOR_RESET"\nAlice wants to update her 300GB file from Bob, she cannot afford to download\n"
                            "the full merkle tree to check if the update is valid, so she will use a technique\n"
                            "called "ANSI_COLOR_YELLOW"Proof of inclusion\n"ANSI_COLOR_RESET);
    char buffer[100];
    printf(ANSI_COLOR_RESET"[ALICE] which file you want to update ?\n");
    fscanf(stdin,"%s",buffer);
    size_t target_index;
    size_t nb_blocks = 0;
    uint8_t** bob_data_blocks;  //to free
    uint8_t** alice_data_blocks;//to free
    uint8_t*** bob_tree;        //to free
    uint8_t *** alice_tree;     //to free
    uint8_t bob_root_hash[HASH_SIZE];
    uint8_t alice_root_hash[HASH_SIZE];
    size_t to_check;
    printf(ANSI_COLOR_RESET"[BOB] extracting update data blocks from file\n");
    bob_data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    size_t depth = (size_t) ceil(log2(nb_blocks)) +1  ;
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);//remove scanf \n
    getc(stdin);
    printf(ANSI_COLOR_RESET"[BOB] file update data sent block by block\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    alice_data_blocks = exctract_blocks_from_file(buffer,&nb_blocks);
    printf(ANSI_COLOR_RED"[MAN IN THE MIDDLE] which update data block you want to corrupt [0-%ld] (-1 to stop)\n"ANSI_COLOR_RESET,nb_blocks-1);
    scanf("%ld",&target_index);
    while (target_index != -1)
    {
        assert((target_index < nb_blocks) || (target_index == -1));
        printf("[MAN IN THE MIDDLE] corrupted Data #%ld\n",target_index);
        if(target_index != -1){
            for(size_t i = 0 ; i < DATA_BLOCK_SIZE;i++){
                alice_data_blocks[target_index][i]=rand()%64;
            }
        }
        scanf("%ld",&target_index);
    }
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    printf("[ALICE] requested the authentic merkle root hash from Bob\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    printf("[BOB] building merkle tree\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    bob_tree = mt_build(bob_data_blocks,nb_blocks);
    memcpy(bob_root_hash,bob_tree[depth-1][0],HASH_SIZE);
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    print_hash(bob_root_hash,HASH_SIZE,"\n[ALICE] received merkle root hash");
    printf("[ALICE] verifying the integrity of the updated file by building merkle tree\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    alice_tree = mt_build(alice_data_blocks,nb_blocks);
    memcpy(alice_root_hash,alice_tree[depth-1][0],HASH_SIZE);
    printf("[ALICE] comparing root hashes\n");
    printf(ANSI_COLOR_RESET"\ntap to continue\n");
    getc(stdin);
    if(tth_t_compare(bob_root_hash,alice_root_hash) == 0){
        print_hash(bob_root_hash,HASH_SIZE,ANSI_COLOR_YELLOW"Bob root hash");
        print_hash(alice_root_hash,HASH_SIZE,ANSI_COLOR_GREEN"\nAlice root hash");
        printf(ANSI_COLOR_RESET"[ALICE] root hashes are the same , the file update is safe !\n");
    }
    else{
        uint8_t** needed = malloc((depth-1)*sizeof(uint8_t*));  //to free
            for(size_t level = 0 ; level < depth-1;level++){
                needed[level] = malloc(HASH_SIZE*sizeof(uint8_t));
            }
        print_hash(bob_root_hash,HASH_SIZE,ANSI_COLOR_YELLOW"Bob root hash");
        print_hash(alice_root_hash,HASH_SIZE,ANSI_COLOR_RED"\nAlice root hash");
        printf(ANSI_COLOR_RESET"[ALICE] root hashes are different , the new update data are corrupted !\n");
        printf(ANSI_COLOR_RESET"\ntap to continue\n");
        getc(stdin);
        printf("[ALICE] type which data you want to valide [0-%ld] (-1 to stop)\n",nb_blocks-1);
        scanf("%ld",&to_check);
        while(to_check != -1){
            assert((to_check < nb_blocks) || (to_check == -1));
            size_t* data_to_request = mt_get_proof_of_inclusion(nb_blocks,to_check);
            printf("[ALICE] to validate Data #%ld i need the hashes with indexes :\n",to_check);
            for(size_t i = 0; i < depth-1; i++){
                printf("level %ld | index %ld\n",i,data_to_request[i]);
            }
            printf("[ALICE] requested hashes needed from bob\n");
            printf(ANSI_COLOR_RESET"\ntap to continue\n");
            getc(stdin);
            getc(stdin);
            for(size_t level = 0 ; level < depth-1;level++){
                memcpy(needed[level],bob_tree[level][data_to_request[level]],HASH_SIZE);
                printf("[BOB] sent : ");
                print_hash(needed[level],HASH_SIZE,"");
                printf("\n");

            }
            uint8_t is_valid=mt_proof_from_additional_data(to_check,alice_data_blocks[to_check],bob_root_hash,needed,depth-1);
            if(!is_valid){
                memcpy(alice_data_blocks[to_check],bob_data_blocks[to_check],DATA_BLOCK_SIZE);
                printf("[ALICE] updated Data #%ld\n",to_check);
            }
            printf("[ALICE] type which data you want to valide [0-%ld] (-1 to stop)\n",nb_blocks-1);
            scanf("%ld",&to_check);
        }
        
        for(size_t i = 0; i < depth-1;i++)
            free(needed[i]); 
        free(needed);
    }
    for(size_t i = 0;i < nb_blocks;i++){
        free(alice_data_blocks[i]);
        free(bob_data_blocks[i]);
    }
    free(alice_data_blocks);
    free(bob_data_blocks);
    printf("################ DESTROYING MERKLE TREES ####################\n");
    printf("destroy Alice merkle tree:\n");
    printf("%ld\n",nb_blocks);
    mt_destroy(alice_tree,nb_blocks);
    printf("\ndestroy Bob merkle tree:\n");
    mt_destroy(bob_tree,nb_blocks);
    printf(ANSI_COLOR_MAGENTA"\n################## FILE UPDATE TEST FINISHED ################\n");
}

int main(){
#if MODIFIED
    alice_bob_file_transfer();
    alice_bob_file_update();
#else
    proof_collision();
#endif
}