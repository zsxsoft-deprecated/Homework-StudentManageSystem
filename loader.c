#include "stdafx.h"
#include "loader.h"
#include "utils.h"
#include "third-party/cjson/cJSON.h"
#include "database.h"
#include "output.h"

#define FREE(s) if (s != NULL) { free(s); s = NULL; }

#define COPY_STRING(dest, src, index, len)\
	{ \
	strncpy(dest, src + index, len);\
	dest[len] = '\0';\
	}
#define INPUT_AND_DUMP(text, scanf_if, name, json_variable, verify_code) \
		while (1) {\
			printf("%s%s%s", "������", text, "��");\
			dump_##json_variable();\
			printf("����");\
			if (scanf_if == EOF) {\
				break_while = 1;\
				break;\
			}\
			verify_code\
			break;\
		}\
		if (break_while == 1) break;

#define INPUT_AND_DUMP_ARRAY(text, name, json_variable) \
		INPUT_AND_DUMP(text, scanf_s("%d", &name), name, json_variable, {\
			if (name >= json_variable##_length) {\
				printf("%s%s%s", "�����", text, "��Ϣ�������������롣\n");\
				continue;\
			}\
		})
#define INPUT_AND_DUMP_OBJECT(text, name, json_variable) \
		INPUT_AND_DUMP(text, scanf_s("%s", name, 10), name, json_variable, {\
			if (!cJSON_HasObjectItem(json_variable, name)) {\
				printf("%s%s%s", "�����", text, "��Ϣ�������������롣\n");\
				continue;\
			}\
		})

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

// ѧ�š��������Ա𡢹��𡢳������¡����塢������ò�����֤�š�ѧ�������ѧ���¡���ѧ��ʽ��ѧԺ��רҵ��ѧ�ơ�������Ρ��꼶���༶�š�ָ��Ա

void load_from_stdin(void) {
	char* id = malloc(sizeof(char) * 20);
	char* admission_date = malloc(sizeof(char) * 60);
	char* gov_id = malloc(sizeof(char) * 20);
	char* instructor = NULL; // malloc(sizeof(char) * 60);
	char* temp = malloc(sizeof(char) * 100);
	char* sql;
	char* name = NULL;// malloc(sizeof(char) * 60);

	char* admission_year_from_id = malloc(sizeof(char) * 3);
	char* college_id = malloc(sizeof(char) * 3);
	char* birthday_from_gov_id = malloc(sizeof(char) * 10);
	char* formatted_birthday = malloc(sizeof(char) * 20);
	char* college_from_json;
	char* discipline_from_json = malloc(sizeof(char) * 10);

	char* nationality = malloc(sizeof(char) * 10);
	char* political_status = malloc(sizeof(char) * 10);

	cJSON* college;
	cJSON *discipline = NULL;

	int nation = 0;
	int sex = 0;
	int source = 0;
	int admission = 0;
	int admission_year = 0, admission_month = 0, admission_day = 0;
	int grade;

	time_t admission_timestamp = 0;
	time_t birthday_timestamp;

	int utf8_len;

	int break_while = 0;

	while (1) {

		printf("������ѧ�ţ�");
		if (scanf_s("%s", id, 10) == EOF) break;
		if (strlen(id) != 9) {
			printf("ѧ��λ������ȷ");
			break;
		}

		COPY_STRING(college_id, id, 0, 2)
			college = get_college(college_id);
		if (college == NULL) {
			printf("ѧ����ʾ��ѧԺ�����ڡ�\n");
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
				printf("ѧ����ʾ��רҵ�����ڡ�\n");
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
					printf("���ѧ���Ѿ����ڣ�\n");
					sqlite3_finalize(stmt);
					break;
				}
			}
			else {
				printf("��ѯ������%s\n", sqlite3_errmsg(db));
				sqlite3_finalize(stmt);
				break;
			}
		}

		printf("������������");
		SCANF_UTF8(name, 10)
			// scanf_s("%s", name, 10);

			while (1) {
				printf("�������Ա�0 = Ů��1 = �У���");
				if (scanf_s("%d", &sex, sizeof(sex)) == EOF) {
					break_while = 1;
					break;
				}
				if (sex != 0 && sex != 1) {
					printf("�Ա�����������������롣\n");
					continue;
				}
				break;
			}
		if (break_while == 1) break;

		INPUT_AND_DUMP_ARRAY("����", nation, nations);
		INPUT_AND_DUMP_OBJECT("����", nationality, nationalities);
		INPUT_AND_DUMP_OBJECT("������ò", political_status, political_statuses);

		while (1) {
			printf("���������֤�ţ�");
			if (scanf_s("%s", gov_id, 19) == EOF) {
				break_while = 1;
				break;
			}
			if (check_gov_id(gov_id) == 0 || gov_id[16] % 2 != sex) {
				printf("���֤��������������º�ʵ��\n");
				continue;
			}
			break;
		}
		if (break_while == 1) break;

		INPUT_AND_DUMP_ARRAY("ѧ�����", source, sources);

		while (1) {
			printf("��������ѧ���£�");
			if (scanf_s("%s", admission_date, 20) == EOF) {
				break_while = 1;
				break;
			}
			birthday_format(&admission_year, &admission_month, &admission_day, admission_date);

			COPY_STRING(admission_year_from_id, id, 2, 2);
			admission_year_from_id[2] = '\0';
			if (admission_year != atoi(admission_year_from_id) + 2000) {
				printf("��ѧ��ݺ�ѧ�Ų�һ�£���ע�⡣\n");
			}
			if (admission_month < 8 || admission_month > 10) {
				printf("��ѧ�·ݲ��ԣ������º�ʵ��\n");
				continue;
			}
			admission_timestamp = get_unix_timestamp(admission_year, admission_month, admission_day);
			break;
		}
		if (break_while == 1) break;

		INPUT_AND_DUMP_ARRAY("��ѧ��ʽ", admission, admissions);
		printf("������ָ��Ա��");
		// scanf_s("%s", instructor, 20);
		SCANF_UTF8(instructor, 20);


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
		sqlite3_bind_int(stmt, 4, (int)birthday_timestamp);
		sqlite3_bind_int(stmt, 5, nation);
		sqlite3_bind_text(stmt, 6, nationality, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 7, political_status, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 8, gov_id, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 9, source);
		sqlite3_bind_int(stmt, 10, (int)admission_timestamp);
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
	FREE(political_status);
	FREE(nationality);
	//	FREE(college_from_json);
	//	FREE(discipline_from_json);
	//	FREE(training_level_from_json);
	FREE(formatted_birthday);

}

#undef FREE
#undef COPY_STRING
#undef INPUT_AND_DUMP
#undef SCANF_UTF8