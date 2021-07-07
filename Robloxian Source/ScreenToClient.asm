.code

_ScreenToClient PROC

	mov r10, rcx
	mov eax, 3735928559
	syscall
	ret

_ScreenToClient ENDP

END