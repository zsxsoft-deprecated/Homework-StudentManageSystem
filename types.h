#pragma once
#include "third-party/cjson/cJSON.h"
#define EXPORT_ARRAY_FUNCTION(name, json_variable)\
	extern cJSON* json_variable;\
	extern int json_variable##_length;\
	char* get_##name(int);\
	void dump_##json_variable(void);
#define EXPORT_OBJECT_FUNCTION(name, json_variable)\
	extern cJSON* json_variable;\
	char* get_##name(char*);\
	void dump_##json_variable(void);

void types_constructor(void);
void types_destructor(void);
EXPORT_ARRAY_FUNCTION(instructor, instructors)
EXPORT_ARRAY_FUNCTION(admission, admissions)
EXPORT_OBJECT_FUNCTION(nationality, nationalities)
EXPORT_ARRAY_FUNCTION(nation, nations)
EXPORT_ARRAY_FUNCTION(source, sources)
EXPORT_OBJECT_FUNCTION(political_status, political_statuses)
extern cJSON* colleges;
extern int colleges_length;
cJSON* get_college(char*);
#undef EXPORT_ARRAY_FUNCTION
#undef EXPORT_OBJECT_FUNCTION