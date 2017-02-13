#include "stdafx.h"
#include "studentStruct.h"
#include "studentSort.h"
#include "student.h"
#include "dateHelper.h"
#include "validator.h"

#define INPUT_AND_RETURN(function_name, input_value, validator_handler, error_message) \
  err = function_name(input_value, validator_handler); \
  if (err != 0) { \
    printf("%s\n", error_message); \
    if (input_value != NULL) { \
      free(input_value); \
    } \
    return err; \
  }
#define ERROR_RETURN(error_message) { \
	printf("%s\n", error_message); \
	continue; }

#define DO_STATISTIC_FUNCTION(key, function_name) \
	case key:\
		qsort(students, student_length + 1, sizeof(Student), function_name); \
		break;

#define INPUT_DATA(prompt_data, input_format, input_value, validator_handler, error_message) \
	if (output_prompt) printf(prompt_data); \
	if (fscanf(stream, input_format, input_value) == 1) {\
		em.input_value = input_value;\
		if (validator_handler(input_value) != 0) ERROR_RETURN(error_message); \
	}\
	else {\
		break_flag = 1; \
		break; \
	}

Student students[1000];
int student_length = -1;

void read_from_file(void) {
	FILE *fp = NULL;
	open_file(&fp);
	if (fp != NULL) {
		input_student_data_from_stream(&fp);
	}
}

int id_validator(char *data) {
	int i;
	if (strlen(data) > 4) return 1;
	for (i = 0; i <= student_length; i++) {
		if (!strcmp(students[i].id, data)) return 1;
	}
	return 0;
}

void input_student_data(void) {
	input_student_data_from_stream(NULL);
}

int input_student_data_from_stream(FILE **stream_position) {

	int output_prompt = 1;
	int break_flag = 0;
	char *id = (char*)malloc(sizeof(char) * 10);
	char *name = (char*)malloc(sizeof(char) * 10);
	char *sex = (char*)malloc(sizeof(char) * 10);
	char *birthday = (char*)malloc(sizeof(char) * 10);
	double wage, subsidy, withholding;
    
	FILE* stream;
	if (stream_position == NULL) {
		stream = stdin;
	}
	else {
		stream = *stream_position;
		output_prompt = 0;
	}
	/*
	ѧ�š��������Ա𡢹��𡢳������¡����塢������ò�����֤�š�ѧ�������ѧ���¡���ѧ��ʽ��ѧԺ��רҵ��ѧ�ơ�������Ρ��꼶���༶�š�ָ��Ա����Щ��Ϣ���԰����Լ���Ϊ����ķ�ʽ�������Ϻõ�ѧ��������ѧ���ݿ�Ĳ������ݣ���ŵ���ͬ�������ļ��

	*/
	
	while (1) {
		if (break_flag == 1) break;
		struct Student em = *(Student*)malloc(sizeof(Student));
		while (1) {
			// INPUT_DATA("������ѧ��ѧ�ţ�", "%s", id, id_validator, "ѧ��ӦΪ9λ���֣�ѧԺ���루2λ��+��ѧ��ݣ�2λ��+���ֱ��루5λ�������磺221500555��");
			// INPUT_DATA("������ѧ��������", "%s", name, name_validator, "�������������ȷ���磺�����죬2��20�����֣��й��ˣ��������ˣ�������40���ַ���");
			// INPUT_DATA("������ѧ���Ա���/Ů����", "%s", sex, sex_validator, "�Ա��������ֻ��Ϊ�л�Ů��");

			
			// 4�������й�/����/...��ö��ֵ��������һ���ļ��������еġ��������ơ���Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ���й���
			// 5���������ڣ����磺1997-05-19��������ѧʱ��10<=����<=100��
			// 6�����壺����/����/...��ö��ֵ��������һ���ļ��������еġ��������ơ���Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ�����塣
			// 7��������ò��������Ա/Ⱥ��/...��ö��ֵ��������һ���ļ��������еġ�������ò����Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ��������Ա��
			// INPUT_DATA("���������֤�ţ�", "%s", gov_id, gov_id_validator)
			/*
8�����֤�ţ��ԡ����𡱷ǡ��й�����ѧ������Ϊ�ա�18λ��ǰ17λ�������֣����һλΪ0��9��X������������6λ��+�������ڣ�8λ��+˳���루3λ��+У���루1λ������˳���롱Ϊż������Ů�ԣ�Ϊ�����������ԣ����磺350103199705191114�������顰�����������Ƿ�Ϸ������������ڡ��Ƿ��ǰ��ġ��������ڡ�һ�£���˳���롱�Ƿ��ǰ��ġ��Ա�ƥ�䣬��У���롱���Ǹ���ǰ��ʮ��λ�����룬����ISO 7064:1983.MOD 11-2У�����������ļ����룩�Ƿ���ȷ�ȡ�
9��ѧ����𣺳���Ӧ��/ũ��Ӧ��/...��ö��ֵ��������һ���ļ��������еġ�ѧ����𡱣�Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ���ޡ�
10����ѧ���£����磺2015-09-07������͡�ѧ�š��ġ���ѧ��ݡ���һ�£������������(������¼��)��8=<�·�<=10��
11����ѧ��ʽ����ͨ��ѧ/��������/ר����/...��ö��ֵ��������һ���ļ��������еġ���ѧ��ʽ����Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ����ͨ��ѧ��
12��ѧԺ���ƣ����ѧԺ/��ѧ��������ѧѧԺ/...��ö��ֵ��������һ���ļ��������еġ�ѧԺ���ơ���Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ���ޡ�
13��רҵ���ƣ��������/�������ѧ��Ӧ��/...��ö��ֵ��������һ���ļ��������еġ�ѧԺ����/רҵ���ƶ��ձ���Ȼ�����û�ѡ�񣨲���ֱ�����룩��Ĭ��ֵ���ޡ�
14���꼶�����磺2015������͡�ѧ�š��ġ���ѧ��ݡ���һ�£������������(������¼��)��Ĭ��ֵ��2000+��ѧ�š��ġ���ѧ��ݡ���
15��ѧ�ƣ�Ĭ��ֵ��4�꣬����͡�רҵ���ơ���Ӧ�ġ�ѧ�ơ�һ�£���һ���ļ��������Ϣ����
16��������Σ�Ĭ��ֵ�����ƣ�����͡�רҵ���ơ���Ӧ�ġ�������Ρ�һ�£���һ���ļ��������Ϣ����
17���༶�ţ��ַ����ͣ����磺01��ȡֵ��Χ��01��10��
18��ָ��Ա�����磺�ֺ�ϼ��
			*/
		}
		
		
		
		if (amount_validator(&wage) != 0) ERROR_RETURN("��λ�����������");
		if (amount_validator(&subsidy) != 0) ERROR_RETURN("�����ܶ��������");
		if (amount_validator(&withholding) != 0) ERROR_RETURN("�����ܶ��������");

		em.id = id;
		em.name = name;
		em.sex = sex;
		em.wage = wage;
		em.subsidy = subsidy;
		em.withholding = withholding;
		em.payable = wage + subsidy - withholding;

		{
			Birthday *p = (Birthday*)malloc(sizeof(Birthday));
			birthday_format(p, birthday);
			if (p == NULL || !is_date_legal(p->year, p->month, p->day)) {
				ERROR_RETURN("����������󣬿��ܳ���18-60�꣬�����ʽ��\n");
			}
			em.birthday = *p;
		}
		students[++student_length] = em;
		
		save_file(students, student_length);

		id = (char*)malloc(sizeof(char) * 10);
		name = (char*)malloc(sizeof(char) * 10);
		sex = (char*)malloc(sizeof(char) * 10);
		birthday = (char*)malloc(sizeof(char) * 10);
		/*
		INPUT_AND_RETURN(input_int, &em.id, id_validator, "ѧ��ID������󣬱���Ϊ���֣��������ظ����ҳ��Ȳ��ɴ���4��");
		INPUT_AND_RETURN(input_wchar, em.name, name_validator, "���ֱ�����ڵ����������֣���С�ڵ����ĸ���");
		INPUT_AND_RETURN(input_wchar, em.sex, sex_validator, "�Ա��������ֻ��Ϊ�л�Ů��");
		INPUT_AND_RETURN(input_int, &em.wage, amount_validator, "��λ�����������");
		INPUT_AND_RETURN(input_int, &em.subsidy, amount_validator, "�����ܶ��������");
		INPUT_AND_RETURN(input_int, &em.withholding, amount_validator, "�����ܶ��������");
		*/
	}

	if (stream_position == NULL) eat_line();
	return 0;
}

void output_single_student_data(Student data) {
	printf("%6s ", data.id);
	printf("%10s %4s", data.name, data.sex);
	printf("     %04d.%02d.%02d%10.2lf  %10.2lf  %10.2lf  %10.2lf\n",
		data.birthday.year, data.birthday.month, data.birthday.day,
		data.wage, data.subsidy, data.withholding,
		data.payable
	);
}

void output_student_data(void) {
	int i;
	printf("����ѧ������Ϊ��%d\n\n", student_length + 1);
	printf("��ѧ���š������������Ա𡡡��������ڡ�����λ���ʡ��������ܶ�������ܶ��Ӧ������\n");
	printf("====================================================================================\n");
	for (i = 0; i <= student_length; i++) {
		output_single_student_data(students[i]);
	}
	printf("====================================================================================\n");
}

void output_student_statistic(void) {
	int i;
	double maxWage = DBL_MIN, minWage = DBL_MAX;
	double totalWage = .0, totalSum = .0;
	int maleSum = .0, femaleSum = .0;
	double maleWage = .0, femaleWage = .0;
	for (i = 0; i <= student_length; i++) {
		totalWage += students[i].payable;
		totalSum++;
		if (strcmp(students[i].sex, "��") == 0) {
			maleSum++;
			maleWage += students[i].payable;
		}
		else {
			femaleSum++;
			femaleWage += students[i].payable;
		}
		if (students[i].payable > maxWage) maxWage = students[i].payable;
		if (students[i].payable < minWage) minWage = students[i].payable;
	}

	printf("Ӧ���������Ϊ��%.2lf�����Ϊ��%.2lf������ѧ��Ӧ�������ܶ�Ϊ��%.2lf\n\n", maxWage, minWage, totalWage);
	printf("����ѧ������Ϊ��%d��Ů��ѧ������Ϊ��%d\n\n", maleSum, femaleSum);
	printf("Ӧ������ƽ��ֵΪ��%.2lf(���У�����ƽ��ֵΪ��%.2lf��Ů��ƽ��ֵΪ��%.2lf)\n\n", totalWage / totalSum, maleWage / maleSum, femaleWage / femaleSum);

}

void find_student(void) {
	int i;
	char *id = (char*)malloc(sizeof(char) * 100);
	printf("����ѧ���ţ�");
	eat_line();
	scanf("%s", id);
	for (i = 0; i <= student_length; i++) {
		if (!strcmp(id, students[i].id)) {
			output_single_student_data(students[i]);
			return;
		}
	}
	printf("û�ҵ��ˣ�");
}

void find_and_delete_student(void) {
	int i;
	char *id = (char*)malloc(sizeof(char) * 100);
	printf("����ѧ���ţ�");
	eat_line();
	scanf("%s", id);
	for (i = 0; i <= student_length; i++) {
		if (!strcmp(id, students[i].id)) {
			struct Student lastIndex = students[student_length];
			students[i] = lastIndex;
			//free((void*)&students[student_length]);
			//students[student_length] = NULL;
			student_length--;
			save_file(students, student_length);
			return;
		}
	}
	printf("û�ҵ��ˣ�");
}


void display_sort_menu(void) { // ����дsubmenu��
	char w;
	printf("����ʽѡ��˵�\n\
== == == == == == == == == == == == == == == ==\n\
1�� ��ѧ���Ŵ�С��������\n\
2 : ��ѧ���ŴӴ�С����\n\
3 : ��ѧ����������\n\
4 : ��ѧ���Ա�����\n\
5 : ��ѧ�������С��������\n\
6 : ��ѧ������Ӵ�С����\n\
7 : ��ѧ��Ӧ�����ʴ�С��������\n\
8 : ��ѧ��Ӧ�����ʴӴ�С����\n\
q : �������˳�\n\
== == == == == == == == == == == == == == == == \n");
	eat_line();
	scanf("%c", &w);
	switch (w) {
		DO_STATISTIC_FUNCTION('1', sort_by_id_asc)
		DO_STATISTIC_FUNCTION('2', sort_by_id_desc)
		DO_STATISTIC_FUNCTION('3', sort_by_name_asc)
		DO_STATISTIC_FUNCTION('4', sort_by_sex_asc)
		DO_STATISTIC_FUNCTION('5', sort_by_date_asc)
		DO_STATISTIC_FUNCTION('6', sort_by_date_desc)
		DO_STATISTIC_FUNCTION('7', sort_by_wage_asc)
		DO_STATISTIC_FUNCTION('8', sort_by_wage_desc)
	default:
		return;
	}
	output_student_data();
}

