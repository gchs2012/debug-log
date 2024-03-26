/******************************************************************************

            版权所有 (C), 2017-2018, xxx Co.xxx, Ltd.

 ******************************************************************************
    文 件 名 : ss_msg.c
    版 本 号 : V1.0
    作    者 : zc
    生成日期 : 2018年8月2日
    功能描述 : 消息处理
    修改历史 :
******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "dg_log.h"
#include "ss_msg.h"

extern void ss_process_events(void);
extern int ss_init_server(int num, int log_num);

/* 事件注册函数 */
static ss_msg_pf ss_msg_func_arr[SS_MSG_TYPE_MAX] = { NULL };

static int MSG_LOG = 0;

#define SS_MSG_LOG_FILE     "/var/log/ss_msg_%d.log"
#define SS_MSG_JSON_FILE    "/var/log/ss_msg_type.json"

/*****************************************************************************
    函 数 名 : ss_msg_deal
    功能描述 : 处理消息
    输入参数 : ss_msg *req
    输出参数 : ss_msg *resp
    返 回 值 : 无
    作    者 : zc
    日    期 : 2018年8月2日
*****************************************************************************/
void ss_msg_deal(ss_msg *req, ss_msg *resp)
{
    int ret = -1;

    if ((req->type >= SS_MSG_TYPE_MIN) && (req->type < SS_MSG_TYPE_MAX)) {
        if (ss_msg_func_arr[req->type] != NULL) {
            ret = ss_msg_func_arr[req->type](
                req->type,
                req->len,
                req->data,
                resp);
        }
    }
    resp->type = SS_RESP_RESULT_TYPE;
    resp->result = ret;
}

/*****************************************************************************
    函 数 名 : ss_msg_thread
    功能描述 : 消息处理线程
    输入参数 : void *arg
    输出参数 : 无
    返 回 值 : void *
    作    者 : zc
    日    期 : 2018年8月2日
*****************************************************************************/
void *ss_msg_thread(void *arg)
{
    SS_UNUSED(arg);

    while (1) {
        ss_process_events();
    }

    return NULL;
}

/*****************************************************************************
    函 数 名 : ss_msg_register
    功能描述 : 消息注册
    输入参数 : ss_msg_pf func
               int type
    输出参数 : 无
    返 回 值 : 无
    作    者 : zc
    日    期 : 2018年8月2日
*****************************************************************************/
void ss_msg_register(ss_msg_pf func, int type)
{
    if ((type < SS_MSG_TYPE_MIN) || (type >= SS_MSG_TYPE_MAX)) {
        DG_LOG(MSG_LOG, "msg type(%d) error.\n", type);
        return;
    }

    if (func != NULL) {
        ss_msg_func_arr[type] = func;
    }

    return;
}

/*****************************************************************************
    函 数 名 : ss_msg_type2json
    功能描述 : 消息类型转换成JSON文件
    输入参数 : void
    输出参数 : 无
    返 回 值 : 无
    作    者 : zc
    日    期 : 2018年8月2日
*****************************************************************************/
void ss_msg_type2json(void)
{
    int i;
    FILE *fp;
    char buf[1024];

    if ((fp = fopen(SS_MSG_JSON_FILE, "w")) != NULL) {
        fwrite("{\n", 2, 1, fp);
    } else {
        DG_LOG(MSG_LOG, "fopen file(%s) failed\n", SS_MSG_JSON_FILE);
        return;
    }

    for (i = SS_MSG_TYPE_MIN; i < SS_MSG_TYPE_MAX; i++) {
        const char *msg = SS_GET_MSG_INFO(i);
        if (msg) {
            if (i == SS_RESP_RESULT_TYPE) {
                snprintf(buf, sizeof(buf), "    \"%d\": \"%s\"\n", i, msg);
            } else {
                snprintf(buf, sizeof(buf), "    \"%d\": \"%s\",\n", i, msg);
            }
            fwrite(buf, strlen(buf), 1, fp);
        }
    }

    fwrite("}\n", 2, 1, fp);
    fclose(fp);
}

/*****************************************************************************
    函 数 名 : ss_msg_init
    功能描述 : 消息初始化
    输入参数 : int num
    输出参数 : 无
    返 回 值 : int
    作    者 : zc
    日    期 : 2018年8月2日
*****************************************************************************/
int ss_msg_init(int num)
{
    char file[1024];

    snprintf(file, sizeof(file), SS_MSG_LOG_FILE, num);
    MSG_LOG = DG_LOG_INIT(file);
    ss_msg_type2json();

    return ss_init_server(num, MSG_LOG);
}

