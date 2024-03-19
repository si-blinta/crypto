#include "tth.h"

int main(){
	srand(time(NULL));
	uint64_t nb_iterations =  floyd_collision();
	printf("needed %ld iterations\n",nb_iterations);
	return 0;

}
