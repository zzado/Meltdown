#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>
#include <string.h>
#include <unistd.h>
#define PAGE_SIZE 4096
#define ARR_SIZE 256

uint8_t leak_cache(void* target_addr, void* L1);
void meltdown(void* addr, uint32_t* size);
uint64_t get_time(void *addr);
void delay_commit(void);
void trigger_speculative_execution(void* target_addr, void* L1);

