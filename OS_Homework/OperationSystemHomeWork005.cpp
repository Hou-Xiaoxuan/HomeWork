/*
任务：修改模拟内存管理程序代码，添加“循环首次适应”算法。
系统：Windows 10
时间：2021/06/08
*/

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<iostream>
#include<iso646.h>
#define PROCESS_NAME_LEN 32 //进程名字长度
#define MIN_SLICE 10 //最小碎片大小 
#define DEFAULT_MEM_SIZE 1024 // 默认的内存大小
#define DEFAULT_MEM_START 0 //起始地址

#define MA_FF 1                    //首次适应算法
#define MA_BF 2                    //最佳适应算法
#define MA_WF 3                    //最坏适应算法
#define MA_RFF 4                   // 循环首次适应算法

//空闲分区的结构体
typedef struct free_block_type{
    int size;
    int start_addr;
    struct free_block_type *next;
}free_block_type; 
 free_block_type *free_block; // 使用链表储存的空闲块


 //已分配分区的结构体
typedef struct allocated_block{
    int pid; 
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
 }allocated_block;

struct allocated_block *allocated_block_head = NULL; // 使用链表储存的使用块

int mem_size=DEFAULT_MEM_SIZE; 
int ma_algorithm = MA_FF; 
static int pid = 0; 
int flag = 0; 

//函数声明
void display_menu();
int set_mem_size();
void set_algorithm();
void rearrange(int algorithm);
int new_process();
int allocate_mem (struct allocated_block *ab);
void kill_process();
int free_mem (struct allocated_block *ab);
int dispose (struct allocated_block *free_ab);
int display_mem_usage();
allocated_block * find_process(int pid);
void rearrange_FF();
void rearrange_BF();
void rearrange_WF();
void rearrange_RFF();

//初始化空闲分区
free_block_type* init_free_block(int mem_size){
    free_block_type *fb;
    fb=(free_block_type *)malloc(sizeof(free_block_type));
    if(fb==NULL){
        printf("No mem\n");
        return NULL;
        }
    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb; 
}

//显示主菜单
void display_menu(){
    printf("\n");
    printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algorithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");
}

/*设置内存大小*/
int set_mem_size(){
    int size;
    if(flag!=0){ /*flag标志防止内存被再次设置*/
        printf("Cannot set memory size again\n");
        return 0;
        }
    printf("Total memory size =");
    scanf("%d", &size);
    if(size>0) {
        mem_size = size; 
        free_block->size = mem_size;/*设置初始大小为 1024*/
        }
    flag=1; 
    return 1;
 }
 /*选择当前算法*/ // 修改完成
void set_algorithm(){
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");
    std::cout<<"\t4 - Recycle First Fit \n"<<std::endl;

    printf("Please input your choice : ");
    scanf("%d", &algorithm);
    if(algorithm>=1 && algorithm <=4) 
              ma_algorithm=algorithm;
    
    rearrange(ma_algorithm); 
}

/*为每一个进程分配完内存以后重新按已选择的算法再次排序*/ // 修改完成
void rearrange(int algorithm){
    switch(algorithm){
        case MA_FF: rearrange_FF(); break;
        case MA_BF: rearrange_BF(); break;
        case MA_WF: rearrange_WF(); break;
        case MA_RFF: rearrange_RFF(); break;
        }
}
/*首次适应算法，按地址的大小由小到达排序*/
void rearrange_FF(){
    free_block_type *temp,*p=NULL;
    free_block_type *head=NULL;
    int current_min_addr;

    if(free_block){
        temp=free_block;
        current_min_addr=free_block->start_addr;
        while(temp->next!=NULL){
            if(temp->next->start_addr<current_min_addr){
                current_min_addr=temp->next->start_addr;
                p=temp;
                }
            temp=temp->next;
        }
        if(p!=NULL){
            temp=p->next;
            p->next=p->next->next;
            temp->next=free_block;
            free_block=temp;
        }
        head=free_block;
        p=head;
        temp=head->next;
        while(head->next!=NULL){
            current_min_addr=head->next->start_addr;
            while(temp->next!=NULL){
                if(temp->next->start_addr<current_min_addr){
                    current_min_addr=temp->next->start_addr;
                    p=temp;
                }
            temp=temp->next;
            }
            if(p->next!=head->next){
                temp=p->next;
                p->next=p->next->next;
                temp->next=head->next;
                head->next=temp;
            }
        head=head->next;
        temp=head->next;
        p=head;
        }
    }
    return ;
}

/*最佳适应算法，按内存块的大小由小到大排序*/
void rearrange_BF(){
    free_block_type *temp,*p=NULL;
    free_block_type *head=NULL;
    int current_min_size=free_block->size;
    
    temp=free_block;
    while(temp->next!=NULL){
        if(temp->next->size<current_min_size){
            current_min_size=temp->next->size;
            p=temp;
        }
        temp=temp->next;
    }
    if(p!=NULL){
        temp=p->next;
        p->next=p->next->next;
        temp->next=free_block;
        free_block=temp;
    }
    head=free_block;
    p=head;
    temp=head->next;
    while(head->next!=NULL){
        current_min_size=head->next->size;
        while(temp->next!=NULL){
            if(temp->next->size<current_min_size){
                current_min_size=temp->next->size;
                p=temp;
            }
            temp=temp->next;
        }
    if(p->next!=head->next){
        temp=p;
        p->next=p->next->next;
        temp->next=head->next;
        head->next=temp;
        }
        head=head->next;
        temp=head->next;
        p=head;
    }

}

/*最坏适应算法，按地址块的大小从大到小排序*/
void rearrange_WF(){
       free_block_type *temp,*p=NULL;
    free_block_type *head=NULL;
    int current_max_size=free_block->size;
    temp=free_block;
    while(temp->next!=NULL){
        if(temp->next->size>current_max_size){
            current_max_size=temp->next->size;
            p=temp;
        }
        temp=temp->next;
    }
    if(p!=NULL){
        temp=p;
        p->next=p->next->next;
        temp->next=free_block;
        free_block=temp;
    }
    head=free_block;
    p=head;
    temp=head->next;
    while(head->next!=NULL){
        current_max_size=head->next->size;
        while(temp->next!=NULL){
            if(temp->next->size>current_max_size){
                current_max_size=temp->next->size;
                p=temp;
            }
        temp=temp->next;
        }
        if(p->next!=head->next){
            temp=p->next;
            p->next=p->next->next;
            temp->next=head->next;
            head->next=temp;
        }
        head=head->next;
        temp=head->next;
        p=head;
    }
    return ;
}

/*循环首次适应算法，从上次分配的位置，循环排序*/
void rearrange_RFF()
{
    /*这次作业的核心应该就是这个排序。每次排序的时候应该保证首部是上一次查询的位置就可以了*/
    /*显然（去**没注释的显然），allocated_block_head的头部就是上次分配的位置
     *所以直接查询上次分配的位置，重新排序就可以了*/
    rearrange_FF();
    if(free_block == nullptr) return; // 没有空间时不需要排序
    free_block_type *head = nullptr; // 本次排序的新首地址编号。
    /*获取首部块*/
    if(allocated_block_head == nullptr) // 没有 使用块 的时候保持头部不变
        head = free_block;
    else
    {
        int last_adr = allocated_block_head->start_addr;
        
        // 遍历 free_allock, 寻找地址大于last_adr的最小地址
        free_block_type *st;
        st = free_block;
        while(st != nullptr)
        {
            if(st->start_addr > last_adr)
            {
                if(head == nullptr) head = st;
                else if(head->start_addr > st->start_addr) head = st;
            }
            st = st->next;
        }
    }

    // 重新拼接链表
    if(head == free_block) return; // 不需要拼接
    else
    {
        free_block_type *head_pre = free_block; // 找head的前一个节点
        while(head_pre->next != nullptr)
        {
            if(head_pre->next == head) break;
            head_pre = head_pre->next;
        }
        free_block_type *tail = free_block; // 找当前的尾部节点
        while(tail->next != nullptr)
        {
            tail = tail->next;
        }
        head_pre->next = nullptr;
        tail->next = free_block;
        free_block = head;
    }

}

//创建一个新的进程
int new_process(){
    struct allocated_block *ab;
    int size; 
    int ret;
    ab=(struct allocated_block *)malloc(sizeof(struct allocated_block));
    if(!ab) exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid; 
    printf("Memory for %s:", ab->process_name);
    printf("Please input you want to allocate process' size : ");
    scanf("%d", &size);
    if(size>0) {
        
        ab->size=size;
    }
    ret = allocate_mem(ab); 
    if((ret==1) &&(allocated_block_head == NULL)){ 
        allocated_block_head=ab;
        return 1; }
   
    else if (ret==1) {
        ab->next=allocated_block_head;
        allocated_block_head=ab;
        return 2; }
    else if(ret==-1){ 
        printf("Allocation fail\n");
        pid--;
        free(ab);
        return -1; 
     }
    return 3;
    }

//内存分配
int allocate_mem(struct allocated_block *ab){
    // 这个函数应该是要返回首地址编号吧
    free_block_type *fbt, *pre;
    free_block_type *temp,*p,*p1;
    allocated_block *q;
    int request_size=ab->size; // 要分配出来的内存大小
    int sum=0;
    int max;
    fbt = pre = free_block;
    if(fbt){
        if(ma_algorithm==MA_WF){ // 对于最大匹配，表首大小不够直接pass
            if(fbt==NULL||fbt->size<request_size)
                    return -1;
        }
        else{
            // 否则直接寻找第一个合适的
            while(fbt!=NULL&&fbt->size<request_size){
                pre=fbt;        
                fbt=fbt->next;
            }
        }
        if(fbt==NULL||fbt->size<request_size){
            if(free_block->next!=NULL){
                sum=free_block->size;
                temp=free_block->next;
                while(temp!=NULL){
                    sum+=temp->size;
                    if(sum>=request_size)
                        break;
                    temp=temp->next;
                }
                if(temp==NULL)
                    return -1;
                else{
                    pre=free_block;
                    max=free_block->start_addr;
                    fbt=free_block;
                    while(temp->next!=pre){
                        if(max<pre->start_addr){
                            max=pre->start_addr;
                            fbt=pre;
                        }
                        pre=pre->next;
                    }
                    pre=free_block;
            
                    while(pre!=temp->next)
                    {
                        q=allocated_block_head;
                        p=free_block;
                    
                            while(q!=NULL)
                            {
                                if(q->start_addr>pre->start_addr)
                                    q->start_addr=q->start_addr-pre->size;
                                q=q->next;
                            }
                            while(p!=NULL)
                            {
                                if(p->start_addr>pre->start_addr)
                                    p->start_addr=p->start_addr-pre->size;
                                p=p->next;
                                
                            }
                    
                        pre=pre->next;
                    }
                
                pre=free_block;
                while(pre!=temp->next){
            
                    p1=pre->next;
                    if(pre==fbt)
                        break;
                    free(pre);
                    pre=p1;
                
                }
                q=allocated_block_head;
                free_block=fbt;
                free_block->start_addr=q->start_addr+q->size;
            
                free_block->size=sum;
                free_block->next=temp->next;
                if(free_block->size-request_size<MIN_SLICE){
                    ab->size=free_block->size;
                    ab->start_addr=free_block->start_addr;
                    pre=free_block;
                    free_block=free_block->next;
                    free(pre);
                }
                else{
                    ab->start_addr=free_block->start_addr;
                    free_block->start_addr=free_block->start_addr+request_size;
                    free_block->size=free_block->size-request_size;
                }
                
            }
        }
        else
            return -1;
         }
        else{
            if(fbt->size-request_size<MIN_SLICE){
                ab->size=fbt->size;
                ab->start_addr=fbt->start_addr;
                if(pre->next==free_block){
                    free_block=fbt->next;
                }
                else{
                    pre->next=fbt->next;
                }
                free_block=fbt->next;
                free(fbt);
            }
            else{
                ab->start_addr=fbt->start_addr;
                fbt->start_addr=fbt->start_addr+request_size;
                fbt->size=fbt->size-request_size;
            }
        }
        rearrange(ma_algorithm);
        return 1;
    }
    else
    {
        printf("Free Memory already has been allocated over: ");
        return -1;
    }

}

//选择杀死一个进程

void kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab=find_process(pid);
    if(ab!=NULL){
        free_mem(ab); 
        dispose(ab); 
        }
}
//找到要杀死的进程的标号
allocated_block * find_process(int pid){
    allocated_block *abb;
    abb=allocated_block_head;
    if(abb->pid==pid)
    {
        return abb;
    }
    abb=allocated_block_head->next;
    while(abb->next!=NULL){
        if(abb->pid==pid)
            return abb;
        abb=abb->next;
    }
    return abb;
}

//释放杀死进程的内存块
int free_mem(struct allocated_block *ab){
       int algorithm = ma_algorithm;
       struct free_block_type *fbt, *pre;
       fbt=(struct free_block_type*) malloc(sizeof(struct free_block_type));
    pre=(struct free_block_type*) malloc(sizeof(struct free_block_type));
       if(!fbt) return -1;

    fbt->start_addr=ab->start_addr;
    fbt->size=ab->size;
    fbt->next=free_block;
    free_block=fbt;
    rearrange_FF();
    pre->next=free_block;
    pre->size=0;
    while(pre->next&&(pre->next->start_addr!=fbt->start_addr))
        pre=pre->next;
    if(pre->size!=0&&fbt->next!=NULL){
        if(((pre->start_addr+pre->size)==fbt->start_addr)&&((fbt->start_addr+fbt->size)==fbt->next->start_addr)){
            pre->size=pre->size+fbt->size+fbt->next->size;
            pre->next=fbt->next->next;
            free(fbt->next);
            free(fbt);
        }
        else if((pre->start_addr+pre->size)==fbt->start_addr){
            pre->size=pre->size+fbt->size;
            pre->next=fbt->next;
            free(fbt);
        }
        else if(fbt->start_addr+fbt->size==fbt->next->start_addr){
            fbt->size=fbt->size+fbt->next->size;
            fbt->next=fbt->next->next;
            free(fbt->next);
        }
    }
    else if((pre->size==0)&&fbt->next){
        if((fbt->start_addr+fbt->size)==fbt->next->start_addr){
            fbt->size=fbt->size+fbt->next->size;
            fbt->next=fbt->next->next;
            free_block=fbt;
            free(fbt->next);
        }
    }
    else if(fbt->next==NULL){
        if((pre->start_addr+pre->size)==fbt->start_addr){
            pre->size=pre->size+fbt->size;
            pre->next=fbt->next;
            free(fbt);
        }
    }
    rearrange(algorithm);
     
    return 1;
}

//销毁杀死进程的结点
int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre,*ab;

   if(free_ab == allocated_block_head) { 
     allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
        }
    pre = allocated_block_head; 
    ab = allocated_block_head->next;
    while(ab!=free_ab)
    {
     pre = ab;
     ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
 }

//显示内存使用情况

int display_mem_usage(){
    struct free_block_type *fbt=free_block;
    struct allocated_block *ab=allocated_block_head;
    printf("----------------------------------------------------------\n");

    if(fbt==NULL) 
    {
        printf("Free Memory already used over !\n");
    }
    printf("----------------------------------------------------------\n");


    if(fbt)
    {
        printf("Free Memory:\n");
        printf("%20s %20s\n", " start_addr", " size");
        while(fbt!=NULL){
            printf("%20d %20d\n", fbt->start_addr, fbt->size);
            fbt=fbt->next;
            } 
    }
    
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while(ab!=NULL){
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab=ab->next;
        }
    printf("----------------------------------------------------------\n");
    return 0;
}

//退出，销毁所有链表
void do_exit()
{
    free_block_type *temp;
    allocated_block *temp1;
    temp=free_block->next;
    while(temp!=NULL)
    {
        free_block->next=temp->next;
        free(temp);
        temp=free_block->next;
    }
    free(free_block);
    temp1=allocated_block_head->next;
    while(temp1!=NULL)
    {
        allocated_block_head->next=temp1->next;
        free(temp1);
        temp1=allocated_block_head->next;
    }
    free(allocated_block_head->next);

}
//主函数
int main(){
    char choice; 
    pid=0;
    free_block=init_free_block(mem_size); 
    while(1) {
    display_menu();    
    fflush(stdin);

   choice=getchar();    
    switch(choice){
        case '1': set_mem_size(); break;     
        case '2': set_algorithm();flag=1; break;
        case '3': new_process(); flag=1; break;
        case '4': kill_process(); flag=1; break;
        case '5': display_mem_usage(); flag=1; break;    
        case '0': do_exit();exit(0);    
        default: break; 
    } 
    } 
    return 0;
}
