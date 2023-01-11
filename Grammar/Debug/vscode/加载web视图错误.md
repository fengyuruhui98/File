# vscode加载web 视图，报错:“Error: Could not register serviceworkers: InvalidstateError: Failed to regist“
1、关闭vscode

2、按WIN + R，输入cmd，打开终端，然后输入命令

code --no-sandbox

3、会重启vscode，就可以正常使用了。