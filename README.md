# test libdglog.so

// eg:
//	    ......
//      int log_num;
//      log_num = DG_LOG_INIT(test_log_mod, "./test.log");
//      ......
//      DG_LOG(log_num, "Test debug_log_mod!\n");
//      DG_LOG(log_num, "Test debug_log_mod id = %d\n", log_num);
//      ......

[CODE]
int LOG_NUM = 0;

int main(int argc, char *argv[])
{
    LOG_NUM = DG_LOG_INIT(test, "test.log");
    DG_SET_MODE(LOG_NUM, DG_LOG_MODE_FILE);
    DG_SET_SIZE(LOG_NUM, 50);

    DG_LOG(LOG_NUM, "test code1!\n");
    DG_LOG(LOG_NUM, "test code2, num = %d\n", LOG_NUM);
    DG_LOG(LOG_NUM, "test code3, str = %s\n", "test code");
    
    return 0;
}

[BUILD]gcc test.c -o test -ldglog
