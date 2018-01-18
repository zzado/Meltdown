.intel_syntax noprefix

.global speculate
	speculate:
		mfence
		call herring
  # Speculatively executed (because of herring's ret being mispredicted):
		movzx eax, byte ptr [rdi]
		shl eax, 12
		movzx eax, byte ptr [rsi+rax]

.global herring
	herring:
  # Lots of slow dependent instructions:
		xorps xmm0, xmm0
		sqrtpd xmm0, xmm0
		sqrtpd xmm0, xmm0
		sqrtpd xmm0, xmm0
		sqrtpd xmm0, xmm0
  # Using result of dependent instructions, adjust rsp to trick prediction of ret
		movd eax, xmm0
		lea rsp, [rsp+rax+8]
		ret # Actually returns from speculate, but predicted as returning from herring 
