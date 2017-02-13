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
	学号、姓名、性别、国别、出生年月、民族、政治面貌、身份证号、学生类别、入学年月、入学方式、学院、专业、学制、培养层次、年级、班级号、指导员。这些信息可以按你自己认为合理的方式（基础较好的学生可以自学数据库的部分内容）存放到不同的数据文件里。

	*/
	
	while (1) {
		if (break_flag == 1) break;
		struct Student em = *(Student*)malloc(sizeof(Student));
		while (1) {
			// INPUT_DATA("请输入学生学号：", "%s", id, id_validator, "学号应为9位数字，学院编码（2位）+入学年份（2位）+数字编码（5位），形如：221500555。");
			// INPUT_DATA("请输入学生姓名：", "%s", name, name_validator, "名字输入错误，正确形如：刘国庆，2～20个汉字（中国人），其他人：不超过40个字符。");
			// INPUT_DATA("请输入学生性别（男/女）：", "%s", sex, sex_validator, "性别输入错误，只能为男或女。");

			
			// 4、国别：中国/美国/...可枚举值，必须用一个文件保存所有的“国家名称”，然后让用户选择（不能直接输入）。默认值：中国。
			// 5、出生日期：形如：1997-05-19，限制入学时的10<=年龄<=100。
			// 6、民族：汉族/回族/...可枚举值，必须用一个文件保存所有的“民族名称”，然后让用户选择（不能直接输入）。默认值：汉族。
			// 7、政治面貌：共青团员/群众/...可枚举值，必须用一个文件保存所有的“政治面貌”，然后让用户选择（不能直接输入）。默认值：共青团员。
			// INPUT_DATA("请输入身份证号：", "%s", gov_id, gov_id_validator)
			/*
8、身份证号：对“国别”非“中国”的学生可以为空。18位：前17位都是数字，最后一位为0～9或X，行政区划（6位）+出生日期（8位）+顺序码（3位）+校验码（1位），“顺序码”为偶数代表女性，为奇数代表男性，形如：350103199705191114。必须检查“行政区划”是否合法，“出生日期”是否和前面的“出生日期”一致，“顺序码”是否和前面的“性别”匹配，“校验码”（是根据前面十七位数字码，按照ISO 7064:1983.MOD 11-2校验码计算出来的检验码）是否正确等。
9、学生类别：城镇应届/农村应届/...可枚举值，必须用一个文件保存所有的“学生类别”，然后让用户选择（不能直接输入）。默认值：无。
10、入学年月：形如：2015-09-07，如果和“学号”的“入学年份”不一致，必须给出警告(允许其录入)，8=<月份<=10。
11、入学方式：普通入学/自主招生/专项生/...可枚举值，必须用一个文件保存所有的“入学方式”，然后让用户选择（不能直接输入）。默认值：普通入学。
12、学院名称：软件学院/数学与计算机科学学院/...可枚举值，必须用一个文件保存所有的“学院名称”，然后让用户选择（不能直接输入）。默认值：无。
13、专业名称：软件工程/计算机科学与应用/...可枚举值，必须用一个文件保存所有的“学院名称/专业名称对照表”，然后让用户选择（不能直接输入）。默认值：无。
14、年级：形如：2015，如果和“学号”的“入学年份”不一致，必须给出警告(允许其录入)，默认值：2000+“学号”的“入学年份”。
15、学制：默认值：4年，必须和“专业名称”对应的“学制”一致（用一个文件保存该信息）。
16、培养层次：默认值：本科，必须和“专业名称”对应的“培养层次”一致（用一个文件保存该信息）。
17、班级号：字符类型，形如：01，取值范围：01～10。
18、指导员：形如：林红霞。
			*/
		}
		
		
		
		if (amount_validator(&wage) != 0) ERROR_RETURN("岗位工资输入错误。");
		if (amount_validator(&subsidy) != 0) ERROR_RETURN("补贴总额输入错误。");
		if (amount_validator(&withholding) != 0) ERROR_RETURN("代扣总额输入错误。");

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
				ERROR_RETURN("生日输入错误，可能超过18-60岁，请检查格式。\n");
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
		INPUT_AND_RETURN(input_int, &em.id, id_validator, "学生ID输入错误，必须为数字，不允许重复，且长度不可大于4。");
		INPUT_AND_RETURN(input_wchar, em.name, name_validator, "名字必须大于等于两个汉字，且小于等于四个。");
		INPUT_AND_RETURN(input_wchar, em.sex, sex_validator, "性别输入错误，只能为男或女。");
		INPUT_AND_RETURN(input_int, &em.wage, amount_validator, "岗位工资输入错误");
		INPUT_AND_RETURN(input_int, &em.subsidy, amount_validator, "补贴总额输入错误");
		INPUT_AND_RETURN(input_int, &em.withholding, amount_validator, "代扣总额输入错误");
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
	printf("现有学生总数为：%d\n\n", student_length + 1);
	printf("　学生号　　姓名　　性别　　出生日期　　岗位工资　　补贴总额　　代扣总额　　应发工资\n");
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
		if (strcmp(students[i].sex, "男") == 0) {
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

	printf("应发工资最高为：%.2lf，最低为：%.2lf，所有学生应发工资总额为：%.2lf\n\n", maxWage, minWage, totalWage);
	printf("男性学生人数为：%d，女性学生人数为：%d\n\n", maleSum, femaleSum);
	printf("应发工资平均值为：%.2lf(其中：男性平均值为：%.2lf，女性平均值为：%.2lf)\n\n", totalWage / totalSum, maleWage / maleSum, femaleWage / femaleSum);

}

void find_student(void) {
	int i;
	char *id = (char*)malloc(sizeof(char) * 100);
	printf("输入学生号：");
	eat_line();
	scanf("%s", id);
	for (i = 0; i <= student_length; i++) {
		if (!strcmp(id, students[i].id)) {
			output_single_student_data(students[i]);
			return;
		}
	}
	printf("没找到人！");
}

void find_and_delete_student(void) {
	int i;
	char *id = (char*)malloc(sizeof(char) * 100);
	printf("输入学生号：");
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
	printf("没找到人！");
}


void display_sort_menu(void) { // 不想写submenu了
	char w;
	printf("排序方式选择菜单\n\
== == == == == == == == == == == == == == == ==\n\
1： 按学生号从小到大排序\n\
2 : 按学生号从大到小排序\n\
3 : 按学生姓名排序\n\
4 : 按学生性别排序\n\
5 : 按学生年龄从小到大排序\n\
6 : 按学生年龄从大到小排序\n\
7 : 按学生应发工资从小到大排序\n\
8 : 按学生应发工资从大到小排序\n\
q : 不排序退出\n\
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

