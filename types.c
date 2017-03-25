#include "stdafx.h"
#include "types.h"
#include <stdio.h>
#include "utils.h"

#define READ_JSON(variable, file)\
	{\
		unsigned char* data = types_read_file(file);\
		variable = cJSON_Parse(data);\
		variable##_length = cJSON_GetArraySize(variable);\
		free(data);\
	}

#define EXPORT_FUNCTION(name, json_variable)\
	cJSON* json_variable = NULL;\
	int json_variable##_length = 0;\
	char* get_##name(int index) {\
		if (json_variable##_length <= index) return NULL;\
		return cJSON_GetArrayItem(json_variable, index)->valuestring;\
	}\
	\
	void dump_##json_variable() {\
		int i;\
		for (i = 0; i < json_variable##_length; i++) {\
			const char *utf8_data = cJSON_GetArrayItem(json_variable, i)->valuestring;\
			const unsigned char *gbk_data = NULL; \
			utf8_to_gbk_all((unsigned char*)utf8_data, &gbk_data); \
			printf("%d = %s\t", i, (const char*)gbk_data); \
			free(gbk_data);\
		}\
	}


cJSON* colleges = NULL;
int colleges_length = 0;
	
EXPORT_FUNCTION(admission, admissions)
EXPORT_FUNCTION(nationality, nationalities)
EXPORT_FUNCTION(nation, nations)
EXPORT_FUNCTION(source, sources)
EXPORT_FUNCTION(political_status, political_status)


cJSON* get_college(char* str) {
	int i;
	for (i = 0; i < colleges_length; i++) {
		cJSON* w = cJSON_GetArrayItem(colleges, i);
		if (strcmp(str, cJSON_GetObjectItem(w, "id")->valuestring) == 0) {
			return w;
		}
	}
	return NULL;
}

unsigned char* types_read_file(char* json_name) {
	char *path = malloc(sizeof(char) * 255);
	
	strcpy(path, "./data/");
	strcat(path, json_name);
	strcat(path, ".json");
	FILE *fp = fopen(path, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET); 
	unsigned char *buffer = malloc(fsize + 1);
	long readsize = fread(buffer, sizeof(unsigned char), fsize, fp);
	fclose(fp);
	buffer[readsize] = '\0';
	
	return buffer;
}

void types_constructor() {
	READ_JSON(admissions, "admissions");
	READ_JSON(colleges, "colleges");
	READ_JSON(nationalities, "nationalities");
	READ_JSON(nations, "nations");
	READ_JSON(sources, "sources");
	READ_JSON(political_status, "political_status");
}

void types_destructor() {
	cJSON_Delete(admissions);
	cJSON_Delete(colleges);
	cJSON_Delete(nationalities);
	cJSON_Delete(nations);
	cJSON_Delete(sources);
	cJSON_Delete(political_status);
}

#undef READ_JSON
#undef EXPORT_FUNCTION