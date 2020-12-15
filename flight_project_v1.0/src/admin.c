#include "my_head.h"

//打印回收站内容
int recycle_fun(struct ticket_library *recycle_head)
{
	system("clear");
	struct ticket_library *p;
	int i = 0;

	for (p = recycle_head->next; p != NULL; p = p->next)
	{
		printf("===============================================================\n");
		printf("航班号   起点站   终点站   班期      机型   起飞时间  票价   余票\n");
		printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
		       p->number,
		       p->staddress,
		       p->arraddress,
		       p->date,
		       p->type,
		       p->stime,
		       p->price,
		       p->ticket_num
		      );
		i++;
	}
	if (i == 0)
	{
		printf("回收站没有东西。\n");
	}
	return 0;
}


//生效保险
int effect_insurance(struct userinfo *userinfo_head)
{
	int insurance;
	char number[10];
	struct userinfo *p = NULL; //用来查找用户信息
	struct user_ticket *q = NULL; //用来查找用户买的票
	system("clear");
	printf("请输入你要生效的航班：\n" );
	scanf("%s", number);
	printf("请输入你要生效的的险种（1-延误险-赔购买此票总费用的双倍，2-新冠险-赔购买此票总费用的10倍）：\n");
	scanf("%d", &insurance);


	for (p = userinfo_head->next; p != NULL; p = p->next)//遍历所有用户
	{

		for (q = p->user_ticket_head.next; q != NULL; q = q->next)//遍历所有用户的所有票
		{
			if (strcmp(q->number, number) == 0)
			{
				if (q->insurance == 1) //找到购买了延误险的用户
				{
					p->balance += q->price * 2; //赔购买此票总费用的双倍
					q->insurance = 0;//陪了后保险标志位为0

					save_delete_ticket_fun(p);
					save_userinfo_fun(p);

				}
				else if (q->insurance == 2)
				{
					p->balance += q->price * 10; //赔购买此票总费用的十倍
					q->insurance = 0;//陪了后保险标志位为0
					save_delete_ticket_fun(p);
					save_userinfo_fun(p);

				}
			}

		}
	}
	printf("保险生效成功！！！\n");
	return 0;
}


//管理员界面
int admin_login_fun(int fd, struct userinfo *userinfo_head,
                    struct ticket_library *ticket_library_head,
                    struct ticket_library *recycle_head)
{
	system("clear");
	int x = 0, y = 0;
	mmap_show_bmp("./bmp_dir/admin.bmp");

	struct input_event buf;
	while (1)
	{


		read(fd, &buf, sizeof(buf));
		if (buf.type == 3 && buf.code == 0)
		{
			x = buf.value;
		}
		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		//查看所有航班
		if (x < 500 && y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				check_all_flight(ticket_library_head);
			}

		}


		//录入航班
		if (x > 500 && y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				entry_flight(ticket_library_head);
			}

		}


		//删除航班
		if (x < 500 && y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				delete_flight(userinfo_head, ticket_library_head, recycle_head);
			}

		}


		//航班回收站
		if (x > 500 && y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				recycle_fun(recycle_head);//打印回收站内容

			}

		}



		//生效保险
		if (x < 500 && y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				effect_insurance(userinfo_head);

			}

		}



		//退出登陆
		if (x > 500 && y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				printf("管理员退出登陆\n");
				return 0;

			}

		}

	}
	return 0;
}


//判断管理员账号密码
int admin_fun(int fd, struct userinfo *userinfo_head,
              struct ticket_library *ticket_library_head,
              struct ticket_library *recycle_head)
{
	system("clear");
	char admin[5];
	char passwd[6];
	printf("请输入您的管理员账号：\n");
	scanf("%s", admin);
	if (strcmp(admin, "root") == 0)
	{
		printf("请输入您的密码：\n");
		scanf("%s", passwd);
		if (strcmp(passwd, "123123") == 0)
		{
			admin_login_fun(fd, userinfo_head, ticket_library_head, recycle_head);
		}
		else
		{
			printf("密码输入错误！！！\n");
		}
	}
	else
	{
		printf("账号输入有误！！！\n");
	}

	return 0;
}
