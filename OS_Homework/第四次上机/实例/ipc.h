#ifndef IPC_H
#define IPC_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/unistd.h>
#define BUFSZ 256

// 建立或获取 ipc 的一组函数的原型说明
int get_ipc_id(char *proc_file, key_t key);
char *set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);

/*信号灯控制用的共同体*/
typedef union semuns {
    int val;
} Sem_uns;

/* 消息结构体 */
typedef struct msgbuf {
    long mtype;
    char mtext[1];
} Msg_buf;

// 生产消费者共享缓冲区即其有关的变量
static key_t buff_key;
static int buff_num;
static char *buff_ptr;

// 生产者放产品位置的共享指针
static key_t pput_key;
static int pput_num;
static int *pput_ptr;

// 消费者取产品位置的共享指针
static key_t cget_key;
static int cget_num;
static int *cget_ptr;

// 生产者有关的信号量
static key_t prod_key;
static key_t pmtx_key;
static int prod_sem;
static int pmtx_sem;

all：缓存区的空或满。 producer：生产者生产同步信号量。 ab、ac、bc：分别是三个消费者的独立信号量。
    // 消费者有关的信号量
    static key_t cons_key;
static key_t cmtx_key;
static int cons_sem;
static int cmtx_sem;
static int sem_val;
static int sem_flg;
static int shm_flg;

#endif