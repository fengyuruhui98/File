SQL中只要用到聚合函数就一定要用到group by 吗?

今天记录一个弱智问题，一直没发现这个问题。

答：看情况

1、当聚集函数和非聚集函数出现在一起时，需要将非聚集函数进行group by
2、当只做聚集函数查询时候，就不需要进行分组了。

举例来说，
SELECT SUM(TABLE.A )  FROM  TABLE

上述SQL不需要使用Group by 进行分组，因为其中没有非聚合字段，所以不用Group by 也可以。

如果是SELECT SUM(TABLE.A ),MAX(B), FROM  TABLE GROUP BY B

由于B是非聚合字段，则需要使用MAX()或者其他聚合函数并且Group by 才可以正常执行。
