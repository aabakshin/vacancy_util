/* Основной модуль программы */

#include "../includes/DataList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../includes/Additional.h"

/* Описаны в модуле DataList */
extern int l_u_d_flag;
extern char* last_update_date;

/* Опции меню программы */
enum
{
	READ,
	PRINT,
	ADD,
	REMOVE,
	REENUM,
	WRITE,
	QUIT
};

/* Содержит индексы для обращения к fields */
enum
{
	URL,
	CITY_NAME,
	COMPANY_NAME,
	WORK_EXPERIENCE,
	WORK_CONDITIONS,
	SALARY,
	VACANCY_NAME,
	REQUIREMENTS,
	ADDITIONAL_REQUIREMENTS
};

/* название полей данных для форматированного вывода */
static const char* fields[] =
{
	"URL",
	"city name",
	"company name",
	"work experience",
	"work conditions",
	"salary",
	"vacancy name",
	"requirements",
	"additional requirements",
	NULL
};

/* Функция добавления новой записи в список. Используется в меню программы */
static int add_new_record(DataList** list_ptr)
{
	if ( list_ptr == NULL )
	{
		fprintf(stderr, "%s", "\nUnable to delete record\n");
		return 0;
	}

	Data* new_data = NULL;
	new_data = malloc(sizeof(struct Data));
	if ( !new_data )
	{
		fprintf(stderr, "%s", "\nIn function \"add_new_record\" memory error\n");
		return 0;
	}
	int number = dl_get_new_number(*list_ptr);
	char num[10];
	itoa(number, num, 9);
	
	new_data->req_nums = 0;
	new_data->adds_req_nums = 0;

	int m, n;
	for ( m = 0; m < STR_REQUIREMENTS_SIZE; m++ )
	{
		for ( n = 0; n < STR_SIZE; n++ )
		{
			new_data->requirements[m][n] = 0;
			new_data->additional_requirements[m][n] = 0;
		}
	}

	int i;
	for ( i = 0; num[i]; i++ )
		new_data->number[i] = num[i];
	new_data->number[i] = '\0';

	
	char buffer[DATA_BUF_SIZE] = {0};
	n = 0;
	do
	{
		printf("Enter %s:\n", fields[n]);
		printf("%s", "? ");
		fgets(buffer, DATA_BUF_SIZE-1, stdin);
		
		int len = strlen(buffer);
		buffer[len-1] = '\0';
		
		switch ( n )
		{
			case URL:
				for ( i = 0; buffer[i]; i++ )
					new_data->url[i] = buffer[i];
				new_data->url[i] = '\0';
				break;
			case CITY_NAME:
				for ( i = 0; buffer[i]; i++ )
					new_data->city_name[i] = buffer[i];
				new_data->city_name[i] = '\0';
				break;
			case COMPANY_NAME:
				for ( i = 0; buffer[i]; i++ )
					new_data->company_name[i] = buffer[i];
				new_data->company_name[i] = '\0';
				break;
			case WORK_EXPERIENCE:
				for ( i = 0; buffer[i]; i++ )
					new_data->work_experience[i] = buffer[i];
				new_data->work_experience[i] = '\0';
				break;
			case WORK_CONDITIONS:
				for ( i = 0; buffer[i]; i++ )
					new_data->work_conditions[i] = buffer[i];
				new_data->work_conditions[i] = '\0';
				break;
			case SALARY:
				for ( i = 0; buffer[i]; i++ )
					new_data->salary[i] = buffer[i];
				new_data->salary[i] = '\0';
				break;
			case VACANCY_NAME:
				for ( i = 0; buffer[i]; i++ )
					new_data->vacancy_name[i] = buffer[i];
				new_data->vacancy_name[i] = '\0';
				break;
			case REQUIREMENTS:	/* разделитель - вертикальная черта */
				;char* tokens[100];
				char* istr = strtok(buffer, "|");
				int j = 0;
				while ( istr != NULL )
				{
					tokens[j] = istr;
					j++;
					istr = strtok(NULL, "|");
				}
				int tokens_num = j;
				
				int m, n;
				for ( m = 0; m < tokens_num; m++ )
				{
					for ( n = 0; tokens[m][n]; n++ )
						new_data->requirements[m][n] = tokens[m][n];
					new_data->requirements[m][n] = '\0';
					new_data->req_nums++;
				}
				break;
			case ADDITIONAL_REQUIREMENTS: /* разделитель - вертикальная черта */
				istr = strtok(buffer, "|");
				j = 0;
				while ( istr != NULL )
				{
					tokens[j] = istr;
					j++;
					istr = strtok(NULL, "|");
				}
				tokens_num = j;
				
				for ( m = 0; m < tokens_num; m++ )
				{
					for ( n = 0; tokens[m][n]; n++ )
						new_data->additional_requirements[m][n] = tokens[m][n];
					new_data->additional_requirements[m][n] = '\0';
					new_data->adds_req_nums++;
				}
		}

		n++;
	}
	while ( n <= ADDITIONAL_REQUIREMENTS );


	if ( dl_insert(list_ptr, new_data) )
	{
		printf("Record #%s has successfully added\n", new_data->number);
		return 1;
	}

	return 0;
}

/* Функция удаления записи из списка. Используется в меню программы */
static int delete_old_record(DataList** list_ptr)
{
	if ( (list_ptr == NULL) || (*list_ptr == NULL) )
	{
		fprintf(stderr, "%s", "\nUnable to delete record\n");
		if ( *list_ptr == NULL )
			fprintf(stderr, "%s", "List is empty\n");

		return 0;
	}
	
	printf("%s", "Enter record number:\n");
	char buffer[10];
	printf("%s", "? ");
	fgets(buffer, 9, stdin);
	int len = strlen(buffer);
	buffer[len-1] = '\0';

	int record_num = atoi(buffer);
	
	if ( record_num < 1 )
	{
		fprintf(stderr, "%s", "Incorrect record number!\n");
		return 0;
	}

	DataList* del_val = dl_find(list_ptr, buffer);
	if ( !del_val )
	{
		fprintf(stderr, "Unable to find \"%s\" record\n", buffer);
		return 0;
	}

	Data* del = dl_delete(list_ptr, del_val->data);
	if ( del == NULL )
	{
		fprintf(stderr, "Unable to delete \"%s\" record\n", buffer);
		return 0;
	}

	free(del);
	printf("Record #%s has successfully deleted\n", buffer);
	return 1;
}


int main(int argc, char** argv)
{
	if ( argc != 3 )
	{
		fprintf(stderr, "%s", "Incorrect arguments amount!\n"
						      "Usage: ./<program_name> <input_file> <output_file>\n");
		return 1;
	}

	DataList* vacancies_list = NULL;

	/*dl_print(vacancies_list);*/

	/*
	time_t cur_time = 0;
	time(&cur_time);

	if ( l_u_d_flag )
	{
		free(last_update_date);
		l_u_d_flag = 0;
	}
	last_update_date = ctime(&cur_time);
	*/

	printf("%s", "Program options:\n"
				 "(0) - read vacancies list from input file\n"
				 "(1) - print current vacancies list\n"
				 "(2) - add new record\n"
				 "(3) - remove old record\n"
				 "(4) - reenum records\n"
				 "(5) - write current list in output file\n"
				 "(6) - quit\n");

	char choice_buf[10] = {0};
	int choice = -1;
	int success_flag = 0;

	do
	{
		printf("\n%s", "? ");
		fgets(choice_buf, 9, stdin);
		choice = atoi(choice_buf);
		
		switch ( choice )
		{
			case READ:
				dl_read(&vacancies_list,argv[1]);
				break;
			case PRINT:
				dl_print(vacancies_list);
				break;
			case ADD:
				if ( add_new_record(&vacancies_list) )
				{
					time_t cur_time = 0;
					time(&cur_time);

					if ( l_u_d_flag )
					{
						free(last_update_date);
						l_u_d_flag = 0;
					}
					last_update_date = ctime(&cur_time);
				}
				break;
			case REMOVE:
				if ( delete_old_record(&vacancies_list) )
				{
					time_t cur_time = 0;
					time(&cur_time);

					if ( l_u_d_flag )
					{
						free(last_update_date);
						l_u_d_flag = 0;
					}
					last_update_date = ctime(&cur_time);
				}
				break;
			case REENUM:
				if ( dl_reenum(vacancies_list) )
				{
					printf("%s", "Current list has been reenumed\n");
					time_t cur_time = 0;
					time(&cur_time);

					if ( l_u_d_flag )
					{
						free(last_update_date);
						l_u_d_flag = 0;
					}
					last_update_date = ctime(&cur_time);
				}
				break;
			case WRITE:
				if ( dl_write(vacancies_list, argv[2]) )
					printf("New data has wrote in file \"%s\"\n", argv[2]);
				success_flag = 1;
				break;
			case QUIT:
				success_flag = 1;
				break;
			default:
				fprintf(stderr, "%s", "Incorrect choice!\n");
		}

		if ( success_flag )
			break;
	} 
	while ( 1 );
	
	dl_clear(&vacancies_list, 1);
	
	if ( l_u_d_flag )
		free(last_update_date);

	return 0;
}
