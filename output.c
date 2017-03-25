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

	printf("执行SQL：");
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
			COMPARE("id", "学号")
			else COMPARE("name", "名字")
			else COMPARE("sex", "性别")
			else COMPARE("birth_time", "生日")
			else COMPARE("nation", "国家")
			else COMPARE("nationality", "民族")
			else COMPARE("political_status", "政治面貌")
			else COMPARE("gov_id", "身份证")
			else COMPARE("sources", "类别")
			else COMPARE("admission_time", "入学时间")
			else COMPARE("admission_type", "入学类别")
			else COMPARE("college", "学院")
			else COMPARE("discipline", "专业")
			else COMPARE("grade", "年级")
			else COMPARE("learn_year", "学制")
			else COMPARE("training_level", "计划")
			else COMPARE("class_number", "班级")
			else COMPARE("instructor", "指导员")
			else strcpy(display_text, column_name);
			printf("　　%s　　", display_text);
			if (i % 5 == 4) printf("\n");
			change_color(hConsole, -1, saved_attributes);
			free(display_text);
		}
		printf("\n");
	}
	if (stat != SQLITE_DONE && stat != SQLITE_ROW) {
		printf("SQL执行错误！\n%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
	}
	printf("\n--------------------------------------\n");
	while (stat == SQLITE_ROW) {
		int i;
		
		for (i = 0; i < columns; i++) {
			change_color(hConsole, i, (WORD)NULL);
			const char* name = sqlite3_column_name(stmt, i);
			printf("　　");
			if (strcmp(name, "sex") == 0) {
				printf("%s", sqlite3_column_int(stmt, i) == 0 ? "女" : "男");
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
			printf("　　");
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
	printf("请输入SQL：");
	eat_line();
	gets_s(sql, 2000);
	output_by_statement(sql);
}

void display_delete_menu(void) {
	char* name = malloc(sizeof(char) * 1000);
	char* delete_sql = malloc(sizeof(char) * 1000);
	strcpy(delete_sql, "DELETE FROM students ");
	printf("删除方式选择菜单\n\
== == == == == == == == == == == == == == == ==\n\
1： 按ID删除\n\
2 : 按专业名删除\n\
q : 不删除退出\n\
== == == == == == == == == == == == == == == == \n\
请选择（1-8、q）：\
");
	eat_line();
	char w = (char)getchar();
	switch (w) {
	case '1':
		printf("输入待删除ID：");
		scanf_s("%s", name, 10);
		strcat(delete_sql, " WHERE id = '");
		strcat(delete_sql, name);
		strcat(delete_sql, "'");
		output_by_statement(delete_sql);
		break;
	case '2':
		printf("输入待删除专业：");
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
	printf("按国家统计：\n");
	output_by_statement("SELECT nation, count(id) as 数量 FROM students group by nation");
	printf("按民族统计：\n");
	output_by_statement("SELECT nationality, count(id) as 数量 FROM students group by nationality");
	printf("按专业+年级+性别统计：\n");
	output_by_statement("SELECT discipline, grade, sex, count(id) as 数量 FROM students group by discipline, grade, sex");

}


void display_sort_menu(void) { 
	printf("排序方式选择菜单\n\
== == == == == == == == == == == == == == == ==\n\
1： 按学号从小到大排序\n\
2 : 按学号从大到小排序\n\
3 : 按姓名排序\n\
4 : 按性别排序\n\
5 : 按年龄从小到大排序\n\
6 : 按年龄从大到小排序\n\
q : 不排序退出\n\
== == == == == == == == == == == == == == == == \n\
请选择（1-8、q）：\
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