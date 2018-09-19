/******************************************************************************

                  Copyright (C), 2017-2018, xxx Co.xxx, Ltd.

 ******************************************************************************
    File Name     : dg_log.h
    Version       : V1.0
    Author        : zc
    Created       : 2018/2/2
    Description   : debug log
                    1. DG_INIT      --- Init
                    2. DG_SET_MODE  --- Set the log mode
                    3. DG_SET_LEN   --- Set the log file size
                    4. DG_LOG       --- Print log
    History       :
******************************************************************************/
#ifndef _SP_DG_LOG_H_
#define _SP_DG_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* log mode */
enum {
    DG_LOG_MODE_STDOUT   = 1, // output to screen
    DG_LOG_MODE_FILE     = 2, // output to file
    DG_LOG_MODE_DISABLE  = 3, // close log
    DG_LOG_MODE_MAX,
};

/********************************************************************/
// The following is the internal use interface
int dg_log_init(const char *name, const char *file);
void dg_log_set_mode(int num, int mode);
int dg_log_get_mode(int num);
void dg_log_set_size(int num, int size);
void dg_log_print(int num, const char *function,
    unsigned int line, const char *format, ...);
/********************************************************************/


#define DG_TOSTRING(s)		#s


/********************************************************************/
// eg:
//	    ......
//      int log_num;
//      log_num = DG_LOG_INIT(test_log_mod, "./test.log");
//      ......
//      DG_LOG(log_num, "Test debug_log_mod!\n");
//      DG_LOG(log_num, "Test debug_log_mod id = %d\n", log_num);
//      ......

/* init */
#define DG_LOG_INIT(_log_mod, _log_file) ({ \
    int _log_num = dg_log_init(DG_TOSTRING(_log_mod), (_log_file)); \
    (_log_num); \
})

/* set log mode */
#define DG_SET_MODE(_log_num, _log_mode) do { \
    dg_log_set_mode((_log_num), (_log_mode)); \
} while (0)

/* set log file size */
#define DG_SET_SIZE(_log_num, _log_size) do { \
    dg_log_set_size((_log_num), (_log_size)); \
} while (0)

/* get log mode */
#define DG_GET_MODE(_log_num) ({ \
    int _mode = dg_log_get_mode((_log_num)); \
    (_mode); \
})

/* print log */
#define DG_LOG(_log_num, ...) do { \
    if (dg_log_get_mode((_log_num)) != DG_LOG_MODE_DISABLE) \
        dg_log_print((_log_num), __FUNCTION__, __LINE__,  __VA_ARGS__); \
} while (0)
/********************************************************************/

#ifdef __cplusplus
}
#endif

#endif

