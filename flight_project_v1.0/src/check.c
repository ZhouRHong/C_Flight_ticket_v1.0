#include "my_head.h"

//查询所有航班
int check_all_flight(struct ticket_library *ticket_library_head)
{
	system("clear");
	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
	struct ticket_library *check = NULL;
	for (check = ticket_library_head->next; check != NULL; check = check->next)
	{
		printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
		       check->number,
		       check->staddress,
		       check->arraddress,
		       check->date,
		       check->type,
		       check->stime,
		       check->price,
		       check->ticket_num
		      );
	}
	return 0;
}


//票价查询
int search_flight_price(int fd, struct userinfo *userinfo_head,
                        struct ticket_library *ticket_library_head)
{
	system("clear");
	int i = 0, price = 0;
	struct ticket_library *pr = NULL;
	printf("请输入航班的价格：\n");
	scanf("%d", &price);


	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
	for (pr = ticket_library_head->next; pr != NULL; pr = pr->next)
	{

		if (price >= pr->price)
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       pr->number,
			       pr->staddress,
			       pr->arraddress,
			       pr->date,
			       pr->type,
			       pr->stime,
			       pr->price,
			       pr->ticket_num
			      );
			i++;
		}

	}
	if (i == 0)
	{
		printf("没有此价格的航班！！！\n");
	}


	return 0;
}



//机型查询
int search_flight_type(int fd, struct userinfo *userinfo_head,
                       struct ticket_library *ticket_library_head)
{
	int i = 0;
	struct ticket_library *ty = NULL;
	char type[5];
	printf("请输入航班的机型：\n");
	scanf("%s", type);


	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
	for (ty = ticket_library_head->next; ty != NULL; ty = ty->next)
	{
		if (strcmp(ty->type, type) == 0)
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       ty->number,
			       ty->staddress,
			       ty->arraddress,
			       ty->date,
			       ty->type,
			       ty->stime,
			       ty->price,
			       ty->ticket_num
			      );
			i++;
		}

	}
	if (i == 0)
	{
		printf("没有此机型的航班！！！\n");
	}


	return 0;
}



//出发日期查询
int search_flight_date(int fd, struct userinfo *userinfo_head,
                       struct ticket_library *ticket_library_head)
{
	int i = 0;
	struct ticket_library *da = NULL;
	char date[10];
	printf("请输入航班的出发时间：\n");
	scanf("%s", date);


	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
	for (da = ticket_library_head->next; da != NULL; da = da->next)
	{
		if (strcmp(da->date, date) == 0)
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       da->number,
			       da->staddress,
			       da->arraddress,
			       da->date,
			       da->type,
			       da->stime,
			       da->price,
			       da->ticket_num
			      );
			i++;
		}

	}
	if (i == 0)
	{
		printf("没有此出发时间的航班！！！\n");
	}


	return 0;
}


//目的地查询
int search_flight_location(int fd, struct userinfo *userinfo_head,
                           struct ticket_library *ticket_library_head)
{
	int i = 0;
	struct ticket_library *loca = NULL;
	char location[5];
	printf("请输入航班的目的地：\n");
	scanf("%s", location);

	struct input_event buf;


	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
	for (loca = ticket_library_head->next; loca != NULL; loca = loca->next)
	{
		if (strcmp(loca->staddress, location) == 0)
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       loca->number,
			       loca->staddress,
			       loca->arraddress,
			       loca->date,
			       loca->type,
			       loca->stime,
			       loca->price,
			       loca->ticket_num
			      );
			i++;
		}

	}
	if (i == 0)
	{
		printf("没有此目的地的航班！！！\n");
	}


	return 0;
}

//条件查询
int search_flight(int fd, struct userinfo *userinfo_head,
                  struct ticket_library *ticket_library_head)
{
	system("clear");
	int y = 0;

	struct input_event buf;

	while (1)
	{
		mmap_show_bmp("./bmp_dir/search_flight.bmp");
		read(fd, &buf, sizeof(buf));


		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		if (y < 120)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

			{
				search_flight_location( fd, userinfo_head, ticket_library_head);
			}

		}
		if ( y < 240 && y > 120)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

			{
				search_flight_date( fd, userinfo_head, ticket_library_head);

			}

		}
		if ( y < 360 && y > 240)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				search_flight_type( fd, userinfo_head, ticket_library_head);

			}

		}
		if ( y < 480 && y > 360)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				search_flight_price(fd, userinfo_head, ticket_library_head);

			}

		}
		if (y > 480)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
				//if (ret == GET_TS_RELEASE_STATUS)
			{
				printf("五级返回\n");
				return 0;

			}

		}
	}

}



//查询航班界面
int check_flight(int fd, struct userinfo * userinfo_head,
                 struct ticket_library *ticket_library_head)
{
	int x = 0, y = 0;

	struct input_event buf;

	while (1)
	{
		mmap_show_bmp("bmp_dir/check_flight.bmp");
		/*	printf("======================GEC SYSTEM===================\n");
			printf("                                                   \n");
			printf("                       1 查询所有航班               \n");
			printf("                       2 条件查询                   \n");
			printf("                       3 返回                      \n");
			printf("                                                   \n");
			printf("===================================================\n");
			printf("请输入您的选项：\n");*/

		read(fd, &buf, sizeof(buf));

		if (buf.type == 3 && buf.code == 0)
		{
			x = buf.value;
		}
		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		if ( y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				check_all_flight(ticket_library_head);

		}
		if (y < 400 && y > 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
				//if (ret == GET_TS_RELEASE_STATUS)
			{
				search_flight(fd, userinfo_head, ticket_library_head);//条件查询

			}

		}
		if (y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
				//if (ret == GET_TS_RELEASE_STATUS)
			{
				return 0;

			}

		}

		/*scanf("%d", &choise);

		switch (choise)
		{
		case 1:
			check_all_flight(ticket_library_head);
			break;
		case 2:
			search_flight(fd, userinfo_head, ticket_library_head);//条件查询
			break;
		case 3:
			return 0;
		default:

			break;*/
	}
}




//快速查询
int check_fast(int fd, struct userinfo *userinfo_head,
               struct ticket_library *ticket_library_head)
{
	system("clear");
	int i = 0;
	struct ticket_library *num = NULL;
	char number[5];
	printf("====================================================================\n");
	printf("航班号有：");
	for (num = ticket_library_head->next; num != NULL; num = num->next)
	{
		printf("%s    ", num->number );
	}

	printf("请输入您要查询的航班号：\n");
	scanf("%s", number);


	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
	for (num = ticket_library_head->next; num != NULL; num = num->next)
	{
		if (strcmp(num->number, number) == 0)
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       num->number,
			       num->staddress,
			       num->arraddress,
			       num->date,
			       num->type,
			       num->stime,
			       num->price,
			       num->ticket_num
			      );
			i++;
		}

	}
	if (i == 0)
	{
		printf("没有此航班！！！\n");
	}


	return 0;
}


//订单查询
int check_order(struct userinfo *user_name)
{
	system("clear");
	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  价格   保险种类（0-无 1-延误险 2-新冠险）\n");
	struct user_ticket *check = NULL;
	for (check = user_name->user_ticket_head.next; check != NULL; check = check->next)
	{
		printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
		       check->number,
		       check->staddress,
		       check->arraddress,
		       check->date,
		       check->type,
		       check->stime,
		       check->price,
		       check->insurance
		      );
	}

	return 0;
}

//充值
int top_up(struct userinfo *user_name)
{
	int balance = 0;
	printf("=========================================================\n");
	printf("=========              充值活动！！！                ======\n");
	printf("=========             充值100送20                   ======\n");
	printf("=========            充值500送200                   ======\n");
	printf("=========         一次充值5000成为终身VIP            ======\n");
	printf("==========================================================\n");

	printf("请输入你要充值的金额：");
	scanf("%d", &balance);
	if (balance == 100)
	{
		user_name->balance += 120;
		printf("充值成功！！！\n");
	}
	else if (balance == 500)
	{
		user_name->balance += 700;
		printf("充值成功！！！\n");
	}
	else if (balance >= 5000)
	{
		user_name->balance += balance;
		user_name->vip_flag = 1;//vip标志位置1，成为会员
		printf("恭喜您成为了终身VIP!!!\n");
	}
	/*else if (balance < 0)
	{
		printf("充值错误！！！\n");
		return 0;
	}*/
	else
	{
		user_name->balance += balance;
		printf("充值成功！！！\n");
	}
	printf("充值后：%d\n", user_name->balance );
	save_userinfo_fun(user_name);
	return 0;
}