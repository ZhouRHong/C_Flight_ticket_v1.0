#include "my_head.h"

//显示bmp图片
int mmap_show_bmp(const char *bmp_path)
{
	FILE *fp;
	int n, lcd;
	int i, j;
	int x, y;
	int k;
	char bmp_buf[800 * 480 * 3] = {0};
	char lcd_buf[800 * 480 * 4] = {0};
	char show_buf[800 * 480 * 4] = {0};

	/*1. 打开图片 */
	fp = fopen(bmp_path, "r"); //默认在最开头
	if (fp == NULL)
		printf("fopen error!\n");

	/*2. 打开lcd设备 */
	lcd = open("/dev/fb0", O_RDWR);
	if (lcd < 0)
		printf("open fb0 error!\n");

	/*3. 先跳过54个头数据 */
	fseek(fp, 54, SEEK_SET);

	/*4. 将图片的数据读取到缓冲区中 */
	n = fread(bmp_buf, 800 * 480 * 3, 1, fp);
	if (n != 1)
		printf("fread error!\n");

	/*5. 将24位转32位 */
	for (i = 0, j = 0; i < 800 * 480 * 4; i += 4, j += 3)
	{
		lcd_buf[i]   = bmp_buf[j];
		lcd_buf[i + 1] = bmp_buf[j + 1];
		lcd_buf[i + 2] = bmp_buf[j + 2];
		lcd_buf[i + 3] = 0;
	}

	/* 6. 上下颠倒 */
	for (y = 0; y < 480; y++)
	{
		for (x = 0; x < 800 * 4; x++)
		{
			show_buf[800 * 4 * y + x] = lcd_buf[800 * 4 * (479 - y) + x];
		}
	}

	/* 7. 产生一片内存空间，作为映射 */
	char *p = (char *)mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
	if (p == (void *) - 1)
		printf("mmap error!\n");

	/* 8. 将数据拷贝到内存上 */
	for (k = 0; k < 800 * 480 * 4; k++)
	{
		memcpy(p + k, &show_buf[k], 1);
	}

	/* 7. 关闭文件 */
	munmap(p, 800 * 480 * 4);
	fclose(fp);
	close(lcd);

	return 0;
}

//退出系统界面
void show_black_color()
{
	int lcd;

	int *p = NULL;
	lcd = open("/dev/fb0", O_RDWR);

	if (lcd < 0)
	{
		printf("open lcd error!\n");
	}



	p = (int*)mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
	if (p == (void *) - 1)
	{
		printf("mmap error\n");
	}

	int black_color = 0x00000000;


	mmap_show_bmp("./bmp_dir/exit.bmp");

	sleep(3);
	for (int i = 0; i < 480; ++i)
	{
		for (int a = i * 800; a < i * 800 + 800; a++)
		{
			memcpy(p + a, &black_color, 4);
		}
	}


	munmap(p, 800 * 480 * 4);
	close(lcd);
}


//显示欢迎界面
void show_welcome_logo()
{
	mmap_show_bmp("./bmp_dir/welcome.bmp");
	printf("======================GEC SYSTEM===================\n");
	printf("                                                   \n");
	printf("                       欢迎使用                     \n");
	printf("                                                   \n");
	printf("===================================================\n");
	printf("正在加载信息...\n");


	sleep(2);
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
