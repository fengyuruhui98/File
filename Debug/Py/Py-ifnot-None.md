```py
var = None
 
def fun_not_var(var_data):
    if not var_data:
        print('哈哈哈哈')
    else:
        print('嘿嘿嘿')
fun_not_var(var)
 
# 哈哈哈哈
 
 
def fun_var(var_data):
    if var_data:
        print('哈哈哈哈')
    else:
        print('嘿嘿嘿')
fun_var(var)
 
# 嘿嘿嘿
 
 
在python 判断语句中 None,  False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()都相当于 False 
 
not None == not False == not '' == not 0 == not [] == not {} == not ()
```


# ***None不是false***

```py
>>>None == 0
False
>>>None == ' ' 
False
>>>None == False
False
>>>None == None
True
```