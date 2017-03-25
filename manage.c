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
	
	append_menu_item('I', "����ѧ����Ϣ", load_from_stdin);
	append_menu_item('O', "���ѧ����Ϣ", output_student_data);
	/*append_menu_item('T', "���ѧ��ͳ����Ϣ", output_student_statistic);
	append_menu_item('F', "��ѧ���Ų��Ҳ�����乤����Ϣ", find_student);
	*/
	append_menu_item('T', "���ͳ����Ϣ", display_stat);
	append_menu_item('D', "ɾ��", display_delete_menu);
	append_menu_item('S', "����", display_sort_menu);
	append_menu_item('U', "SQL��ѯ", output_student_with_sql);
	append_menu_item('Q', "�˳�����", exit_program);

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

