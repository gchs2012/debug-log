/******************************************************************************

            版权所有 (C), 2017-2018, xxx Co.xxx, Ltd.

 ******************************************************************************
    文 件 名 : ss_common.h
    版 本 号 : V1.0
    作    者 : zc
    生成日期 : 2018年8月2日
    功能描述 : 消息类型定义公共头文件，消息类型及消息信息需自行添加
    修改历史 :
******************************************************************************/
#ifndef _SS_COMMON_H_
#define _SS_COMMON_H_

/// 消息类型
enum {
    SS_MSG_TYPE_MIN = 0,

    // <-------------- 新增消息类型 -------------->
#define SS_AAA_LOG_ENABLE_INFO                          "Enable aaa log"
    SS_AAA_LOG_ENABLE_TYPE,

#define SS_AAA_LOG_DISABLE_INFO                         "Disable aaa log"
    SS_AAA_LOG_DISABLE_TYPE,

#define SS_BBB_LOG_ENABLE_INFO                          "Enable bbb log"
    SS_BBB_LOG_ENABLE_TYPE,

#define SS_BBB_LOG_DISABLE_INFO                         "Disable bbb log"
    SS_BBB_LOG_DISABLE_TYPE,

    // <------------------------------------------->

#define SS_RESP_RESULT_INFO                            "Response message"
    SS_RESP_RESULT_TYPE,

    SS_MSG_TYPE_MAX,
};

/// 通过类型获取信息
#define SS_GET_MSG_INFO(_type_) ({ \
    const char *_msg_ = NULL; \
    switch (_type_) { \
        case SS_AAA_LOG_ENABLE_TYPE:                (_msg_) = SS_AAA_LOG_ENABLE_INFO; break; \
        case SS_AAA_LOG_DISABLE_TYPE:               (_msg_) = SS_AAA_LOG_DISABLE_INFO; break; \
        case SS_BBB_LOG_ENABLE_TYPE:                (_msg_) = SS_BBB_LOG_ENABLE_INFO; break; \
        case SS_BBB_LOG_DISABLE_TYPE:               (_msg_) = SS_BBB_LOG_DISABLE_INFO; break; \
        case SS_RESP_RESULT_TYPE:                  (_msg_) = SS_RESP_RESULT_INFO; break; \
    } \
    _msg_; })

#endif
