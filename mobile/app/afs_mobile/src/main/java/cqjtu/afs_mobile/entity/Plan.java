package cqjtu.afs_mobile.entity;


import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Locale;

public class Plan implements Serializable {
    private long id;
    private long device;
    private Time time;
    private int duration;
    private Date beginDate;
    private Date endDate;


    public Plan(long id, long device, Time time, int duration, Date beginDate, Date endDate) {
        this.id = id;
        this.device = device;
        this.time = time;
        this.duration = duration;
        this.beginDate = beginDate;
        this.endDate = endDate;
    }
    public Plan() {

    }
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public long getDevice() {
        return device;
    }

    public void setDevice(long device) {
        this.device = device;
    }

    public Time getTime() {
        return time;
    }

    public void setTime(Time time) {
        this.time = time;
    }

    public int getDuration() {
        return duration;
    }

    public void setDuration(int duration) {
        this.duration = duration;
    }

    public Date getBeginDate() {
        return beginDate;
    }
    public String getDateString(int flag){
        if (flag==0){
            return ""+beginDate.getMonth()+"月"+beginDate.getDay()+"日";
        }else {
            return ""+endDate.getMonth()+"月"+endDate.getDay()+"日";
        }
    }
    public String getTimeString(){
        return ""+time.getHours()+":"+time.getMinute();
    }
    public void setBeginDate(Date beginDate) {
        this.beginDate = beginDate;
    }

    public Date getEndDate() {
        return endDate;
    }

    public void setEndDate(Date endDate) {
        this.endDate = endDate;
    }
}
