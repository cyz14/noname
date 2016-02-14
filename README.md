# noname

## MergeSessionAndRequest  
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
2. 根据session日志的SessionID 搜索request日志中具有相同SessionId日志  
3. 最后将具有同样SessionID形成一条新的日志格式，格式为JSON，基本如下： 
  

    {"SesLog":session日志,"ReqLog":[request日志,request日志]}
  
缩进后为：  

    {
        “SesLog”: session日志，
        “ReqLog”: [request日志，request日志]
    }