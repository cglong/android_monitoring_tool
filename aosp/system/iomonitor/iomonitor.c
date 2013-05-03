#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NETLINK_IOMONITOR 17
#define MAX_PAYLOAD 100000 //Maximum payload size
#define MAX_RINGBUF_SIZE  10

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;


struct io_monitor_data {
	char opcode[6];
	char process[16];
	int pid;
	int address;
	int blocks;
	int tm_mon;
	int tm_mday;
	int tm_year;
	int tm_hour;
	int tm_min;
	int tm_sec;
	long int tm_usec;
};


int main()
{
	FILE *fd2;
	int i;
	
	/*Create Socket
	 * int domain:PF_NETLINK = domain of socket to create (netlink - socket used for kernel to userspace communication)
	 * int type:SOCK_RAW = type of socket to create (SOCK_RAW - raw protocol..might have to change this later for photos)
	 * NETLINK_IOMONITOR = developer defined NETLINK protocol (I defined this in the kernel module, here, and linux/netlink.h)
	 * I used 17 for the IOMONITOR macro since it was the next available integer
	 */
	sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_IOMONITOR);
	
	//Checks
	if(sock_fd<0){
		return -1;
	}
	
	//Initialize socket address struct neccessary for socket binding below
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid(); //current processes' pid, acts like a pointer to this program (used by kernel?)

	//bind the address struct to the socket file descriptor
	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
	
	//Initialize the destination address struct (to Linux kernel)
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0; //Linux kernel
	dest_addr.nl_groups = 0; //Unicast
	
	//The kernel assumes the existence of a message header with each netlink message for consistency
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 0;

	strcpy(NLMSG_DATA(nlh), "Hello");

	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	printf("Sending message to kernel\n");
	sendmsg(sock_fd,&msg,0);
	printf("Waiting for message from kernel\n");

	/* Read message from kernel */
	recvmsg(sock_fd, &msg, 0);
	printf("Received message payload:\n");
	fd2 = fopen("/sdcard/iomonitor_log.csv", "a");
	if (fd2 < 0) {
		printf("Couldn't create a file in /data/local'");
    }
	for(i=0;i<(sizeof(struct io_monitor_data)*MAX_RINGBUF_SIZE);i+=sizeof(struct io_monitor_data)){
		fprintf(fd2,"Op:%s\nProcess:%s\nPID:%d\nAddress:%x\nBlocks:%x\nTimestamp:%d/%d/%d %d:%d:%d:%ld\n\n", 
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->opcode,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->process,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->pid,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->address,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->blocks,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_mon,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_mday,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_year,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_hour,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_min,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_sec,
		   ((struct io_monitor_data *)(NLMSG_DATA(nlh)+i))->tm_usec
		);
	}
	fclose(fd2);
		
	
	return 0;
}
