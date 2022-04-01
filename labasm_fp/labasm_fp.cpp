#include <iostream> 

using namespace std;

int findsubstring2(char* str, char* substr) {
	if (str[0] == '\0' || substr[0] == '\0')
		return -1;

	int res = -1;

	for (int i = 0; str[i] != '\0'; i++)
	{
		int j;
		for (j=0; (str[i+j] == substr[j]) && str[i+j] != '\0' && substr[j] != '\0';j++)
		{
		}
		if (substr[j] == '\0') {
			res = i;
			break;
		}
	}
	return res;
}

int findsubstring3(char strbig[], char substr[]) {
    // этот код реализует ровно тот алгоритм, что на С выше
    __asm {
            mov esi, DWORD PTR[strbig]  ; строка
            mov edi, DWORD PTR[substr]  ; подстрока

            mov eax, -1                 ; здесь будет результат, сразу делаем - 1 - то есть в начале = не найдено
            xor ebx, ebx                ; здесь будем сравнивать текущий символ, обнулим

            xor ecx, ecx                ; здесь будет номер текущего просматриваемого символа в строке 

            mov edx, edi                ; EDX сохранит адрес подстроки, чтоб можно было быстро переходить на начало подстроки

            cmp BYTE PTR[esi], 0        ; Если на вход пустая строка или  подстрока
            je $ex                      ; выход
            cmp BYTE PTR[edi], 0
            je $ex                      ; выход


                                        ; будем искать в цикле.ESI - большая строка.EDI - подстрока
$for :                                  ; начало внешнего цикла
            mov bl, BYTE PTR[esi]       ; в BL - текущий символ строки
            cmp bl, 0                   ; если 0 = строка кончилась, выход из цикла
            je $ex

            mov edi, edx                ;переходим на начало подстроки
            push esi                    ;сохраним в стек значение esi - позиция в основной строке
$intfor:                                ;начало внутреннего цикла
            mov bl, BYTE PTR[esi]
            cmp bl, BYTE PTR [edi]      ; сравним его с символом в подстроке
            jne $endintfor;

            cmp bl, 0                   ;проверем текущий символ строки на 0, т.е. на конец
            je $endintfor
            
            mov bl, BYTE PTR [edi]      ;проверем текущий символ подстроки на 0, т.е.на конец
            cmp bl, 0
            je $endintfor

            inc esi
            inc edi
            jmp $intfor

$endintfor:                             ;конец внутреннего цикла
            pop esi                     ;восстановим из стека - чтобы вернуться в текущий символ в основной строке. во внутреннем цикле мы меняли этот указатель
            mov bl, BYTE PTR [edi]
            cmp bl, 0
            jne $next
            mov eax, ecx
            jmp $ex                     ; если дошли во внутреннем цикле до конца подстроки - значит нашли ее. выходим, результат кладем в eax

$next :
            inc ecx                     ; номер просматриваемого символа основной строки + 1
            inc esi                     ; сдвигаемся в большой строке на следующий символ
            jmp $for                    ; на начало цикла
$ex :
    }
}
int findsubstring4(char strbig[], int strlen, char substr[], int substrlen) {
    __asm {
        //mov	eax, strbig; первый аргумент
        //push	eax
        //mov     eax, offset prnformat
        //push    eax
        //call	printf
        //add	esp, 8

        //mov	eax, substr; второй аргумент
        //push	eax
        //mov     eax, offset prnformat
        //push    eax
        //call	printf
        //add	esp, 8

            mov eax, -1; здесь будет результат, сразу делаем - 1 - то есть в начале = не найдено
            cmp strlen, 0
            je $ex

            cmp substrlen, 0
            je $ex

            mov edx, strlen
            sub edx, substrlen      ;если длина подстроки больше чем строка, то как бы надо выходить
            cmp edx,0
            jl $ex

            inc edx
            mov     esi, DWORD PTR [strbig]     ; pointer to string in ESI

            ; enter the compare loop
$repeat:
            mov     edi, DWORD PTR[substr]  ; pointer to substring in EDI
            mov     ecx, substrlen          ; length substring in ECX(loop counter)
            cld
            mov     eax,esi
            repe    cmpsb               ; compare string at edi with length ecx with string in esi
            jz      $found              ; if zero flag then substring is found within string, exit loop

            ; substring is not found yet, put substring pointer at begin of substring

            inc eax
            mov esi,eax
            dec edx
            cmp edx, 0            ; check remaining length to search in. это просто проверка на ноль edx
            jnz $repeat             ; remaining length non - zero, repeat

$notfound:
            ; else, substring wasn't found, exit loop
            ; substring not found actions
            mov eax,-1
            jmp $ex
$found:
            ; substring found actions
            ; si has address to start of substring + the length of the substring
            ; subtracting the start of the string we can calculate the offset(or index) in the string where substring starts

            sub         esi, substrlen
            sub         esi, strbig
            mov         eax, esi
$ex :
    }
}

int main()
{
	string str, substr;
	//do {
	//	cout << "enter string (not empty):";
	//	getline(cin, str); 
	//	cout << "enter substring (should be shorter than str):";
	//	getline(cin, substr); 
	//} while ((str.length() == 0) || (substr.length() > str.length()) || substr.length() == 0);
	// чтоб проще отлаживать было убрал интерактивный ввод
	str = "hello world";
	substr = "wor";
	cout << "substring position (c version):" << findsubstring2((char*)str.c_str(), (char*)substr.c_str()) << endl;
    cout << "substring position (asm version 2):" << findsubstring3((char*)str.c_str(), (char*)substr.c_str()) << endl;
    cout << "substring position (asm version 3):" << findsubstring4((char*)str.c_str(), str.length(), (char*)substr.c_str(), substr.length()) << endl;
}
