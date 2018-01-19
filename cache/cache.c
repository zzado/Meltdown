#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>
#include <string.h>

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

void* initmem(uint32_t size){

	void* mem = (void*)malloc(size * 3);
	mem = (void *)((uint64_t)mem + 0x1000 >> 12 << 12);	
	memset(mem, 0, size * 2);

	return mem;	
}

void main(int argc, char* argv[]) {

	void* mem = initmem(4096);
	// Alloc & Init memory
	
	uint64_t time;
	for(uint32_t i=0; i<16; i++){ 
		time = get_time(mem);
	}
	// Load memory to L1 cache
	printf("Cached %ld\n", time);
	
	_mm_clflush(mem);
    _mm_mfence();
	// Flush L1 cahce
    time = get_time(mem);
	printf("Not cached %ld\n", time);
}


