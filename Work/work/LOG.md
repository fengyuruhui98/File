# shanghai AGM 3018 reader 

## 2023-01-06.M1卡读第八块未经过认证，读失败，重新读

修改代码：
```c
    //shtrans.c line:1310
	//读B8
	gDebugStep=41;
	if(mcmh_read(8,buf,2,0) != 0) //此处传参修改为：mcmh_read(8,buf,2,0) != 0
	{
		goto label_err_read;
	}
```