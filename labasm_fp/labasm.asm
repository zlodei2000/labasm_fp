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

        mov     ebp, esp        ; Move ESP -> EBP чтоб на аргументы в стеке ссылаться
                                ;   

        mov	eax, DWORD PTR _str$[ebp]   ;первый аргумент
	    push	eax
	    call	printf
	    add	esp, 4

        mov     eax, DWORD PTR _substr$[ebp]        ;второй аргумент
	    push	eax
	    call	printf
	    add	esp, 4


        mov     esi, DWORD PTR _substr$[ebp]        ; подстрока
        mov     edi, DWORD PTR _str$[ebp]           ; строка
        
        mov eax,-1   ; здесь будет результат
        xor ecx,ecx   ; кол-во просмотренных символов в подстроке 
        xor ebx,ebx   ; здесь будем сравнивать текущий символ

        mov edx,edi   ;EDX сохранит адрес подстроки

        cmp BYTE PTR [esi],0   ;Если конец строки
        je $exitproc           ;выход
        cmp BYTE PTR [edi],0   ;Если конец строки
        je $exitproc           ;выход

                               ;Иначе будем искать в цикле. ESI - большая строка. EDI - подстрока
$for:
        mov bl,BYTE PTR [esi]  ;в BL - текущий символ строки
        cmp bl,[edi]           ;сравним его с символом в подстроке
        jne $no                ;если символы не равны, значит надо двигать на следующий в большой строке и заканчиваем сравнение блока
        inc ecx                ;Иначе увеличим счетчик длины искомого
        inc edi                ;Перескочим на следующий символ в подстроке
        cmp BYTE PTR [edi],0   ;И узнаем не конец ли подстроки
        je $exitproc           ;Если конец - строка найдена. Выходим.
        jmp $nextiter          ;Иначе пойдем на следующую итерацию цикла поиска
$no:
        sub esi,ecx            ;Поскольку используется один цикл для прохода
                               ;После поиска подстроки нам приходится возвращаться на тот символ, с которого мы начали.
                               ;Достигается это минусованием прошедших символов в искомом и текущей позиции в источнике
        xor ecx,ecx            ;Потом не забудим сбросить в ноль кол-во совпавших символов, раз они не найдены
        mov edi,edx            ;И в подстроке двигаем в начало для поиска с следующего символа
$nextiter:
        inc eax
        inc esi                ;сдвигаемся в большой строке на следующий символ
        jmp $for               ;на начало цикла

$exitproc:
        sub eax,ecx             ; Поскольку у нас ЕАХ будет указывать на позицию последнего символа искомого,
                                ; Придется отнять от него длину искомого, чтоб получить позицию начинающуюся с нуля
        pop     edi             ; restore regs
        pop     esi  
        pop     ebx
        pop     ebp             ; restore ebp
        ret                     ; результат в eax
_findsubstring ENDP
_TEXT   ENDS
END
