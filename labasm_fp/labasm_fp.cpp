#include <iostream> 

using namespace std;

int findsubstring(char strbig[], char substr[]) {
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

                mov     esi, DWORD PTR[strbig]; строка
                mov     edi, DWORD PTR[substr]; подстрока

                mov eax, -1                     ; здесь будет результат, сразу делаем -1 - типа не найдено
                xor ecx, ecx                    ; сколько совпавших символов подстроки, в начале 0
                xor ebx, ebx                    ; здесь будем сравнивать текущий символ, обнулим

                mov edx, edi                    ; EDX сохранит адрес подстроки, чтоб можно было быстро переходит на начало подстроки

                cmp BYTE PTR[esi], 0            ; Если на вход пустая строка или  подстрока
                je $ex                          ; выход
                cmp BYTE PTR[edi], 0            ; 
                je $ex                          ; выход

                xor eax, eax                    ; номер текущего символа большой строки   = 0

                ; Иначе будем искать в цикле.ESI - большая строка.EDI - подстрока
$for :
                mov bl, BYTE PTR[esi]       ; в BL - текущий символ строки
                cmp bl, 0                   ; если 0 = строка кончилась, значит не нашли подстроку
                jne $cnt
                mov eax,-1                  ; возвращаем -1
                jmp $ex
$cnt:           cmp bl, [edi]               ; сравним его с символом в подстроке
                jne $no                     ; если символы не равны, значит надо двигать на следующий в большой строке и заканчиваем сравнение блока
                inc ecx                     ; Иначе увеличим счетчик длины искомого
                inc edi                     ; Перескочим на следующий символ в подстроке
                cmp BYTE PTR[edi], 0        ; проверим узнаем не конец ли подстроки
                je $endproc                 ; Если конец - строка найдена.Выходим.
                jmp $next                   ; Иначе пойдем на следующую итерацию цикла поиска
$no :
                sub esi, ecx                ; Поскольку используется один цикл для прохода
                                            ; После поиска подстроки нам приходится возвращаться на тот символ, с которого мы начали.
                                            ; 
                xor ecx, ecx                ; делаем что совпавших символов = 0
                mov edi, edx                ; И в подстроке снова на начало следующего символа
$next :
                inc eax                     ; номер просматриваемого +1
                inc esi                     ; сдвигаемся в большой строке на следующий символ

                jmp $for                    ; на начало цикла
$endproc :
                sub eax, ecx                ; Поскольку у нас ЕАХ будет указывать на позицию последнего символа искомого,
                inc eax                     ; и корректировка
$ex:
    }
}



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
	substr = "wo";
	cout << "substring position (c version):" << findsubstring2((char*)str.c_str(), (char*)substr.c_str()) << endl;

	cout << "substring position (asm version):" << findsubstring((char*)str.c_str(), (char*)substr.c_str()) <<endl;

    cout << "substring position (asm version 2):" << findsubstring3((char*)str.c_str(), (char*)substr.c_str()) << endl;
}
