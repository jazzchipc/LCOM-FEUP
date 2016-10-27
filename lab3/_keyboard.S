#include "kbc.h"

.global _read_key_asm
.global _key_asm

.data

_key_asm:
	.byte 0

.text

_read_key_asm:

xor %eax, %eax
inb $OUT_BUF, %al
movb %al, _key_asm

ret
