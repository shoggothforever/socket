# socket
该项目有两部分，分别对应windows和linux两种操作系统，不过项目的目前主要部分还是在linux

linux下的socket编程以最简单的echo程序作为起点，逐步完善各种功能（多路复用，线程编程，信号量监测，select，poll，epoll机制）
### 涉及知识点：
0. 基本的linux操作系统知识，vim使用方法，一点点计网知识（套接字编程）
1. 利用fork开启多个子进程实现一个服务端对接多个客户端的请求（多路复用）
2. 使用sigaction 机制 reap childproc zombies（stopped procession but still remain)
3. select轮询（数据量小的情况下使用）
4. epoll调度（水平触发和边缘触发模式的使用）
5. 多线程编程
