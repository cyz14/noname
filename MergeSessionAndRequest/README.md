# MergeSessionAndRequest  
___
session 日志格式如下:    

    {"SessionId":10415320112011191685,"SesStartTime":1454095454993,"SesAllTime":30007,"ClientIP":"119.41.208.195","BfeIP":"0.0.0.0","KeepAliveNum":1,"InitialCwnd":10,"RetransRate":50,"Mss":1440,"Rtt":1,"Rttvar":200,"RTO":402,"ClientInitRwnd":45,"ClientMaxRwnd":45,"TransmitSize":516,"LatencyInfo"：[{"TcpLatency":245,"TransDataSize":516,"TimeOut":1,"AvgCwnd":0,"FinalCwnd":10}]}

缩进后是：  
    
    {  
        "SessionId":10415320112011191685,
        "SesStartTime":1454095454993,
        "SesAllTime":30007,
        "ClientIP":"119.41.208.195",
        "BfeIP":"0.0.0.0",
        "KeepAliveNum":1,
        "InitialCwnd":10,
        "RetransRate":50,
        "Mss":1440,
        "Rtt":1,
        "Rttvar":200,
        "RTO":402,
        "ClientInitRwnd":45,
        "ClientMaxRwnd":45,
        "TransmitSize":516,
        "LatencyInfo":[{
            "TcpLatency":245,
            "TransDataSize":516,
            "TimeOut":1,
            "AvgCwnd":0,
            "FinalCwnd":10
            }]  
    }  

request 日志格式如下:  

    {"Timestamp":1454095485,"SessionId":9357898644039614699,"ProductName":"tieba","ClientIP":"222.185.90.59","BodyLen":49,"BfeIP":"0.0.0.0","Url":"","UserAgent":"Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36","ClusterName":"router-tieba.orp.nj03","ServiceName":"cluster_tieba","OutStatus":200,"Referrer":""}

缩进后是：  

    {      
        "Timestamp":1454095485,
        "SessionId":9357898644039614699,
        "ProductName":"tieba",
        "ClientIP":"222.185.90.59",
        "BodyLen":49,
        "BfeIP":"0.0.0.0",
        "Url":"",
        "UserAgent":"Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36",
        "ClusterName":"router-tieba.orp.nj03",
        "ServiceName":"cluster_tieba",
        "OutStatus":200,
        "Referrer":""  
    }

两个数据中共同点是SessionId，现在需要把两个日志合并，要求具有SessionId的session日志和request日志合并在一起，其中一个
SessionId 对应一条session日志和多条request日志，简单做法如下：  

1. 按行读取session.log, json解析,获取其SessionId值和KeepAliveNum值，KeepAliveNum代表会有几条request
  日志跟它具有同样的SessionId.
2. 根据session日志的SessionID 搜索request日志中具有相同SessionId日志.
3. 最后将具有同样SessionID形成一条新的日志格式，格式为JSON.  

合并后格式为：  

    {"SesLog":session日志,"ReqLog":[request日志,request日志]}  
  
缩进后为：  

    {
        “SesLog”: session日志，
        “ReqLog”: [request日志，request日志]
    }

第一次先对SessionId排序再扫描合并速度过慢，学长给了回复建议：  

  1. 不用按SessionId, 数据本身是按时间戳排序的，首先读session日志，获取它的SesStartTime（ms单位，表示session开始时间），SesAllTime（ms单位，表示session总共时间），其对应request日志中具有同样SessionId的日志发生的时间应该在SesStartTime～（SesStartTime＋SesAllTime）之间，如果读完在这个时间段没有找到request日志则不做记录。
  2. 程序输入直接改成session文件的路径和request文件的路径。
  3. 输出形成一个新的文件。

目前我采用的方法是用python的dict存储具有相同SessionId的session和request
目前的不足之处是一次读入了所有session然后对request依次读取，占用内存较大，也没有及时清理dict中不会再满足条件的session。
可行的优化之处是根据session时间 SesStartTime + SesAllTime 的基本有序性和request的Timestamp的有序性进行分段式处理，可节约内存。  
另外：  
  * session 文件中 time 以毫秒 ms 为单位，
  * request 文件中 timestamp 以秒 s 为单位  

实现起来可能会稍复杂一些，我之前做的不够优雅而且有错误，就用了现在的方法。
