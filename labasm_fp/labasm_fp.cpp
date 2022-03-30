#include <iostream> 

using namespace std;
//char prnformat[] = "%s\n";
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
                je $endproc                     ; выход
                cmp BYTE PTR[edi], 0            ; 
                je $endproc                     ; выход

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

int main()
{
	string str, substr;
	//do {
	//	cout << "enter string (not empty):";
	//	getline(cin, str); 
	//	cout << "enter substring (should be shorter than str):";
	//	getline(cin, substr); 
	//} while ((str.length() == 0) || (substr.length() > str.length()) || substr.length() == 0);
	//
	str = "hello world";
	substr = "wo";
	cout << "substring position (c version):" << findsubstring2((char*)str.c_str(), (char*)substr.c_str()) << endl;

	cout << "substring position (asm version):" << findsubstring((char*)str.c_str(), (char*)substr.c_str()) <<endl;
}
