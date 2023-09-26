import os
import time

#os.walk方法获取当前路径下的root（所有路径）、dirs（所有子文件夹）、files（所有文件）

path = r"I:\藏乐\MuStore\增量更新"

def file_name():
    F = []
    for root, dirs, files in os.walk(path):
        #print root
        #print dirs
        for file in files:
            #print file.decode('gbk')    #文件名中有中文字符时转码
            if os.path.splitext(file)[1] == '.vpr':
                t = os.path.splitext(file)[0]
                print(t)   #打印所有py格式的文件名
                F.append(t) #将所有的文件名添加到L列表中
    return F   # 返回L列表

F = file_name()
cmd = "start /d " + "\"" + path + "\"" + " um-windows-amd64.exe " + "\"" + path + "\\"
print(cmd)
for name in F:
    cmdf = cmd + name + ".vpr" + "\""
    print(cmdf)
    os.system(cmdf)
    time.sleep(5)



