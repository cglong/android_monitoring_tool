#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include <linux/skbuff.h>
#include <linux/string.h>
#define NETLINK_IOMONITOR 17
#define MAX_RINGBUF_SIZE  10

MODULE_LICENSE("GPL");

struct io_monitor_data {
	int opcode;
	int address;
	int blocks;
};

extern struct io_monitor_data io_ringbuf[];
static struct sock *nl_sk = NULL;

static void nl_data_ready(struct sk_buff *skb){
	struct nlmsghdr *nlh = NULL;
	int pid;
	struct sk_buff *skb_out;
	int msg_size;
	struct io_monitor_data *msg = io_ringbuf;
	int res;
	
	//sk_buff is a linked list of buffers. In our case we only have 1 buffer.
	if(skb == NULL){
		printk("sk buffer is NULL \n");
		return ;
	}
	//Retrieve head pointer to our data which is contained in a message header.
	nlh = (struct nlmsghdr *)skb->data;
	printk(KERN_INFO "%s: received netlink message payload: %s\n", __FUNCTION__, (char *)NLMSG_DATA(nlh));
	
	
	msg_size=sizeof(struct io_monitor_data)*MAX_RINGBUF_SIZE;
	pid = nlh->nlmsg_pid; /*pid of sending process */
	skb_out = nlmsg_new(msg_size,0);
	if(!skb_out){
		printk(KERN_ERR "Failed to allocate new skb\n");
		return;
	} 
	nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
	NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
	//strncpy(nlmsg_data(nlh),msg,msg_size);
	memcpy(nlmsg_data(nlh),msg,msg_size);

	res=nlmsg_unicast(nl_sk,skb_out,pid);

	if(res!=0){
		printk(KERN_INFO "Error while sending back to user\n");
	}
}

static void netlink_create(void){
	//Create the initial socket upon  module_init
	nl_sk = netlink_kernel_create(&init_net,NETLINK_IOMONITOR,0, nl_data_ready,NULL, THIS_MODULE);
}

static int my_module_init(void){
	printk(KERN_INFO "Object Store: Initializing Netlink Socket");
	netlink_create();
	return 0;
}

static void my_module_exit(void){
	sock_release(nl_sk->sk_socket);
	printk(KERN_INFO "Released Object Store");
}

module_init(my_module_init);
module_exit(my_module_exit);
