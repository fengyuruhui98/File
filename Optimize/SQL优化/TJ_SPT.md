帮我优化SQL语句：SELECT 
t3.sle_id DEVICE_NID,t3.trans_time_new TRANS_TIME,t3.trans_type,
t4.phone PHONE_NUMBER,substr(t4.name ,1,1)||'*' USER_NAME,substr(t4.hid,1,4)||'**********'||substr(t4.hid,15,4) ID_NUMBER,
s.cn_name STATION_NAME
FROM
(
	SELECT 
	t2.* 
	FROM(
	    select 
	    t.station_id,t.sle_id ,t.trans_type,t.trans_time,t.trans_time_new , t.account_no 
	    from t_9101_qr_temp t
	    where 
	    t.trans_time >= TO_CHAR(TO_DATE ('2023-08-29 00:00:00', 'YYYY-MM-DD HH24:MI:SS'),'YYYYMMDDHH24MISS')
	    AND t.trans_time <= TO_CHAR(TO_DATE ('2023-08-30 00:00:00', 'YYYY-MM-DD HH24:MI:SS'),'YYYYMMDDHH24MISS')
	) t2
	WHERE t2.sle_id IN ('010B0614') 
) t3
LEFT JOIN ( 
    select 
    to_number(sptapi.SM4_DECODE(u.phone_c,'930AE2901D240ACE9A3046B525665FB5')) phone,'0'||u.user_id user_id, sptapi.SM4_DECODE(u.hid_c,'930AE2901D240ACE9A3046B525665FB5') hid,
    sptapi.SM4_DECODE(u.name_c,'930AE2901D240ACE9A3046B525665FB5') name from sptapi.tj_spt_user u
	union all
	select 
	to_number(sptapi.SM4_DECODE(su.merchant_user_phone_c,'930AE2901D240ACE9A3046B525665FB5')) phone ,'0'||su.user_id user_id,
	sptapi.SM4_DECODE(su.MERCHANT_USER_ID_NUMBER_C,'930AE2901D240ACE9A3046B525665FB5') hid,sptapi.SM4_DECODE(MERCHANT_USER_REAL_NAME_C,'930AE2901D240ACE9A3046B525665FB5') name
	from sptapi.tj_sdk_user su
) t4
ON   t3.account_no = t4.user_id
LEFT JOIN t_station_code s
ON t3.station_id = s.station_nid
order by t3.trans_time_new 



以下是我对SQL语句的优化建议：

1. 考虑在子查询中使用内连接代替左连接。根据您的要求，如果有可能，子查询中使用内连接可以更好地匹配数据，提高查询性能。

2. 注意索引的使用。确保表 t_9101_qr_temp 的 trans_time 列上有索引，以支持快速地按时间范围过滤数据。

3. 避免使用函数对列进行计算。在 SELECT 子句中，您使用了 substr 函数，这可能会导致性能下降。如果可能的话，最好将这些计算移到数据导入或更新阶段，以避免在查询时进行计算。

根据上述建议，优化后的 SQL 语句如下：

```sql
SELECT 
    t3.sle_id AS DEVICE_NID,
    t3.trans_time_new AS TRANS_TIME,
    t3.trans_type,
    t4.phone AS PHONE_NUMBER,
    substr(t4.name, 1, 1) || '*' AS USER_NAME,
    substr(t4.hid, 1, 4) || '**********' || substr(t4.hid, 15, 4) AS ID_NUMBER,
    s.cn_name AS STATION_NAME
FROM
(
    SELECT 
        t2.station_id,
        t2.sle_id,
        t2.trans_type,
        t2.trans_time,
        t2.trans_time_new,
        t2.account_no 
    FROM
    (
        SELECT 
            t.station_id,
            t.sle_id,
            t.trans_type,
            t.trans_time,
            t.trans_time_new,
            t.account_no 
        FROM t_9101_qr_temp t
        WHERE 
            t.trans_time >= TO_CHAR(TO_DATE('2023-08-29 00:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'YYYYMMDDHH24MISS')
            AND t.trans_time <= TO_CHAR(TO_DATE('2023-08-30 00:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'YYYYMMDDHH24MISS')
        INNER JOIN t2 ON t2.sle_id = t.sle_id
    ) t2
) t3
INNER JOIN
(
    SELECT 
        to_number(sptapi.SM4_DECODE(u.phone_c, '930AE2901D240ACE9A3046B525665FB5')) AS phone,
        '0' || u.user_id AS user_id,
        sptapi.SM4_DECODE(u.hid_c, '930AE2901D240ACE9A3046B525665FB5') AS hid,
        sptapi.SM4_DECODE(u.name_c, '930AE2901D240ACE9A3046B525665FB5') AS name
    FROM sptapi.tj_spt_user u
    UNION ALL
    SELECT 
        to_number(sptapi.SM4_DECODE(su.merchant_user_phone_c, '930AE2901D240ACE9A3046B525665FB5')) AS phone,
        '0' || su.user_id AS user_id,
        sptapi.SM4_DECODE(su.MERCHANT_USER_ID_NUMBER_C, '930AE2901D240ACE9A3046B525665FB5') AS hid,
        sptapi.SM4_DECODE(MERCHANT_USER_REAL_NAME_C, '930AE2901D240ACE9A3046B525665FB5') AS name
    FROM sptapi.tj_sdk_user su
) t4 ON t3.account_no = t4.user_id
LEFT JOIN t_station_code s ON t3.station_id = s.station_nid
ORDER BY t3.trans_time_new;
```

请注意，这只是提供了一些基本的优化建议，并不一定适用于所有情况。建议在执行任何修改之前进行性能测试，并确保结果符合您的预期。