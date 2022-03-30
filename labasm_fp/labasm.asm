; labasm.asm 
; x86 code for C interop
; Command line: ml /c /Cx /W3 /WX labasm.asm 
        .686P
        .XMM
        .MODEL  flat
printf PROTO C
includelib msvcrt.lib
includelib legacy_stdio_definitions.lib

PUBLIC  _findsubstring

_TEXT   SEGMENT
_str$ = 8							; 
_substr$ = 12
_findsubstring PROC
        push    ebp             ; save regs
        push    ebx  
        push    esi  
        push    edi 

        mov     ebp, esp        ; Move ESP -> EBP ���� �� ��������� � ����� ���������
                                ;   

        mov	eax, DWORD PTR _str$[ebp]   ;������ ��������
	    push	eax
	    call	printf
	    add	esp, 4

        mov     eax, DWORD PTR _substr$[ebp]        ;������ ��������
	    push	eax
	    call	printf
	    add	esp, 4


        mov     esi, DWORD PTR _substr$[ebp]        ; ���������
        mov     edi, DWORD PTR _str$[ebp]           ; ������
        
        mov eax,-1   ; ����� ����� ���������
        xor ecx,ecx   ; ���-�� ������������� �������� � ��������� 
        xor ebx,ebx   ; ����� ����� ���������� ������� ������

        mov edx,edi   ;EDX �������� ����� ���������

        cmp BYTE PTR [esi],0   ;���� ����� ������
        je $exitproc           ;�����
        cmp BYTE PTR [edi],0   ;���� ����� ������
        je $exitproc           ;�����

                               ;����� ����� ������ � �����. ESI - ������� ������. EDI - ���������
$for:
        mov bl,BYTE PTR [esi]  ;� BL - ������� ������ ������
        cmp bl,[edi]           ;������� ��� � �������� � ���������
        jne $no                ;���� ������� �� �����, ������ ���� ������� �� ��������� � ������� ������ � ����������� ��������� �����
        inc ecx                ;����� �������� ������� ����� ��������
        inc edi                ;���������� �� ��������� ������ � ���������
        cmp BYTE PTR [edi],0   ;� ������ �� ����� �� ���������
        je $exitproc           ;���� ����� - ������ �������. �������.
        jmp $nextiter          ;����� ������ �� ��������� �������� ����� ������
$no:
        sub esi,ecx            ;��������� ������������ ���� ���� ��� �������
                               ;����� ������ ��������� ��� ���������� ������������ �� ��� ������, � �������� �� ������.
                               ;����������� ��� ������������ ��������� �������� � ������� � ������� ������� � ���������
        xor ecx,ecx            ;����� �� ������� �������� � ���� ���-�� ��������� ��������, ��� ��� �� �������
        mov edi,edx            ;� � ��������� ������� � ������ ��� ������ � ���������� �������
$nextiter:
        inc eax
        inc esi                ;���������� � ������� ������ �� ��������� ������
        jmp $for               ;�� ������ �����

$exitproc:
        sub eax,ecx             ; ��������� � ��� ��� ����� ��������� �� ������� ���������� ������� ��������,
                                ; �������� ������ �� ���� ����� ��������, ���� �������� ������� ������������ � ����
        pop     edi             ; restore regs
        pop     esi  
        pop     ebx
        pop     ebp             ; restore ebp
        ret                     ; ��������� � eax
_findsubstring ENDP
_TEXT   ENDS
END
