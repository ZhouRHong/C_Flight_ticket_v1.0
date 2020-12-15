#include "my_head.h"

//录入航班
int entry_flight(struct ticket_library *ticket_library_head)
{
	struct ticket_library *new = NULL;
	struct ticket_library *find = NULL;
	struct ticket_library *p = NULL;
	new = (struct ticket_library *)malloc(sizeof(struct ticket_library));
	if (new == NULL)
	{
		printf("malloc2395 error\n");
	}

	printf("请输入你要录入的航班号（例如-A001-A代表机型0001代表编号）：\n");

	scanf("%s", new->number);

	for (find = ticket_library_head; find != NULL; find = find->next)
	{
		if (strcmp(new->number, find->number) == 0)
		{
			free(new);
			printf("该航班号已被使用请重新输入！！！\n");

			return 0;
		}
	}

	printf("请输入起点站:");
	scanf("%s", new->staddress);

	printf("请输入终点站:");
	scanf("%s", new->arraddress);

	printf("请输入班期:");
	scanf("%s", new->date);

	printf("请输入机型:");
	scanf("%s", new->type);

	printf("请输入起飞时间:");
	scanf("%s", new->stime);


	printf("请输入票价：");
	scanf("%d", &(new->price));

	printf("请输入总票数：");
	scanf("%d", &(new->ticket_num));



	new->next = NULL;
	for (p = ticket_library_head; p->next != NULL; p = p->next);
	p->next = new;

	save_flight_fun(new);
}

//删除航班后退票
int delete_ticket_fun(char *flight_number,
                      struct userinfo *user_name,
                      struct ticket_library *ticket_library_head)
{
	struct user_ticket *p = NULL;
	struct user_ticket *q = NULL;//p q 用于删除节点
	struct user_ticket *delete_ticket = NULL;// 用于删除节点后遍历保存到票库链
	struct ticket_library *ticket = NULL; //用于查找票库的节点给余票加一
	int i = 0;
	for (q = &user_name->user_ticket_head, p = user_name->user_ticket_head.next; p != NULL; q = p, p = p->next)
	{
		if (strcmp(p->number , flight_number) == 0) //找到要退票的节点
		{
			user_name->balance += p->price + 200 ; //退回包括保险费外所有的钱加200赔偿
			save_userinfo_fun(user_name);

			for (ticket = ticket_library_head->next; ticket != NULL; ticket = ticket->next) //遍历票库
			{
				if (strcmp(ticket->number, flight_number) == 0)
				{
					ticket->ticket_num += 1; //对应的余票加一
					save_flight_fun(ticket);//save
					printf("删除航班成功！！！\n");
				}
			}

			q->next = p->next;
			free(p);
			save_delete_ticket_fun(user_name);//删除后保存到用户票链
			return 0;
		}
		else if (p->next == NULL)
		{
			printf("没有用户购买此航班，删除航班成功！！！\n");
			return -1;
		}
	}
}

//寻找所有购买了此航班的人
int find_buy_ticket(char *flight_number,
                    struct userinfo *userinfo_head,
                    struct ticket_library *ticket_library_head,
                    struct ticket_library *recycle_head)
{
	struct userinfo *p = NULL; //用来查找用户信息
	struct user_ticket *q = NULL; //用来查找用户买的票

	//int i = 0;

	struct ticket_library *a = NULL;
	struct ticket_library *b = NULL;//a b 用来删除航班；

	for (p = userinfo_head->next; p != NULL; p = p->next)//遍历所有用户
	{

		for (q = p->user_ticket_head.next; q != NULL; q = q->next)//遍历所有用户的所有票
		{
			if (strcmp(q->number, flight_number) == 0)//找到所有购买了此航班的票
			{
				delete_ticket_fun(flight_number, p, ticket_library_head); //退掉删除的票并赔偿200元

				for (a = ticket_library_head, b = ticket_library_head->next; b != NULL; a = b, b = b->next)
				{


					if (strcmp(b->number , flight_number) == 0)
					{
						//把要删除的航班移动到回收站
						char path[50] = {0};
						sprintf(path, "mv ./flight_data/%s.txt ./recycle", b->number);
						DIR *dp = opendir("./flight_data");
						if (dp == NULL)
							printf("打开目录失败!\n");

						closedir(dp);

						a->next = b->next;//把找到的要删除的航班的节点断开接到上一个
						b->next = recycle_head->next;//把断开的节点头插到回收链
						recycle_head->next = b;
						save_delete_ticket_fun(p);//删除后保存到用户票链


						return 0;
					}

				}
				return -1;

			}
			else if (q->next == NULL)
			{
				if (p->next != NULL)
				{
					break;
				}

				for (a = ticket_library_head, b = ticket_library_head->next; b != NULL; a = b, b = b->next)
				{

					if (strcmp(b->number , flight_number) == 0)
					{
						//把要删除的航班移动到回收站
						char path[50] = {0};
						sprintf(path, "mv ./flight_data/%s.txt ./recycle", b->number);
						DIR *dp = opendir("./flight_data");
						if (dp == NULL)
							printf("打开目录失败!\n");
						system(path);
						closedir(dp);

						a->next = b->next;//把找到的要删除的航班的节点断开接到上一个
						b->next = recycle_head->next;//把断开的节点头插到回收链
						recycle_head->next = b;

						return 0;
					}
				}
				return -1;

			}

		}
	}

	return 0;
}


//删除航班
int  delete_flight(struct userinfo *userinfo_head,
                   struct ticket_library *ticket_library_head,
                   struct ticket_library *recycle_head)
{
	system("clear");
	int i = 0;
	int choise;
	char flight_number[10];
	struct ticket_library *find_flight = NULL; //用于遍历所有航班票库


	check_all_flight(ticket_library_head);//先显示所有航班

	printf("请输入你要删除的航班:");
	scanf("%s", flight_number);

	//遍历航班票库
	for (find_flight = ticket_library_head->next; find_flight != NULL; find_flight = find_flight->next)
	{
		if (strcmp(flight_number, find_flight->number) == 0)
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       find_flight->number,
			       find_flight->staddress,
			       find_flight->arraddress,
			       find_flight->date,
			       find_flight->type,
			       find_flight->stime,
			       find_flight->price,
			       find_flight->ticket_num
			      );


			printf("请确认是否要删除此航班（0-确认 1-取消）:");
label2:
			scanf("%d", &choise);
			if (choise == 0)
			{
				find_buy_ticket(flight_number, userinfo_head, ticket_library_head, recycle_head);
			}
			else if (choise == 1)
			{
				return -1;
			}
			else
			{
				printf("输入错误,请重新输入!!!\n");
				goto label2;
			}

			i++;
		}

	}
	if (i == 0)
	{
		printf("没有找到此航班！！！\n");
	}

	return 0;
}
