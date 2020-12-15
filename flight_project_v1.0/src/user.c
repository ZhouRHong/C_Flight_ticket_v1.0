#include "my_head.h"
//实名认证
int real_name(struct id_car *id_car_head, struct userinfo *user_name)
{
	char real_name[10];//真实姓名
	char id_car_num[10];//身份证号
	int i = 0, j = 0, k = 0;
	int identity;//身份
	struct id_car *find_id;//定义身份证链指针用于遍历身份证库



	printf("=======正在实名登记========\n");
	printf("请输入您的身份证号：\n");
	scanf("%s", id_car_num);
	for (find_id = id_car_head->next; find_id != NULL; find_id = find_id->next)
	{
		if (strcmp(find_id->id_num, id_car_num) == 0)
		{
			printf("请输入您的真实姓名：\n");
			scanf("%s", real_name);
			if (strcmp(find_id->name, real_name) == 0)
			{
				printf("请输入您的身份（普通人输入0、军人输入1、医护人员输入2、残疾人输入3）：\n");
				scanf("%d", &identity);
				if (find_id->status == identity)
				{
					printf("======实名认证成功！！！=======\n");
					user_name->id_name_flag = 1;//认证成功后，用户信息的实名认证标志位置1；
					save_userinfo_fun(user_name);//保存到文件中
					return 0;

				}
				else
				{
					k++;
				}

			}
			else
			{

				j++;
			}
		}
		i++;
	}

	if (i == 0)
	{
		printf("身份证号输入有误（6位）！！！\n" );
		return 0;
	}

	if (j == 1)
	{
		printf("真实姓名输入有误！！！\n");
		return 0;
	}

	if (k == 1)
	{
		printf("身份输入有误（数字）！！！\n");
		return 0;
	}
	return 0;
}

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

