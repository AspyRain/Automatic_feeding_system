~~~c
{
    status:0,//指令类型 0-查看计划，1-新建计划，2-启动投喂器
    detail:{
        
    }
}
{
    status:1,//指令类型 0-查看计划，1-新建计划，2-启动投喂器
    detail:{
        device:0,//设备编号
        Time:{
            h:,
            m:,
            s:
        },
        duration:0,//持续时间
        beginDate:{
            year:,
            month:,
            day:
        },
        endDate:{
            year:,
            month:,
            day:
        }
    }
}
{
    status:2,
    detail:{
        device:0//设备编号
    }
}
~~~

