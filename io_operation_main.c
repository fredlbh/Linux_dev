#include <sys/type.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Linux系统下，一切皆文件 */
/*
 * 一个进程可以打开多个文件，Linux系统中一个进程打开的文件数量是
 * 有限制，并不是可以无限制打开很多文件，因为打开的文件需要占用内
 * 存资源，对整个系统造成影响，当超过进程打开最大文件数限制，内核
 * 会发送警告信号给对应进程，然后结束进程。ulimit -n命令查看进程
 * 可打开的最大文件数，默认情况是1024。对于一个进程，文件描述符从
 * 0~1023分配，每个被打开的文件在同一进程都有唯一的文件描述符，如
 * 果文件被关闭，对于文件描述符会被释放。每次给文件分配描述符都是
 * 从最小的没有被使用的描述符开始分配。
 */
int main(int *arg, char **uchar)
{
   char buff[1024 * 1000];
   int fd1, fd2;
   int ret;

/*
 * open函数返回值是int类型数据，执行成功，返回非负整数，该返回值
 * 就是一个文件描述符（file descriptor），对于Linux内核而言，所
 * 有打开的文件都会通过文件描述符进行索引。
 * 当open函数打开一个现有文件或创建一个新文件时，内核会向进程返回
 * 一个文件描述符，用于指代打开的文件，所有执行IO操作的系统调用都
 * 是通过文件描述符来索引到对应的文件。
 */
/*
 * 文件描述符一般从3开始分配，因为0、1、2默认被系统占用
* 0：系统标注输入 1：系统标准输出 2：系统标准错误
 */
   fd1 = open("./src_file", O_RDONLY);
   if (fd1 < 0) {
      return fd1;
   }

   fd2 = open("./dest_file", O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
   if (fd2 < 0) {
      ret = fd2;
      goto out1;
   }

   int offset = lseek(fd1, 500, SEEK_SET);
   if (offset < 0) {
      return offset;
   }

   ret = read(fd1, buff, sizeof(buff));
   if (ret == -1) {
      goto out2;
   }

   ret = write(fd2, buff, sizeof(buff));
   if (ret == -1) {
      goto out2;
   }

   ret = 0;

out2:
   close(fd2);

out1:
   close(fd1);

   return ret;
}