//##**2022.06.22 上海BOM对旧有功能进行测试中出现的问题进行修改：**##//

1.84字节二维码、128字节二维码，0x32指令中手机返回非0应答代码，读写器应报CE_WRITEERROR(12)，读写器报CE_INVALIDCARD(14)，
修改：
qr.c-qr_trans_exit, line 467. 
qr.c-qr_trans_entry, line 700. 
qr.c-qr_trans_update, line 885.

2.读单程票出站售票记录，返回数据顺序应为售票记录在前，出站记录在后。
修改：
shmt_bom_func.c-ShMetroRecord, line 5412-5415.

3.84字节二维码、128字节二维码，0x34指令中手机返回应答代码，读卡器检查应答代码。
修改：
qr.c-qr_trans_update, line 14.

4.备份：将BOM读卡器读卡记录更新为地铁同步更新之前备份，备份：zdd-2022.06.28

5.更新BOM读卡器读记录至地铁读卡器同步
修改：
shmt_bom_func.c-ShCardReadRecord.

6.增加单程票加值、续期、退票功能及其底层读写函数实现
修改：
新增shmt_bom_func.h-line 62
新增shmt_bom_func.h-line 230-233

7.shtrans.c中trans_a中gMetroIoFlagBkValid = 1；改为0






更新的时候做双向认证还是寻卡的时候？
128 与 84当日使用次数清0是否一样？写回不写回
双向认证与取卡信息中应用区MAC计算不同？
读交通部本地卡0007文件历史交易记录命令各个。