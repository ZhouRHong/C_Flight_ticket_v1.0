# C_Flight_ticket_v1.0

=======================航班购票系统操作文档=========================
1.在pc上打开TFtpd32，选择好项目压缩包所在的目录。

2.在开发板终端SecureCRT上，使用 cd 命令进入家目录。

3.使用tftp 192.168.14.168（你pc的ip）-g -r flight_project.tar.gz

4.等待传输结束。

5：输入以下命令：
[root@GEC6818 ~]#mkdir flight_project   回车

6：输入以下命令：
[root@GEC6818 ~]#mv flight_project.tar.gz /flight_project 回车

7：输入以下命令：
[root@GEC6818 ~]#cd flight_project 回车

8：输入以下命令
[root@GEC6818 ~/flight_project ]#tar zxvf flight_project.tar.gz   然后回车
解压完成

9：输入以下命令
[root@GEC6818 ~/flight_project]#vi /etc/profile  然后回车

10按i进入插入模式  加入以下指令
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/flight_project/lib

11按esc 按shift+: 
输入wq 回车保存退出

12：输入以下命令
[root@GEC6818 ~/flight_project]#./bin/project  然后回车
程序执行，可点击开发板画面提示选项来进行操作。



注意：项目中project1.c为命令行界面文件，可直接编译运行
