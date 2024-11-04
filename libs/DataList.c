/* Файл реализации модуля DataList */

#ifndef DATALIST_C
#define DATALIST_C

#include "../includes/DataList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Additional.h"

/* хранит обновленную дату модификации файла */
char* last_update_date = NULL;

/* флаг устанавливается, если произошла модификация содержимого файла во время его обработки программой */
int l_u_d_flag = 0;


int dl_get_new_number(DataList* list)
{
	if ( list == NULL )
	{
		return 1;
	}

	while ( list->next != NULL )
	{
		list = list->next;
	}
	
	return ++list->number; 
}

int dl_size(DataList* list)
{
	if ( (list == NULL) || (list->data == NULL) )
	{
		return 0;
	}

	int size = 0;

	while ( (list != NULL) && (list->data != NULL) )
	{
		size++;
		list = list->next;
	}

	return size;
}

int dl_insert(DataList** list_ptr, Data* dat)
{
	if ( (list_ptr == NULL) || (dat == NULL) )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_insert \"list_ptr\" OR \"dat\" is NULL\n");
		return 0;
	}

	DataList* newptr = NULL;
	newptr = malloc(sizeof(struct Data));
	if ( !newptr )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_insert memory error\n");
		return 0;
	}
	newptr->number = dl_get_new_number(*list_ptr);
	newptr->data = dat;
	newptr->next = NULL;
	newptr->prev = NULL;


	if ( *list_ptr == NULL )
	{
		*list_ptr = newptr;
		return 0;
	}

	DataList* prevptr = NULL;
	DataList* curptr = *list_ptr;

	while ( curptr != NULL )
	{
		prevptr = curptr;
		curptr = curptr->next;
	}

	prevptr->next = newptr;
	newptr->prev = prevptr;
	
	return 1;
}

Data* dl_delete(DataList** list_ptr, Data* dat)
{
	if ( (list_ptr == NULL) || (*list_ptr == NULL) || ( (*list_ptr)->data == NULL ) || (dat == NULL) )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_delete \"list_ptr\" OR \"*list_ptr\" OR \"dat\" is NULL\n");
		return NULL;
	}
	
	DataList* tempptr = NULL;
	DataList* prevptr = NULL;
	DataList* curptr = *list_ptr;
	
	while ( (curptr != NULL) && (curptr->data != dat) )
	{
		prevptr = curptr;
		curptr = curptr->next;
	}

	if ( curptr == NULL )
	{
		fprintf(stderr, "\n[DataList]: In function dl_delete element with \"%s\" number not found!\n", dat->number);
		return NULL;
	}
	
	Data* ret_val = curptr->data;
	tempptr = curptr;
	if ( curptr == *list_ptr )
	{
		*list_ptr = (*list_ptr)->next;
		if ( *list_ptr != NULL )
			(*list_ptr)->prev = NULL;

		free(tempptr);
		return ret_val;
	}
	
	if ( curptr->next == NULL )
	{
		prevptr->next = NULL;
		
		free(tempptr);
		return ret_val;
	}
	
	prevptr->next = curptr->next;
	curptr->next->prev = prevptr;
	
	free(tempptr);
	return ret_val;
}

int dl_clear(DataList** list_ptr, int clear_data_flag)
{
	if ( (list_ptr == NULL) || (*list_ptr == NULL) || ((*list_ptr)->data == NULL) )
	{
		if ( list_ptr == NULL )
			fprintf(stderr, "%s", "\n[DataList]: In function dl_clear() \"list_ptr\" is NULL\n");
		return 0;
	}

	int size = dl_size(*list_ptr);
	int del_values = 0;

	int i;
	for ( i = 1; i <= size; i++ )
	{
		Data* del_value = dl_delete(list_ptr, (*list_ptr)->data);
		if ( clear_data_flag )
			if ( del_value )
				free(del_value);

		if ( del_value )
			del_values++;
	}

	if ( del_values == size )
	{
		return 1;
	}

	return 0;
}

void dl_print(DataList* list)
{
	if ( (list == NULL) || (list->data == NULL) )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_print() \"list\" OR \"list->data\" is NULL\n");
		return;
	}
	
	printf("\nLast update: %s\n", last_update_date);
	while ( (list != NULL) && (list->data != NULL) )
	{
		printf("%s", "\n/--------------------------------------------------------------------------------\\\n");
		printf("\t\t\t\t\t[%s]:\n", list->data->number);
		printf("%-20s %s\n", "Vacancy URL:", list->data->url);
		printf("%-20s %s\n", "City:", list->data->city_name);
		printf("%-20s %s\n", "Company:", list->data->company_name);
		printf("%-20s %s\n", "Work experience:", list->data->work_experience);
		printf("%-20s %s\n", "Work conditions:", list->data->work_conditions);
		printf("%-20s %s\n", "Salary:", list->data->salary);
		printf("%-20s %s\n", "Vacancy name:", list->data->vacancy_name);
		
		printf("%s", "Requirements:\n");
		int i;
		for ( i = 0; i < list->data->req_nums; i++ )
		{
			printf("\t- %s\n", list->data->requirements[i]);
		}
		printf("%s", "Additional Requirements:\n");
		for ( i = 0; i < list->data->adds_req_nums; i++ )
		{
			printf("\t- %s\n", list->data->additional_requirements[i]);
		}
		printf("%s", "\\--------------------------------------------------------------------------------/\n\n");
		list = list->next;
	}
}

int dl_reenum(DataList* list)
{
	if ( (list == NULL) || (list->data == NULL) )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_reenum() \"list\" OR \"list->data\" is NULL\n");
		return 0;
	}

	int num = 1;
	while ( (list != NULL) && (list->data != NULL) )
	{
		list->number = num;
		char num_buf[10];
		itoa(num, num_buf, 9);
		
		int i;
		for ( i = 0; list->data->number[i]; i++ )
			list->data->number[i] = num_buf[i];
		list->data->number[i] = '\0';

		num++;
		list = list->next;
	}

	return 1;
}

DataList* dl_find(DataList** list_ptr, const char* num)
{
	if (
		 (list_ptr == NULL) || 
		 (*list_ptr == NULL) || 
		 ( (*list_ptr)->data == NULL ) || 
		 (num == NULL) || 
		 (num[0] == '\n') || 
		 (num[0] == '\r') || 
		 (num[0] == '\0') 
	   )
	{
		return NULL;
	}

	DataList* list = *list_ptr;
	while ( (list != NULL) && (list->data != NULL) )
	{
		if ( strcmp(list->data->number, num) == 0 )
			return list;

		list = list->next;
	}
	
	return NULL;
}

int dl_read(DataList** list_ptr, const char* input_filename)
{
	if ( (list_ptr == NULL) || (*list_ptr != NULL) || (input_filename == NULL) || (input_filename[0] == '\0') )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_read incorrect filename!\n");
		return 0;
	}

	FILE* fd;
	if ( (fd = fopen(input_filename, "r")) == NULL )
	{
		fprintf(stderr, "\n[DataList]: In function dl_read unable to open file \"%s\"\n", input_filename);
		return 0;
	}
	

	char buffer[DATA_BUF_SIZE*2] = { 0 };
	int ch;
	int n_count = 0;
	while ( (ch = fgetc(fd)) != EOF )
	{
		if ( ch == '\n' )
			n_count++;
	}
	fseek(fd, 0, SEEK_SET);
	
	int i = 0;
	do
	{
		ch = fgetc(fd);
		buffer[i] = ch;
		i++;
	}
	while ( ch != '\n' );
	
	int len = -1;
	if ( i > 0 ) 
	{
		if ( buffer[i-1] == '\r' ) 
		{
			buffer[i-1] = '\0';
			len = i-1;
		}
		else
		{
			buffer[i] = '\0';
			len = i;
		}
	}
	else
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_read input error\n");
		return 0;
	}
	
	char* data_buf = malloc(len + 1);
	if ( !data_buf )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_read memory error(data_buf)\n");
		return 0;
	}
	for ( i = 0; buffer[i]; i++ )
		data_buf[i] = buffer[i];
	data_buf[i] = '\0';
	
	last_update_date = data_buf;
	l_u_d_flag = 1;

	int j, k;
	for ( i = 2; i <= n_count; i++ )
	{
		for ( j = 0; j < DATA_BUF_SIZE*2; j++ )
			buffer[j] = 0;

		Data* data = malloc(sizeof(struct Data));
		if ( !data )
		{
			fprintf(stderr, "%s", "\n[DataList]: In function dl_read memory error(data)\n");
			return 0;
		}
		
		int m, n;
		for ( m = 0; m < STR_REQUIREMENTS_SIZE; m++ )
		{
			for ( n = 0; n < STR_SIZE; n++ )
			{
				data->requirements[m][n] = 0;
				data->additional_requirements[m][n] = 0;
			}
		}
		data->req_nums = 0;
		data->adds_req_nums = 0;


		fgets(buffer, DATA_BUF_SIZE*2-1, fd);
		
		/*printf("\n[%d]: buffer = %s\n", i, buffer);*/
		
		/*
		putchar('\n');
		for ( j	= 0; buffer[j]; j++ )
		{
			printf("%4d ", buffer[j]);
			if ( ((j+1) % 20) == 0 )
				putchar('\n');
		}
		putchar('\n');
		*/

		for ( j = 0; buffer[j] != '\t'; j++ )
			data->number[j] = buffer[j];
		data->number[j] = '\0';
		j++;
		
		for ( k = 0; buffer[j] != ' '; j++, k++ )
			data->url[k] = buffer[j];
		data->url[k] = '\0';
		j++;

		for ( k = 0; buffer[j] != ';'; j++, k++ )
			data->city_name[k] = buffer[j];
		data->city_name[k] = '\0';
		j++;

		for ( k = 0; buffer[j] != ';'; j++, k++ )
			data->company_name[k] = buffer[j];
		data->company_name[k] = '\0';
		j++;

		for ( k = 0; buffer[j] != ';'; j++, k++ )
			data->work_experience[k] = buffer[j];
		data->work_experience[k] = '\0';
		j++;

		for ( k = 0; buffer[j] != ';'; j++, k++ )
			data->work_conditions[k] = buffer[j];
		data->work_conditions[k] = '\0';
		j++;

		for ( k = 0; buffer[j] != ';'; j++, k++ )
			data->salary[k] = buffer[j];
		data->salary[k] = '\0';
		j++;

		for ( k = 0; buffer[j] != ';'; j++, k++ )
			data->vacancy_name[k] = buffer[j];
		data->vacancy_name[k] = '\0';
		j++;
		
		int l = 0;
		for ( ; buffer[j] != '|'; j++ )
		{
			for ( k = 0; (buffer[j] != ',') && (buffer[j] != '|'); j++, k++ )
			{
				data->requirements[l][k] = buffer[j];
			}
			data->requirements[l][k] = '\0';
			data->req_nums++;

			if ( buffer[j] == '|' )
				break;
	
			l++;
		}
		j++;
		
		l = 0;
		for ( ; (buffer[j] != '\r') && (buffer[j] != '\n'); j++ )
		{
			for ( k = 0; (buffer[j] != ',') && (buffer[j] != '\r') && (buffer[j] != '\n'); j++, k++ )
			{
				data->additional_requirements[l][k] = buffer[j];
			}
			data->additional_requirements[l][k] = '\0';
			data->adds_req_nums++;

			if ( (buffer[j] == '\r') || (buffer[j] == '\n') )
				break;

			l++;
		}
		
		dl_insert(list_ptr, data);
	}
	
	fclose(fd);

	return dl_size(*list_ptr);
}

int dl_write(DataList* list, const char* output_filename)
{
	if ( (list == NULL) || (list->data == NULL) || (output_filename == NULL) )
	{
		fprintf(stderr, "%s", "\n[DataList]: In function dl_output - \"list\" OR \"output_filename\" is NULL\n");
		return 0;
	}
	
	FILE* fd;
	if ( (fd = fopen(output_filename, "w")) == NULL )
	{
		fprintf(stderr, "\n[DataList]: In function dl_output - unable to open file \"%s\"\n", output_filename);
		return 0;
	}

	const char* last_mod_template = "Последние изменения: ";
	char data_buf[DATA_BUF_SIZE];

	int i, j;
	for ( i = 0; last_mod_template[i]; i++ )
		data_buf[i] = last_mod_template[i];

	for ( j = 0; (last_update_date != NULL) && last_update_date[j]; j++, i++ )
	{
		if ( last_update_date[j] == '\n' )
			break;

		data_buf[i] = last_update_date[j];
	}

	data_buf[i] = '\r';
	data_buf[i+1] = '\n';
	data_buf[i+2] = '\0';

	fprintf(fd, "%s", data_buf);
	
	while ( (list != NULL) && (list->data != NULL) )
	{
		fprintf(fd, "%s\t%s %s;%s;%s;%s;%s;%s;", list->data->number, list->data->url, list->data->city_name, list->data->company_name,
				list->data->work_experience, list->data->work_conditions, list->data->salary, list->data->vacancy_name);
		
		int i;
		for ( i = 0; i < list->data->req_nums; i++ )
		{
			if ( (i+1) == list->data->req_nums )
				fprintf(fd, "%s", list->data->requirements[i]);
			else
				fprintf(fd, "%s,", list->data->requirements[i]);
		}
		fprintf(fd, "%s", "|");
		for ( i = 0; i < list->data->adds_req_nums; i++ )
		{
			if ( (i+1) == list->data->adds_req_nums )
				fprintf(fd, "%s", list->data->additional_requirements[i]);
			else
				fprintf(fd, "%s,", list->data->additional_requirements[i]);
		}
		fprintf(fd, "%s", "\r\n");

		list = list->next;
	}
	
	fclose(fd);

	return 1;
}

#endif
