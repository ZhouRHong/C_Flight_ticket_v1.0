#include "my_head.h"

//主函数
int main(int argc, char const * argv[])
{
	show_welcome_logo();//欢迎界面

	struct ticket_library *ticket_library_head = NULL; //定义航班票库链表头
	ticket_library_head = init_ticket_library_head(); //初始化航班链表头

	struct userinfo *userinfo_head = NULL;//定义用户信息链表头
	userinfo_head = init_userinfo_head();//初始化用户信息链表头

	struct user_ticket *user_ticket_head ;

	struct id_car * id_car_head = NULL; //定义身份证链头
	id_car_head = init_id_car_head();//初始化身份证链头

	struct ticket_library *recycle_head;//定义回收站链表头
	recycle_head = init_ticket_library_head();//初始化回收站链头


	loading_ticket_library(ticket_library_head);//开机加载票库信息

	loading_information(userinfo_head); //开机加载用户信息

	loading_id_library(id_car_head);//开机加载身份库信息

	loading_recycle(recycle_head);//开机加载身份库信息


	main_meum(userinfo_head, ticket_library_head, id_car_head, recycle_head); //主界面
	return 0;
}


