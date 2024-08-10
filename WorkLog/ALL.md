################2024-08-05-ZDD################
1.宁波奥体中心工控一体机重启系统

################2024-08-06-ZDD################
1.首南测试环境LC服务器及后台应用程序
2.滁州前端修改
3.徐州票务系统测试环境：10.12.220.35 root/111111 路径：/opt/web/www/

################2024-08-07-ZDD################
1.首南导入正线数据，命令为：
'''imp ccdb/ccdb file=table.dmp log=dbuser.log tables=TBL_3001_RUNMODE,T_UPS_STATUS fromuser=ccdb touser=ccdb buffer=4096000 commit=y ignore=y feedback=10000;'''
导出命令为：
'''exp ccdb/ccdb file=table.dmp log=dbuser.log tables=TBL_3001_RUNMODE,T_UPS_STATUS buffer=4096000 feedback=10000;'''
2.首南测试环境：数据库：10.27.96.75 root/Dp111111 oracle/afcapp123!@# 后台程序：10.27.96.76
3.济南一号线SC重启报“寄存器数据入库错误” 由于累积寄存器错误包在datafile中prepareinsert文件夹下累积超过30个，清除解决。
4.大连2号线数据错误及五号线交易数据重传。

################2024-08-08-ZDD################
1.翠柏里重做系统，烧录loader与rootfs和基本参数。
2.滁州问题测试，前端统计事件遗留。

################2024-08-09-ZDD################
1.奥体中心保障。
2.首南车辆段配合网新测试网新修改的SC软件版本，关于修改软件参数查询异步同步使失败问题，网新修改为本地存储，立即应答。