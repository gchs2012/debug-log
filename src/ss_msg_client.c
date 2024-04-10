/******************************************************************************

            版权所有 (C), 2020-2021, xxx Co.xxx, Ltd.

 ******************************************************************************
    文 件 名 : ss_msg_client.cpp
    版 本 号 : V1.0
    作    者 : zc
    生成日期 : 2021年1月25日
    功能描述 : 消息发送客户端
    修改历史 :
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ss_msg.h"
#include "ss_event.h"

/*****************************************************************************
    函 数 名 : ss_usage
    功能描述 : 打印帮助信息
    输入参数 : 无
    输出参数 : 无
    返 回 值 : 无
    作    者 : zc
    日    期 : 2021年1月25日
*****************************************************************************/
static void ss_usage() {
    printf("Usage:\n");
    printf("  ss_msg_client [-n <0~7>] [-t <type>]\n\n");
    printf("  eg: ss_msg_client -n 0 -t 1\n\n");
    printf("---------------------------------------------\n");
    printf("  [type]       [info]\n");
    for (int i = SS_MSG_TYPE_MIN; i < SS_MSG_TYPE_MAX; i++) {
        const char *msg = SS_GET_MSG_INFO(i);
        if (msg) {
            if (i >= 10) {
                printf("   %d           %s\n", i, msg);
            } else {
                printf("   %d            %s\n", i, msg);
            }
        }
    }
    printf("---------------------------------------------\n\n");
    exit(1);
}

#define SS_CHECK_SOCKETID_TYPE(_count, _sock, _type) do { \
    if (_count != 2) { ss_usage(); } \
    if (_sock < 0 || _sock > 7) { \
        printf("Invalid numa id(%d)\n", _sock); exit(1); } \
    if (_type < 0 || _type >= SS_MSG_TYPE_MAX) { \
        printf("Invalid msg type(%d)\n", _type); exit(1); } \
} while (0)

/*****************************************************************************
    函 数 名 : ss_socket
    功能描述 : 创建socket
    输入参数 : int numa_id
    输出参数 : 无
    返 回 值 : int
    作    者 : zc
    日    期 : 2021年1月25日
*****************************************************************************/
static int ss_socket(int numa_id)
{
    int fd = -1;
    struct sockaddr_un sa;
    struct timeval timeout = {3, 0};

    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    snprintf(sa.sun_path, sizeof(sa.sun_path), SS_UNIX_SOCKET, numa_id);
    if (access(sa.sun_path, F_OK) < 0) {
        printf("Unix socket file(%s) not exist\n", sa.sun_path);
        goto err;
    }

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("Create socket error: %s\n", strerror(errno));
        goto err;
    }

    /* 设置接收超时时间 */
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        printf("Setsockopt SO_RCVTIMEO error: %s\n", strerror(errno));
        goto err;
    }

    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        printf("Connect socket error: %s\n", strerror(errno));
        goto err;
    }

    return fd;

err:
    if (fd >= 0) close(fd);
    exit(1);
}

/*****************************************************************************
    函 数 名 : ss_send
    功能描述 : 发送消息
    输入参数 : int fd
               int type
    输出参数 : 无
    返 回 值 : 无
    作    者 : zc
    日    期 : 2021年1月25日
*****************************************************************************/
static void ss_send(int fd, int type) {
    int len;
    ss_msg *msg;
    char buf[1024] = {0};

    msg = (ss_msg *)buf;
    msg->type = type;

    len = send(fd, buf, sizeof(ss_msg), 0);
    if (len <= 0) {
        printf("Send data error: %s\n", strerror(errno));
        close(fd);
        exit(1);
    }

    return;    
}

/*****************************************************************************
    函 数 名 : ss_recv
    功能描述 : 接收消息
    输入参数 : int fd
    输出参数 : 无
    返 回 值 : 无
    作    者 : zc
    日    期 : 2021年1月25日
*****************************************************************************/
static void ss_recv(int fd) {
    int len;
    ss_msg *msg;
    char buf[1024] = {0};

    len = recv(fd, buf, sizeof(buf), 0);
    if (len > 0) {
        msg = (ss_msg *)buf;
        printf("Resp: type = %d, result = %d\n", msg->type, msg->result);
    } else if (len < 0 && errno == EAGAIN) {
        printf("Resp: recv timeout\n");
    }

    close(fd);
}

/*****************************************************************************
    函 数 名 : main
    功能描述 : 主函数
    输入参数 : int argc
               char *argv[]
    输出参数 : 无
    返 回 值 : int
    作    者 : zc
    日    期 : 2021年1月25日
*****************************************************************************/
int main(int argc, char *argv[]) {
    int opt = -1;
    int type = -1;
    int numa_id = -1;
    int count_arg = 0;

    while ((opt = getopt(argc, argv, "n:t:h")) != -1) {
        switch (opt) {
        case 'n':
            count_arg++;
            numa_id = atoi(optarg);
            break;
        case 't':
            count_arg++;
            type = atoi(optarg);
            break;
        default:
            ss_usage();
        }
    }

    /* 检查SOCKETID  和类型 */
    SS_CHECK_SOCKETID_TYPE(count_arg, numa_id, type);

    int fd = ss_socket(numa_id);
    ss_send(fd, type);
    ss_recv(fd);

    return 0;
}

