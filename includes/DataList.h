/*	
 *	Модуль DataList предоставляет контейнер для хранения 
 *	проанализированных строк, а также имеет интерфейс взаимодействия
 *	с контейнером и исходным файлом.
 *
 */

#ifndef DATALIST_H
#define DATALIST_H

/* Системные константы */
enum
{
	STR_URL_SIZE				=				 100,
	STR_NUM_SIZE				=				  10,
	STR_CITY_SIZE				=				 100,
	STR_COMPANY_SIZE			=				 100,
	STR_EXPERIENCE_SIZE			=				 100,
	STR_CONDITIONS_SIZE			=				 100,
	STR_SALARY_SIZE				=				 100,
	STR_VACANCY_NAME_SIZE		=				 300,
	STR_REQUIREMENTS_SIZE		=				 100,
	STR_SIZE					=				 300,
	DATA_BUF_SIZE				=				2048
};

/* Структура данных, хранящая проанализированные строки из файла */
struct Data
{
	char number[STR_NUM_SIZE];
	char url[STR_URL_SIZE];
	char city_name[STR_CITY_SIZE];
	char company_name[STR_COMPANY_SIZE];
	char work_experience[STR_EXPERIENCE_SIZE];
	char work_conditions[STR_CONDITIONS_SIZE];
	char salary[STR_SALARY_SIZE];
	char vacancy_name[STR_VACANCY_NAME_SIZE];
	char requirements[STR_REQUIREMENTS_SIZE][STR_SIZE];
	char additional_requirements[STR_REQUIREMENTS_SIZE][STR_SIZE];
	int req_nums;
	int adds_req_nums;
};
typedef struct Data Data;

/* Структура данных, хранящая элемент контейнера */
struct DataList
{
	int number;
	Data* data;
	struct DataList* next;
	struct DataList* prev;
};
typedef struct DataList DataList;

/* кол-во элементов в контейнере */
int dl_size(DataList* list);

/* вставить элемент в конец контейнера */
int dl_insert(DataList** list_ptr, Data* dat);

/* удалить элемент из контейнера и вернуть указатель на него */
Data* dl_delete(DataList** list_ptr, Data* dat);

/* очистить контейнер, установка флага очищает контейнер вместе с данными */
int dl_clear(DataList** list_ptr, int clear_data_flag);

/* выводит список элементов в поток вывода в отформатированном виде */
void dl_print(DataList* list);

/* производить перенумерование списка */
int dl_reenum(DataList* list);

/* находит элемент с заданным номером в списке и возвращает указатель его в контейнере */
DataList* dl_find(DataList** list_ptr, const char* num);

/* выполняет чтение исходного файла с сериализованными данными(десериализация) */
int dl_read(DataList** list_ptr, const char* input_filename);

/* выполняет запись в целевой файл данных из контейнера(сериализация) */
int dl_write(DataList* list, const char* output_filename);

/* получает номер нового элемента при добавление его в список */
int dl_get_new_number(DataList* list);

#endif
