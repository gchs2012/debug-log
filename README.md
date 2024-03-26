# debug log

## [CODE]<br>

```
#include <stdio.h>
#include "dg_log.h"
#include "ss_msg.h"

static int AAA_LOG = -1;
static volatile int AAA_LOG_F = 0;

static int BBB_LOG = -1;
static volatile int BBB_LOG_F = 0;

static int test_msg_deal_func(int type, int len, char *data, ss_msg *resp)
{
    switch (type) {
    case SS_AAA_LOG_ENABLE_TYPE:
        DG_LOG(AA_LOG, SS_AAA_LOG_ENABLE_INFO"\n");
        AA_LOG_F = DG_LOG_FLAG|AA_LOG;
        break;
    case SS_AAA_LOG_DISABLE_TYPE:
        DG_LOG(AA_LOG, SS_AAA_LOG_DISABLE_INFO"\n");
        AA_LOG_F = 0;
        break;
    case SS_BBB_LOG_ENABLE_TYPE:
        DG_LOG(BB_LOG, SS_BBB_LOG_ENABLE_INFO"\n");
        BB_LOG_F = DG_LOG_FLAG|BB_LOG;
        break;
    case SS_BBB_LOG_DISABLE_TYPE:
        DG_LOG(BB_LOG, SS_BBB_LOG_DISABLE_INFO"\n");
        BB_LOG_F = 0;
        break;
    default:
        break;
    }

    return 1;
}

int main(int argc, char *argv[]) 
{
    pthread_t stThread;

    // 初始化
    if (ss_msg_init(0) == 0)
    {
        pthread_create(&stThread, NULL, ss_msg_thread, NULL);

        AA_LOG = DG_LOG_INIT("aaa.log");
        BB_LOG = DG_LOG_INIT("bbb.log");

        ss_msg_register(test_msg_deal_func, SS_AAA_LOG_ENABLE_TYPE);
        ss_msg_register(test_msg_deal_func, SS_AAA_LOG_DISABLE_TYPE);
        ss_msg_register(test_msg_deal_func, SS_BBB_LOG_ENABLE_TYPE);
        ss_msg_register(test_msg_deal_func, SS_BBB_LOG_DISABLE_TYPE);
    }

    while (1)
    {
        // 测试日志记录
        DG_LOG(AA_LOG, "test AAA!");
        DG_LOG(AA_LOG, "test AAA, num = %d", 10);
        DG_LOG_F(AA_LOG_F, "test AAA!");
        DG_LOG_F(AA_LOG_F, "test AAA, num = %d", 10);

        DG_LOG(BB_LOG, "test bbb!");
        DG_LOG(BB_LOG, "test bbb, num = %d", 10);
        DG_LOG_F(BB_LOG_F, "test bbb!");
        DG_LOG_F(BB_LOG_F, "test bbb, num = %d", 10);

        sleep(10);
    }

    return 0;
}
```
