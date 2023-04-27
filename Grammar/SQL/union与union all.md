研究
先说结论，因为UNION 操作会对结果去重且排序，所以从速度来说， UNION ALL会更胜一筹。



通过性能分析可得出，union比union all 多做了操作。由此得结论：

union去重并排序，union all直接返回合并的结果，不去重也不排序；
union all比union性能好；
