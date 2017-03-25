#pragma once
#include "third-party/cjson/cJSON.h"
#define EXPORT_FUNCTION(name, json_variable)\
	extern cJSON* json_variable;\
	extern int json_variable##_length;\
	char* get_##name(int index);\
	void dump_##json_variable();

void types_constructor();
void types_destructor();
EXPORT_FUNCTION(admission, admissions)
EXPORT_FUNCTION(nationality, nationalities)
EXPORT_FUNCTION(nation, nations)
EXPORT_FUNCTION(source, sources)
EXPORT_FUNCTION(political_status, political_status)
extern cJSON* colleges;
extern int colleges_length;
cJSON* get_college(char*);
#undef EXPORT_FUNCTION