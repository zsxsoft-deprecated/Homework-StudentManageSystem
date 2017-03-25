#include "stdafx.h"
#include "types.h"
#include <stdio.h>
#include "utils.h"

#define READ_JSON(variable, file, do_func)\
	{\
		unsigned char* data = types_read_file(file);\
		variable = cJSON_Parse((char *)data);\
		do_func;\
		free(data);\
	}
#define READ_ARRAY_JSON(variable, file)\
	{\
		READ_JSON(variable, file, {\
			variable##_length = cJSON_GetArraySize(variable);\
		})\
	}
#define READ_OBJECT_JSON(variable, file)\
	{\
		READ_JSON(variable, file, {})\
	}


#define EXPORT_ARRAY_FUNCTION(name, json_variable)\
	cJSON* json_variable = NULL;\
	int json_variable##_length = 0;\
	char* get_##name(int index) {\
		if (json_variable##_length <= index) return NULL;\
		return cJSON_GetArrayItem(json_variable, index)->valuestring;\
	}\
	\
	void dump_##json_variable(void) {\
		int i;\
		for (i = 0; i < json_variable##_length; i++) {\
			const char *utf8_data = cJSON_GetArrayItem(json_variable, i)->valuestring;\
			unsigned char *gbk_data = NULL; \
			utf8_to_gbk_all((unsigned char*)utf8_data, &gbk_data); \
			printf("%d = %s\t", i, (const char*)gbk_data); \
			free(gbk_data);\
		}\
	}

#define EXPORT_OBJECT_FUNCTION(name, json_variable)\
	cJSON* json_variable = NULL;\
	char* get_##name(char* name) {\
		return cJSON_GetObjectItem(json_variable, name)->valuestring;\
	}\
	\
	void dump_##json_variable(void) {\
		cJSON *data = ##json_variable->child;\
		while (data != NULL) {\
			const char *utf8_data = data->valuestring;\
			unsigned char *gbk_data = NULL; \
			utf8_to_gbk_all((unsigned char*)utf8_data, &gbk_data); \
			printf("%s = %s\t", data->string, (const char*)gbk_data); \
			free(gbk_data);\
			data = data->next;\
		}\
	}

cJSON* colleges = NULL;
int colleges_length = 0;
	
EXPORT_ARRAY_FUNCTION(admission, admissions)
EXPORT_ARRAY_FUNCTION(nation, nations)
EXPORT_ARRAY_FUNCTION(source, sources)
EXPORT_ARRAY_FUNCTION(instructor, instructors)

EXPORT_OBJECT_FUNCTION(nationality, nationalities)
EXPORT_OBJECT_FUNCTION(political_status, political_statuses)

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
	
	cJSON* json_variable = NULL;

	return buffer;
}

void types_constructor(void) {
	READ_ARRAY_JSON(admissions, "admissions");
	READ_ARRAY_JSON(colleges, "colleges");
	READ_ARRAY_JSON(nations, "nations");
	READ_ARRAY_JSON(sources, "sources");
	READ_OBJECT_JSON(nationalities, "nationalities");
	READ_OBJECT_JSON(political_statuses, "political_status");
	READ_OBJECT_JSON(instructors, "instructor");
}

void types_destructor(void) {
	cJSON_Delete(admissions);
	cJSON_Delete(colleges);
	cJSON_Delete(nationalities);
	cJSON_Delete(nations);
	cJSON_Delete(sources);
	cJSON_Delete(political_statuses);
	cJSON_Delete(instructors);
}


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

#undef READ_JSON
#undef EXPORT_ARRAY_FUNCTION
#undef EXPORT_OBJECT_FUNCTION