# debug log

## [CODE]<br>

```
int LOG_NUM = 0;

int main(int argc, char *argv[]) 
{
    LOG_NUM = DG_LOG_INIT(test, "test.log");

    DG_SET_MODE(LOG_NUM, DG_LOG_MODE_FILE);
    DG_SET_SIZE(LOG_NUM, 50);  //log file maximum: 50MB

    DG_LOG(LOG_NUM, "test code1!\n");
    DG_LOG(LOG_NUM, "test code2, num = %d\n", LOG_NUM);
    DG_LOG(LOG_NUM, "test code3, str = %s\n", "test code");

    return 0;
}
```

## [BUILD]<br>
```
gcc test.c -o test -ldglog
```
