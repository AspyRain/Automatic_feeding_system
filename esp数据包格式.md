~~~c
{
    status:0,//指令类型 0-查询指令，1-新建计划，2-启动投喂器
    detail:{
        type:0 //查询的数据类型:0-设备 1-计划 2-记录
    }
}
{
    status:1,//指令类型 0-查看计划，1-新建数据，2-启动投喂器
    detail:{
        type:0, //插入的数据类型:0-设备 1-计划 2-记录 
        {
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
}
{
    status:2,
    detail:{
        device:0//设备编号
    }
}
{
    status:3,//删除计划
    detail:{
        type:0,//0-设备 1-计划
        id:0//设备/计划id
    }
}
~~~

