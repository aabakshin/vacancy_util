/* Файл реализации модуля Additional */

#ifndef ADDITIONAL_C
#define ADDITIONAL_C

#include "../includes/Additional.h"
#include <string.h>


/* Вспомогательная функция для itoa. Производит реверс строки. */
static void reverse(char* s)
{
	int i = 0;
	int j = strlen(s)-1;

	for ( ; i < j; i++, j-- )
	{
		char c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* Вспомогательная функция для itoa. Производит подсчёт цифр целого числа */
static int num_digit_cnt(int number)
{
	int counter = 0;
	
	if ( number == 0 )
		return 1;

	if ( number < 0 )
		number *= -1;

	while ( number > 0 )
	{
		counter++;
		number /= 10;
	}

	return counter;
}
void itoa(int number, char* num_buf, int max_buf_len)
{
	if ( number == 0 )
	{
		num_buf[0] = '0';
		num_buf[1] = '\0';
		return;
	}

	int cnt = num_digit_cnt(number);

	if ( cnt > (max_buf_len-1) )
		cnt = max_buf_len-1;
	
	int flag = 0;
	if ( number < 0 )
	{
		number *= -1;
		flag = 1;
	}
	
	int i = 0;
	while ( number > 0 && (i < cnt) )
	{
		num_buf[i] = (number % 10) + '0';
		number /= 10;
		i++;
	}

	if ( flag )
	{
		num_buf[i] = '-';
		i++;
	}
	num_buf[i] = '\0';

	reverse(num_buf);
}

#endif
