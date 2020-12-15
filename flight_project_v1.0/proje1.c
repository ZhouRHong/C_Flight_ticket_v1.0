

















//修改密码
int change_passwd(struct userinfo *user_name)
{
	char passwd_buf[10];
	char user_name_passwd1[10];
	char user_name_passwd2[10];

	printf("请输入你的旧密码：\n");
	scanf("%s", passwd_buf);

	if (strcmp(user_name->passwd, passwd_buf) == 0)
	{
		printf("请输入你的新密码：\n");
		scanf("%s", user_name_passwd1);

		printf("二次输入：\n");
		scanf("%s", user_name_passwd2);

		if (strcmp(user_name_passwd1, user_name_passwd2) == 0)
		{
			strcpy(user_name->passwd, user_name_passwd1);
			save_userinfo_fun(user_name);
			printf("=========修改密码成功========\n");
			return 0;
		}
		else
		{
			printf("两次密码不一致！！！\n");
			return -2;
		}
	}
	else
	{
		printf("密码错误！！！\n");
		return -1;
	}

}

//修改年龄
int change_age(struct userinfo *user_name)
{
	int age_buf;
	printf("请输入新的年龄：\n");
	scanf("%d", &age_buf);

	if (age_buf <= user_name->age)
	{
		printf("年龄输入错误！！！\n");
		return -1;
	} else
	{
		user_name->age = age_buf;
		printf("年龄修改成功！！！\n");
	}
	save_userinfo_fun(user_name);
}


//修改用户信息
int	change_user_data(int fd, struct userinfo *user_name)
{
	int x = 0, y = 0;
	struct input_event buf;

	while (1)
	{
		mmap_show_bmp("bmp_dir/chang_fun.bmp");
		read(fd, &buf, sizeof(buf));


		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		if ( y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				change_passwd(user_name);//修改密码

		}
		if ( y < 400 && y > 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				change_age(user_name);//修改年龄

		}
		if (  y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				return 0;

		}
	}
}


//退票
int return_ticket(struct userinfo *user_name, struct ticket_library *ticket_library_head) //退票
{
	system("clear");
	char number[10];
	int insurance;
	struct user_ticket *check = NULL;//用于遍历票链
	struct user_ticket *p = NULL;
	struct user_ticket *q = NULL;//p q 用于删除节点
	struct user_ticket *delete_ticket = NULL;// 用于删除节点后遍历保存到票库链
	struct ticket_library *ticket = NULL; //用于查找票库的节点给余票加一
	printf("下面是您的订单：\n");
	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  价格   保险种类（0-无 1-延误险 2-新冠险）\n");

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
	printf("请输入您要退的航班号：\n");
	scanf("%s", number);

	for (check = user_name->user_ticket_head.next; check != NULL; check = check->next)
	{

		if (strcmp(check->number , number) == 0)
		{
			insurance = check->insurance;//找到用户为此航班买的保险用来判断
		}

	}

	printf("1978inruanace:%d\n", insurance);
	if (insurance > 0) //买了保险的
	{
		for (q = &user_name->user_ticket_head, p = user_name->user_ticket_head.next; p != NULL; q = p, p = p->next)
		{

			printf("p->number:%s\n", p->number);
			if (strcmp(p->number , number) == 0) //找到要退票的节点
			{
				if (user_name->status > 0) //判断是否为特殊身份
				{
					user_name->balance += p->price - 30; //特殊用户退回除了保险费外所有的钱
					save_userinfo_fun(user_name);

					for (ticket = ticket_library_head->next; ticket != NULL; ticket = ticket->next) //遍历票库
					{
						if (strcmp(ticket->number, number) == 0)
						{
							ticket->ticket_num += 1; //对应的余票加一
							save_flight_fun(ticket);//save
						}
					}

				}
				else//普通人，扣除5%的手续费
				{
					user_name->balance += 0.95 * p->price - 30;
					save_userinfo_fun(user_name);
					for (ticket = ticket_library_head->next; ticket != NULL; ticket = ticket->next) //遍历票库
					{
						if (strcmp(ticket->number, number) == 0)
						{
							ticket->ticket_num += 1; //对应的余票加一
							save_flight_fun(ticket);//save
						}
					}
				}

				q->next = p->next;
				free(p);
				save_delete_ticket_fun(user_name);//删除后保存到用户票链


				return 0;
			}
		}

		printf("您没有购买此航班！！！!\n");
	}
	else//没买保险的
	{
		for (q = &user_name->user_ticket_head, p = user_name->user_ticket_head.next; p != NULL; q = p, p = p->next)
		{

			printf("p->number:%s\n", p->number);
			if (strcmp(p->number , number) == 0) //找到要退票的节点
			{
				if (user_name->status > 0) //判断是否为特殊身份
				{
					user_name->balance += p->price ; //特殊用户退回所有的钱
					save_userinfo_fun(user_name);

					for (ticket = ticket_library_head->next; ticket != NULL; ticket = ticket->next) //遍历票库
					{
						if (strcmp(ticket->number, number) == 0)
						{
							ticket->ticket_num += 1; //对应的余票加一
							save_flight_fun(ticket);//save
						}
					}

				}
				else//普通人，扣除5%的手续费
				{
					user_name->balance += 0.95 * p->price ;
					save_userinfo_fun(user_name);
					for (ticket = ticket_library_head->next; ticket != NULL; ticket = ticket->next) //遍历票库
					{
						if (strcmp(ticket->number, number) == 0)
						{
							ticket->ticket_num += 1; //对应的余票加一
							save_flight_fun(ticket);//save
						}
					}
				}

				q->next = p->next;
				free(p);
				save_delete_ticket_fun(user_name);//删除后保存到用户票链


				return 0;
			}
		}

		printf("您没有购买此航班！！！!\n");
	}


	return -1;
}


//重新对应购买保险
int change_ticket_insurance(struct user_ticket *p, struct userinfo *user_name, struct ticket_library *find_ticket)
{
	int price;//票价

	if (p->insurance == 0)//没买保险的
	{
		if (user_name->status == 0)//判断身份
		{
			if (user_name->balance < find_ticket->price) //判断余额是否足够
			{
				printf("余额不足以改签，请充值后重新购票！！！\n");
				return 0;

			}
			else
			{
				if (find_ticket->ticket_num > 0)
				{
					price = find_ticket->price;
					user_name->balance -= price; //用户信息减去余额
					save_userinfo_fun(user_name);
					find_ticket->ticket_num -= 1; //票库对应的航班余票减一
					save_flight_fun(find_ticket);//保存此时的余票
					add_user_ticket(p->insurance, price, user_name, find_ticket); //用户票链添加购买的票
				}
				else
				{
					printf("此航班票已卖完无法改签,票额已经退回,请重新购票！！！\n");
					return 0;
				}

			}


		}
		else//特殊身份
		{

			if (user_name->balance < 0.5 * find_ticket->price) //判断余额是否足够
			{
				printf("余额不足以改签，请充值后重新购票！！！\n");
				return 0;
			}
			else
			{

				if (find_ticket->ticket_num > 0)
				{
					price = 0.5 * find_ticket->price;
					user_name->balance -= price;//用户信息减去余额
					save_userinfo_fun(user_name);
					find_ticket->ticket_num -= 1; //票库对应的航班余票减一
					save_flight_fun(find_ticket);//保存此时的余票
					add_user_ticket(p->insurance, price, user_name, find_ticket); //用户票链添加购买的票
				}
				else
				{
					printf("此航班票已卖完无法改签,票额已经退回,请重新购票！！！\n");
					return 0;
				}
			}
		}
	}
	else//买了保险的 30保费
	{

		if (user_name->status == 0)//判断身份 普通
		{
			if (user_name->balance < find_ticket->price + 30) //判断余额是否足够
			{
				printf("余额不足以改签，请充值后重新购票！！！\n");
				return -1;
			}
			else
			{
				if (find_ticket->ticket_num > 0)//判断票有没有卖完
				{
					price = find_ticket->price + 30;
					user_name->balance -= price; //用户信息减去余额
					save_userinfo_fun(user_name);
					find_ticket->ticket_num -= 1; //票库对应的航班余票减一
					save_flight_fun(find_ticket);//保存此时的余票
					add_user_ticket(p->insurance, price, user_name, find_ticket); //用户票链添加购买的票
				}
				else
				{
					printf("此航班票已卖完无法改签,票额已经退回,请重新购票！！！\n");
					return 0;
				}
			}
		}
		else//半价身份
		{

			printf("user_name->balance:%d\n", user_name->balance);
			if (user_name->balance < 0.5 * find_ticket->price + 30) //判断余额是否足够
			{
				printf("余额不足以改签，请充值后重新购票！！！\n");
				return 0;
			}
			else
			{
				if (find_ticket->ticket_num > 0)
				{
					price = 0.5 * find_ticket->price + 30;
					user_name->balance = user_name->balance - price;//用户信息减去余额
					save_userinfo_fun(user_name);
					find_ticket->ticket_num -= 1; //票库对应的航班余票减一
					save_flight_fun(find_ticket);//保存此时的余票
					add_user_ticket(p->insurance, price, user_name, find_ticket); //用户票链添加购买的票
				}
				else
				{
					printf("此航班票已卖完无法改签,票额已经退回,请重新购票！！！\n");
					return 0;
				}
			}
		}
	}
	return 0;
}



//购买改签后的票
int change_ticket_fun(struct user_ticket *p, char *change_num, struct userinfo *user_name, struct ticket_library *ticket_library_head)
{
	struct user_ticket *user_ticket;//定义寻找用户票的指针
	struct ticket_library *find_ticket;//定义票库指针
	int choise;
	int i = 0;


	for (user_ticket = user_name->user_ticket_head.next; user_ticket != NULL; user_ticket = user_ticket->next) //遍历user_name的票链
	{
		printf("p->number：%s\n", p->number );

		if (strcmp(change_num, user_ticket->number) == 0 ) //如果票链中存在则不可以改签，退出！
		{
			printf("您已购买了此航班的票,无法改签到此航班！！！\n");
			return -1;

		}

	}

	if (strcmp(change_num, p->number) == 0)
	{
		printf("无法改签到同一航班，已经把票款退回，请重新购买！！！\n");
		return -1;
	}

	for (find_ticket = ticket_library_head->next; find_ticket != NULL; find_ticket = find_ticket->next)//遍历票库中的航班
	{

		if (strcmp(change_num, find_ticket->number) == 0)//找到要改签的航班号并打印
		{
			printf("%s     %s       %s       %s   %s     %s  %d     %d\n",
			       find_ticket->number,
			       find_ticket->staddress,
			       find_ticket->arraddress,
			       find_ticket->date,
			       find_ticket->type,
			       find_ticket->stime,
			       find_ticket->price,
			       find_ticket->ticket_num
			      );

			change_ticket_insurance(p, user_name, find_ticket); //判断是否购买保险，减去对应余额
			i++;

		}


	}


	if (i == 0)
	{
		printf("没有此航班！！！\n");
		return -1;
	}

	return 0;
}



// 改签
int change_ticket(struct userinfo *user_name, struct ticket_library *ticket_library_head)
{
	system("clear");
	char number[10];
	char change_num[10];
	struct user_ticket *check = NULL;//用于遍历票链
	struct user_ticket *p = NULL;
	struct user_ticket *q = NULL;//p q 用于删除节点
	struct user_ticket *delete_ticket = NULL;// 用于删除节点后遍历保存到票库链
	struct ticket_library *ticket = NULL; //用于查找票库的节点给余票加一
	printf("下面是您的订单：\n");
	printf("===============================================================\n");
	printf("航班号   起点站   终点站   班期      机型   起飞时间  价格   保险种类（0-无 1-延误险 2-新冠险）\n");

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
	printf("====请注意改签后的票将不享受VIP优惠！！！====\n");

	printf("请输入您要改签的航班号：\n");
	scanf("%s", number);
	for (q = &user_name->user_ticket_head, p = user_name->user_ticket_head.next; p != NULL; q = p, p = p->next)
	{


		if (strcmp(p->number , number) == 0) //找到要退票的节点
		{

			user_name->balance += p->price ; //退回所有的钱
			save_userinfo_fun(user_name);

			for (ticket = ticket_library_head->next; ticket != NULL; ticket = ticket->next) //遍历票库
			{
				if (strcmp(ticket->number, number) == 0)
				{
					ticket->ticket_num += 1; //对应的余票加一
					save_flight_fun(ticket);//save
				}
			}

			q->next = p->next;
			printf("请输入您要改签到的航班号：\n");
			scanf("%s", change_num);
			change_ticket_fun(p, change_num, user_name, ticket_library_head); //购买改签后的票
			free(p);
			save_delete_ticket_fun(user_name);//删除后保存到用户票链

			return 0;
		}
	}
	printf("您没有购买此航班！！！\n");
	return 0;
}


//退单与改签
int refund_and_change(int fd, struct userinfo * user_name, struct ticket_library * ticket_library_head)
{
	int x = 0, y = 0;

	struct input_event buf;

	while (1)
	{
		mmap_show_bmp("bmp_dir/refund.bmp");
		read(fd, &buf, sizeof(buf));


		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		if ( y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				return_ticket( user_name, ticket_library_head);//退票

			}
		}
		if ( y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				change_ticket( user_name, ticket_library_head);//改签
		}
		if ( y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				return 0;
		}


	}
	return 0;
}


//普通用户登陆成功界面
int login_success(int fd,
                  struct userinfo * user_name,
                  struct userinfo * userinfo_head,
                  struct ticket_library * ticket_library_head,
                  struct id_car * id_car_head
                 )
{
	system("clear");
	int x = 0, y = 0;
	struct input_event buf;

	while (1)
	{
		mmap_show_bmp("bmp_dir/login_success.bmp");
		read(fd, &buf, sizeof(buf));

		if (buf.type == 3 && buf.code == 0)
		{
			x = buf.value;
		}
		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		if (x < 340 && y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				check_flight( fd, userinfo_head, ticket_library_head);
		}


		//快速查询
		if (x > 340 && x < 680 && y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				check_fast( fd, userinfo_head, ticket_library_head);
		}


		//购票
		if (x > 680 &&  y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				buy_ticket(fd, user_name, ticket_library_head, id_car_head);
		}


		//订单查询
		if (x < 340 && y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				check_order(user_name);
		}



		//退单与改签
		if (x < 680 && x > 340 && y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				refund_and_change(fd, user_name, ticket_library_head);
		}



		//余额查询
		if (x > 680 && y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				printf("====================您的余额为：%d======================\n", user_name->balance);

		}



		//余额充值
		if (x < 340 && y > 400 )
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				top_up(user_name);

		}



		//修改个人信息
		if (x < 680 && x > 340 && y > 400 )
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

				change_user_data(fd, user_name);

		}


		//返回
		if (x > 680 && y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
				//if (ret == GET_TS_RELEASE_STATUS)
			{
				printf("四级级返回\n");
				return 0;

			}

		}
	}
	close(fd);

	return 0;
}


//用户登陆
int user_login_fun(int fd, struct userinfo * userinfo_head,
                   struct ticket_library * ticket_library_head,
                   struct id_car * id_car_head
                  )
{


	system("clear");
	//1. 请求用户输入需要登录的用户名

	struct input_event buf;

	char u_name[10] = {0};
	char user_passwd[10] = {0};
	int i = 0;
	mmap_show_bmp("./bmp_dir/login.bmp");
	printf("请输入您的用户名：");
	scanf("%s", u_name);

	//2. 检查这个想登录的用户注册过了没有。（在注册链表中寻找这个用户名）
	struct userinfo *user_name = NULL;

	for (user_name = userinfo_head->next; user_name != NULL; user_name = user_name->next)
	{

		if (strcmp(user_name->name, u_name) == 0)
		{
			printf("请输入您的密码：");
			mmap_show_bmp("./bmp_dir/passwd.bmp");
			scanf("%s", user_passwd);
			if (strcmp(user_name->passwd, user_passwd) == 0)
			{
				login_success(fd, user_name, userinfo_head, ticket_library_head, id_car_head
				             );
			} else
			{
				printf("密码错误！！！\n");
				return -1;
			}
			i++;

		}
	}
	if (i == 0)
	{
		printf("该用户不存在！！！\n");
		return -2;
	}

	return -3;
}


//保存航班
void save_flight_fun(struct ticket_library *new)
{
	//1. 拼接路径
	char path[50] = {0};
	sprintf(path, "./flight_data/%s.txt", new->number);
	printf("%s\n", path );

	//2. 拼接内容
	char file_data[500] = {0};
	sprintf(file_data, "%s,%s,%s,%s,%s,%s,%d,%d", new->number,
	        new->staddress,
	        new->arraddress,
	        new->date,
	        new->type,
	        new->stime,
	        new->price,
	        new->ticket_num
	       );
	//3. 打开文件。
	FILE *fp = NULL;
	fp = fopen(path, "w");
	system("pwd");
	if (fp == NULL)
		printf("fopen  ./flight_data/%s.txt  error!\n", new->number);

	//4. 将内容保存到文件中
	fwrite(file_data, 100, 1, fp);

	//5. 关闭文件
	fclose(fp);
	printf("票库已经更新！\n");

	return;
}



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
int delete_ticket_fun(char *flight_number, struct userinfo *user_name, struct ticket_library *ticket_library_head)
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


//选择登陆界面
int select_login_fun(int fd, struct userinfo * userinfo_head,
                     struct ticket_library * ticket_library_head,
                     struct id_car * id_car_head,
                     struct ticket_library *recycle_head
                    )
{
	system("clear");
	int x, y;
	mmap_show_bmp("./bmp_dir/select_login.bmp");

	struct input_event buf;
	while (1)
	{
		/*printf("======================GEC SYSTEM===================\n");
		printf("                                                     \n");
		printf("                       1 普通用户登陆                 \n");
		printf("                       2 管理员登陆                   \n");
		printf("                       3 返回                         \n");
		printf("                                                     \n");
		printf("===================================================\n");
		printf("请输入您的选项：\n");
		scanf("%d", &choise);


		}*/

		read(fd, &buf, sizeof(buf));
		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}

		//普通用户登陆
		if (y < 200)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				user_login_fun(fd, userinfo_head, ticket_library_head, id_car_head);
				return 0;
			}

		}


		//管理员登陆
		if (y > 200 && y < 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				admin_fun(fd, userinfo_head, ticket_library_head, recycle_head); //管理员登陆
				return 0;
			}

		}
		if (y > 400)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				printf("二级返回\n");
				return 0;

			}

		}

	}
	return -1;
}


//保存注册信息函数
void save_userinfo_fun(struct userinfo * new)
{
	//1. 拼接路径
	char path[50] = {0};
	sprintf(path, "./usr_data/%s.txt", new->name);

	//2. 拼接内容
	char file_data[100] = {0};
	sprintf(file_data, "%s,%s,%d,%s,%s,%s,%d,%d,%d,%d", new->name,
	        new->passwd,
	        new->age,
	        new->tel,
	        new->question.s_question,
	        new->question.s_answer,
	        new->status,
	        new->balance,
	        new->id_name_flag,
	        new->vip_flag);

	//3. 打开文件。
	FILE *fp = NULL;
	fp = fopen(path, "w");
	if (fp == NULL)
		printf("fopen save error!\n");

	//4. 将内容保存到文件中
	fwrite(file_data, 100, 1, fp);

	printf("======数据保存成功======\n");

	//5. 关闭文件
	fclose(fp);

	return;

}



//注册界面
int register_fun(struct userinfo * userinfo_head)
{
	system("clear");
	mmap_show_bmp("bmp_dir/register.bmp");
	struct userinfo *new = NULL;
	struct userinfo *find = NULL;
	struct userinfo *p = NULL;
	new = (struct userinfo *)malloc(sizeof(struct userinfo));
	if (new == NULL)
	{
		printf("malloc2 error\n");
	}

	printf("请输入您的用户名：");
	scanf("%s", new->name);

	printf("请输入您的密码:");
	scanf("%s", new->passwd);

	printf("请输入您的年龄:");
	scanf("%d", &(new->age));

	printf("请输入您的电话:");
	scanf("%s", new->tel);


	for (find = userinfo_head; find != NULL; find = find->next)
	{
		if (strcmp(new->tel, find->tel) == 0)
		{
			free(new);
			printf("该电话已经注册！！！\n");
			return 0;
		}
	}

	printf("请输入您的密保问题:");
	scanf("%s", new->question.s_question);

	printf("请输入您的密保答案:");
	scanf("%s", new->question.s_answer);

	printf("请输入您的身份（普通人输入0、军人输入1、医护人员输入2、残疾人输入3）：");
	scanf("%d", &(new->status));
	if (new->status > 4)
	{
		printf("输入格式错误!!!\n");
		return 0;
	}

	new->balance = 0;

	new->id_name_flag = 0;

	new->vip_flag = 0;

	new->next = NULL;
	for (p = userinfo_head; p->next != NULL; p = p->next);
	p->next = new;

	save_userinfo_fun(new);


}


//找回密码
int find_passwd(struct userinfo * userinfo_head)
{
	system("clear");
	char find_passwd_name[20];
	char new_passwd1[10];
	char new_passwd2[10];
	char answer_buf[50];

	printf("请输入您要修改密码的账户：\n");
	scanf("%s", find_passwd_name);
	struct userinfo *change = NULL;
	for (change = userinfo_head->next; change != NULL; change = change->next)
	{
		if (strcmp(change->name, find_passwd_name) == 0)
		{
			printf("密保问题：%s\n", change->question.s_question);
			printf("请输入您的回答：\n");
			scanf("%s", answer_buf);

			if (strcmp(change->question.s_answer, answer_buf) == 0)
			{
				printf("请输入您的新密码：\n");
				scanf("%s", new_passwd1);

				printf("二次输入：\n");
				scanf("%s", new_passwd2);

				if (strcmp(new_passwd1, new_passwd2) == 0)
				{
					strcpy(change->passwd, new_passwd1);
					printf("===修改密码成功！！！===\n");
					return 0;
				}
				else
				{
					//show_color();
					printf("两次密码不一致！！！\n");
					return -1;
				}
			}
			else
			{

				printf("密码错误！！！\n");
				return -2;
			}
		}
	}
	//show_color();
	printf("该用户未注册！！！\n");
	return 0;

}



//显示主界面
int main_meum(struct userinfo * userinfo_head,
              struct ticket_library * ticket_library_head,
              struct id_car * id_car_head,
              struct ticket_library *recycle_head
             )
{

	system("clear");
	int fd;
	fd = open("/dev/input/event0", O_RDONLY);
	if (fd < 0)
	{
		printf("open touch error!\n");
	}
	struct input_event buf;

	int x = 0, y = 0;
	while (1)
	{
		mmap_show_bmp("./bmp_dir/main_meum.bmp");

		read(fd, &buf, sizeof(buf));

		if (buf.type == 3 && buf.code == 1)
		{
			y = buf.value;
		}


		if (y < 150)
		{
			//选择登陆
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)

			{
				select_login_fun(fd, userinfo_head, ticket_library_head, id_car_head, recycle_head);

			}

		}


		if (y < 300 && y > 150)
		{
			//注册
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				register_fun(userinfo_head);

			}
		}

		//找回密码
		if (y < 450 && y > 300)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				find_passwd(userinfo_head);

			}
		}


		if (y > 450)
		{
			if (buf.type == 1 && buf.code == 330 && buf.value == 0)
			{
				printf("一级退出\n");
				show_black_color();
				return 0;
			}

		}

	}
	close(fd);
	return 0;
}



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


