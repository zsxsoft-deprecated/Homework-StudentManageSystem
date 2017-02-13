#include "stdafx.h"
#include "loader.h"

#define FREE(s) if (s != NULL) { free(s); s = NULL; }

#define COPY_STRING(dest, src, index, len)\
	{ \
	strncpy(dest, src + index, len);\
	dest[len] = '\0';\
	}
#define INPUT_AND_DUMP(text, name, json_variable) \
		while (1) {\
			printf("%s%s%s", "请输入", text, "（");\
			dump_##json_variable();\
			printf("）：");\
			if (scanf_s("%d", &name) == EOF) {\
				break_while = 1;\
				break;\
			}\
			if (name >= json_variable##_length) {\
				printf("%s%s%s", "输入的", text, "信息错误，请重新输入。\n");\
				continue;\
			}\
			break;\
		}\
		if (break_while == 1) break;

#define SCANF_UTF8(variable, length)\
	if (scanf_s("%s", temp, length) == EOF) break;\
	utf8_len = gbk_to_utf8((unsigned char *)temp, NULL, NULL);\
	variable = malloc(sizeof(char) * (utf8_len + 1));\
	gbk_to_utf8((unsigned char *)temp, (unsigned char *)variable, utf8_len);

void birthday_format(int *year, int *month, int *day, char *data) {
	char *p = NULL, *q = NULL;
	char y[20], m[20], d[20];
	p = strchr(data, '-');
	if (p) {
		q = strchr(p + 1, '-');
		if (q == NULL) return;
	}
	else {
		p = strchr(data, '.');
		if (p) {
			q = strchr(p + 1, '.');
			if (q == NULL) return;
		}
		else {
			return;
		}
	}

	strcpy(y, data);
	strcpy(m, p + 1);
	strcpy(d, q + 1);
	y[p - data] = '\0';
	m[q - p - 1] = '\0';

	*year = atoi(y);
	*month = atoi(m);
	*day = atoi(d);
	return;
}

int check_gov_id(char *gov_id)
{
	long sum = 0;
	int R[] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
	char table[11] = { '1','0','X', '9', '8', '7', '6', '5', '4', '3', '2' };
	if (18 != strlen(gov_id)) return 0;
	for (int i = 0; i < 18; i++) {
		if (!isdigit(gov_id[i]) && !(('X' == gov_id[i] || 'x' == gov_id[i]) && 17 == i)) {
			return 0;
		}
	}
	for (int i = 0; i <= 16; i++) {
		sum += (gov_id[i] - 48) * R[i];
	}
	if (table[sum % 11] != gov_id[17]) {
		return 0;
	}

	return 1;
}

// 学号、姓名、性别、国别、出生年月、民族、政治面貌、身份证号、学生类别、入学年月、入学方式、学院、专业、学制、培养层次、年级、班级号、指导员

void load_from_stdin() {
	char* id = malloc(sizeof(char) * 20);
	char* admission_date = malloc(sizeof(char) * 60);
	char* gov_id = malloc(sizeof(char) * 20);
	char* instructor = malloc(sizeof(char) * 60);
	char* temp = malloc(sizeof(char) * 100);
	char* sql = NULL;
	char* name = NULL;// = malloc(sizeof(char) * 60);

	char* admission_year_from_id = malloc(sizeof(char) * 3);
	char* college_id = malloc(sizeof(char) * 3);
	char* birthday_from_gov_id = malloc(sizeof(char) * 10);
	char* formatted_birthday = malloc(sizeof(char) * 20);
	char* college_from_json = malloc(sizeof(char) * 10);
	char* discipline_from_json = malloc(sizeof(char) * 10);
	char* training_level_from_json = malloc(sizeof(char) * 10);
	int learn_year_from_json = 0;

	cJSON* college = NULL;
	cJSON *discipline = NULL;

	int nation = 0;
	int sex = 0;
	int nationality = 0;
	int political_status = 0;
	int source = 0;
	int admission = 0;
	int admission_year = 0, admission_month = 0, admission_day = 0;
	int grade = 0;
	int learn_year = 0;

	time_t admission_timestamp = 0;
	time_t birthday_timestamp = 0;

	int utf8_len = 0;

	int break_while = 0;

	while (1) {
		
		printf("请输入学号：");
		if (scanf_s("%s", id, 10) == EOF) break;
		if (strlen(id) != 9) {
			printf("学号位数不正确");
			break;
		}
		
		COPY_STRING(college_id, id, 0, 2)
		college = get_college(college_id);
		if (college == NULL) {
			printf("学号所示的学院不存在。\n");
			break;
		}
		college_from_json = cJSON_GetObjectItem(college, "name")->valuestring;

		{
			int i, found_flag = 0;
			COPY_STRING(temp, id, 4, 1);
			cJSON *disciplines = cJSON_GetObjectItem(college, "disciplines");
			for (i = 0; i < cJSON_GetArraySize(disciplines); i++) {
				discipline = cJSON_GetArrayItem(disciplines, i);
				if (strcmp(temp, cJSON_GetObjectItem(discipline, "id")->valuestring) == 0) {
					discipline_from_json = cJSON_GetObjectItem(discipline, "name")->valuestring;
					found_flag = 1;
				}
			}
			if (found_flag == 0) {
				printf("学号所示的专业不存在。\n");
				break;
			}
		}

		{
			sql = "SELECT count(id) FROM students WHERE id=?";
			sqlite3_stmt *stmt;
			sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, id, -1, SQLITE_TRANSIENT);
			int stat = sqlite3_step(stmt);
			if (stat == SQLITE_ROW) {
				int ret = sqlite3_column_int(stmt, 0);
				if (ret > 0) {
					printf("这个学号已经存在！\n");
					sqlite3_finalize(stmt);
					break;
				}
			}
			else {
				printf("查询出错！：%s\n", sqlite3_errmsg(db));
				sqlite3_finalize(stmt);
				break;
			}
		}

		printf("请输入姓名：");
		SCANF_UTF8(name, 10)

		while (1) {
			printf("请输入性别（0 = 女，1 = 男）：");
			if (scanf_s("%d", &sex, sizeof(sex)) == EOF) {
				break_while = 1;
				break;
			}
			if (sex != 0 && sex != 1) {
				printf("性别输入错误，请重新输入。\n");
				continue;
			}
			break;
		}
		if (break_while == 1) break;
		
		INPUT_AND_DUMP("国家", nation, nations)
		INPUT_AND_DUMP("民族", nationality, nationalities)
		INPUT_AND_DUMP("政治面貌", political_status, political_status)

		while (1) {
			printf("请输入身份证号：");
			if (scanf_s("%s", gov_id, 19) == EOF) {
				break_while = 1;
				break;
			}
			if (check_gov_id(gov_id) == 0 || gov_id[16] % 2 != sex) {
				printf("身份证号输入错误，请重新核实。\n");
				continue;
			}
			break;
		}
		if (break_while == 1) break;

		INPUT_AND_DUMP("学生类别", source, sources)

		while (1) {
			printf("请输入入学年月：");
			if (scanf_s("%s", admission_date, 20) == EOF) {
				break_while = 1;
				break;
			}
			birthday_format(&admission_year, &admission_month, &admission_day, admission_date);
			
			COPY_STRING(admission_year_from_id, id, 2, 2);
			admission_year_from_id[2] = '\0';
			if (admission_year != atoi(admission_year_from_id) + 2000) {
				printf("入学年份和学号不一致，请注意。\n");
			}
			if (admission_month < 8 || admission_month > 10) {
				printf("入学月份不对，请重新核实。\n");
				continue;
			}
			admission_timestamp = get_unix_timestamp(admission_year, admission_month, admission_day);
			break;
		}
		if (break_while == 1) break;
		
		INPUT_AND_DUMP("入学方式", admission, admissions)
		printf("请输入指导员：");
		SCANF_UTF8(instructor, 20)


		int birthday_year, birthday_month, birthday_day;

		COPY_STRING(temp, gov_id, 6, 4);
		birthday_year = atoi(temp);
		COPY_STRING(temp, gov_id, 10, 2);
		birthday_month = atoi(temp);
		COPY_STRING(temp, gov_id, 12, 2);
		birthday_day = atoi(temp);
		birthday_timestamp = get_unix_timestamp(birthday_year, birthday_month, birthday_day);

		COPY_STRING(temp, id, 2, 2);
		grade = atoi(temp);
		COPY_STRING(temp, id, 5, 2);

		sql = "INSERT INTO students \
			(id, name, sex, birth_time, nation, nationality, political_status, gov_id, sources, admission_time, admission_type, college, discipline, grade, learn_year, training_level, class_number, instructor)\
			VALUES \
			(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)\
		";

		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

		sqlite3_exec(db, "begin;", NULL, NULL, NULL);
		sqlite3_bind_text(stmt, 1, id, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 3, sex);
		sqlite3_bind_int(stmt, 4, birthday_timestamp);
		sqlite3_bind_int(stmt, 5, nation);
		sqlite3_bind_int(stmt, 6, nationality);
		sqlite3_bind_int(stmt, 7, political_status);
		sqlite3_bind_text(stmt, 8, gov_id, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 9, source);
		sqlite3_bind_int(stmt, 10, admission_timestamp);
		sqlite3_bind_int(stmt, 11, admission);
		sqlite3_bind_text(stmt, 12, college_from_json, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 13, discipline_from_json, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 14, grade);
		sqlite3_bind_int(stmt, 15, cJSON_GetObjectItem(discipline, "learn_year")->valueint);
		sqlite3_bind_text(stmt, 16, cJSON_GetObjectItem(discipline, "training_level")->valuestring, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 17, temp, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 18, instructor, -1, SQLITE_TRANSIENT);

		int ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE) {
			printf("SQLite Error: %s", sqlite3_errmsg(db));
		}
		sqlite3_exec(db, "commit;", NULL, NULL, NULL);
		sqlite3_finalize(stmt);		
		break;
	}
	
//	FREE(discipline);
	FREE(id);
	FREE(name);
	FREE(college_id);
//	FREE(college);
	FREE(instructor);
	FREE(admission_date);
	FREE(temp);
	FREE(admission_year_from_id);
	FREE(birthday_from_gov_id);
//	FREE(college_from_json);
//	FREE(discipline_from_json);
//	FREE(training_level_from_json);
	FREE(formatted_birthday);
	
}

