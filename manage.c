#include "stdafx.h"
#include "menu.h"
#include "types.h"
#include "loader.h"
#include "output.h"
#include "database.h"
#include <locale.h>

void exit_program(void) {
	exit(0);
}

int main (void) {

	print_welcome();
	
	append_menu_item('I', "输入学生信息", load_from_stdin);
	append_menu_item('O', "输出学生信息", output_student_data);
	/*append_menu_item('T', "输出学生统计信息", output_student_statistic);
	append_menu_item('F', "按学生号查找并输出其工资信息", find_student);
	*/
	append_menu_item('T', "输出统计信息", display_stat);
	append_menu_item('D', "删除", display_delete_menu);
	append_menu_item('S', "排序", display_sort_menu);
	append_menu_item('U', "SQL查询", output_student_with_sql);
	append_menu_item('Q', "退出程序", exit_program);

	//read_from_file();

	types_constructor();
	database_constructor();
	do_menu();
#ifdef _DEBUG
	system("pause");
#endif
	types_destructor();
	database_destructor();
    return 0;
}

