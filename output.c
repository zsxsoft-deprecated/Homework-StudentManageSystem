#include "stdafx.h"
#include "output.h"
#include "types.h"
#include "windows.h"
#include "utils.h"
#define DO_SORT_FUNCTION(index, sql) \
	case index: output_by_statement(sql); break;
#define COMPARE(col, str)\
	if (strcmp(col, column_name) == 0) { strcpy(display_text, str);} 
#define PRINT_UTF8(format, data) \
	unsigned char *gbk_data = NULL;\
	utf8_to_gbk_all((unsigned char*)data, &gbk_data);\
	printf(format, (const char *)gbk_data);\
	free(gbk_data);

void change_color(HANDLE hConsole, int i, WORD p) {
	if (i == -1) {
		SetConsoleTextAttribute(hConsole, p);
		return;
	}
	switch (i % 6) {
	case 0:
		SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case 1:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 2:
		SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case 3:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case 4:
		SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case 5:
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	default:
		break;
	}
}

void output_by_statement(char *sql) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	printf("ִ��SQL��");
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s\n\n", sql);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	int stat = sqlite3_step(stmt);
	int rows = 0;
	int columns = sqlite3_column_count(stmt);
	{
		int i;
		for (i = 0; i < columns; i++) {
			change_color(hConsole, i, (WORD)NULL);
			char* display_text = malloc(sizeof(char) * 100);
			const char* column_name = sqlite3_column_name(stmt, i);
			COMPARE("id", "ѧ��")
			else COMPARE("name", "����")
			else COMPARE("sex", "�Ա�")
			else COMPARE("birth_time", "����")
			else COMPARE("nation", "����")
			else COMPARE("nationality", "����")
			else COMPARE("political_status", "������ò")
			else COMPARE("gov_id", "���֤")
			else COMPARE("sources", "���")
			else COMPARE("admission_time", "��ѧʱ��")
			else COMPARE("admission_type", "��ѧ���")
			else COMPARE("college", "ѧԺ")
			else COMPARE("discipline", "רҵ")
			else COMPARE("grade", "�꼶")
			else COMPARE("learn_year", "ѧ��")
			else COMPARE("training_level", "�ƻ�")
			else COMPARE("class_number", "�༶")
			else COMPARE("instructor", "ָ��Ա")
			else strcpy(display_text, column_name);
			printf("����%s����", display_text);
			if (i % 5 == 4) printf("\n");
			change_color(hConsole, -1, saved_attributes);
			free(display_text);
		}
		printf("\n");
	}
	if (stat != SQLITE_DONE && stat != SQLITE_ROW) {
		printf("SQLִ�д���\n%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
	}
	printf("\n--------------------------------------\n");
	while (stat == SQLITE_ROW) {
		int i;
		
		for (i = 0; i < columns; i++) {
			change_color(hConsole, i, (WORD)NULL);
			const char* name = sqlite3_column_name(stmt, i);
			printf("����");
			if (strcmp(name, "sex") == 0) {
				printf("%s", sqlite3_column_int(stmt, i) == 0 ? "Ů" : "��");
			} else if (strcmp(name, "admission_type") == 0) {
				PRINT_UTF8("%s", get_admission(sqlite3_column_int(stmt, i)));
			} else if (strcmp(name, "nationality") == 0) {
				PRINT_UTF8("%s", get_nationality(sqlite3_column_int(stmt, i)));
			} else if (strcmp(name, "nation") == 0) {
				PRINT_UTF8("%s", get_nation(sqlite3_column_int(stmt, i)));
			} else if (strcmp(name, "sources") == 0) {
				PRINT_UTF8("%s", get_source(sqlite3_column_int(stmt, i)));
			} else if (strcmp(name, "political_status") == 0) {
				PRINT_UTF8("%s", get_political_status(sqlite3_column_int(stmt, i)));
			} else if (strcmp(name, "birth_time") == 0 || strcmp(name, "admission_time") == 0) {
				time_t raw_time = sqlite3_column_int(stmt, i);
				PRINT_UTF8("%s", ctime(&raw_time));
			}
			else {
				PRINT_UTF8("%s", sqlite3_column_text(stmt, i));
			}
			printf("����");
			if (i % 5 == 4) printf("\n");
			change_color(hConsole, -1, saved_attributes);
			
		}
		printf("\n--------------------------------------\n");
		stat = sqlite3_step(stmt);
		rows++;
	}
	printf("%d Rows...\n\n", rows);
	SetConsoleTextAttribute(hConsole, saved_attributes);
	sqlite3_finalize(stmt);
}

void output_student_data(void) {
	output_by_statement("SELECT * FROM students");
}

void output_student_with_sql(void) {
	char* sql = malloc(sizeof(char) * 2000);
	printf("������SQL��");
	eat_line();
	gets_s(sql, 2000);
	output_by_statement(sql);
}

void display_delete_menu(void) {
	char* name = malloc(sizeof(char) * 1000);
	char* delete_sql = malloc(sizeof(char) * 1000);
	strcpy(delete_sql, "DELETE FROM students ");
	printf("ɾ����ʽѡ��˵�\n\
== == == == == == == == == == == == == == == ==\n\
1�� ��IDɾ��\n\
2 : ��רҵ��ɾ��\n\
q : ��ɾ���˳�\n\
== == == == == == == == == == == == == == == == \n\
��ѡ��1-8��q����\
");
	eat_line();
	char w = (char)getchar();
	switch (w) {
	case '1':
		printf("�����ɾ��ID��");
		scanf_s("%s", name, 10);
		strcat(delete_sql, " WHERE id = '");
		strcat(delete_sql, name);
		strcat(delete_sql, "'");
		output_by_statement(delete_sql);
		break;
	case '2':
		printf("�����ɾ��רҵ��");
		scanf_s("%s", name, 10);
		strcat(delete_sql, " WHERE discipline = '");
		strcat(delete_sql, name);
		strcat(delete_sql, "'");
		output_by_statement(delete_sql);
		break;
	default:
		return;
	}
	free(delete_sql);
	free(name);
}

void display_stat(void) {
	printf("������ͳ�ƣ�\n");
	output_by_statement("SELECT nation, count(id) as ���� FROM students group by nation");
	printf("������ͳ�ƣ�\n");
	output_by_statement("SELECT nationality, count(id) as ���� FROM students group by nationality");
	printf("��רҵ+�꼶+�Ա�ͳ�ƣ�\n");
	output_by_statement("SELECT discipline, grade, sex, count(id) as ���� FROM students group by discipline, grade, sex");

}


void display_sort_menu(void) { 
	printf("����ʽѡ��˵�\n\
== == == == == == == == == == == == == == == ==\n\
1�� ��ѧ�Ŵ�С��������\n\
2 : ��ѧ�ŴӴ�С����\n\
3 : ����������\n\
4 : ���Ա�����\n\
5 : �������С��������\n\
6 : ������Ӵ�С����\n\
q : �������˳�\n\
== == == == == == == == == == == == == == == == \n\
��ѡ��1-8��q����\
");
	eat_line();
	char w = (char)getchar();
	switch (w) {
	DO_SORT_FUNCTION('1', "SELECT * FROM students ORDER BY id ASC")
	DO_SORT_FUNCTION('2', "SELECT * FROM students ORDER BY id DESC")
	DO_SORT_FUNCTION('3', "SELECT * FROM students ORDER BY name ASC")
	DO_SORT_FUNCTION('4', "SELECT * FROM students ORDER BY sex ASC")
	DO_SORT_FUNCTION('5', "SELECT * FROM students ORDER BY birth_time ASC")
	DO_SORT_FUNCTION('6', "SELECT * FROM students ORDER BY birth_time DESC")
	default:
		break;
	}
}


#undef DO_SORT_FUNCTION
#undef COMPARE
#undef PRINT_UTF8