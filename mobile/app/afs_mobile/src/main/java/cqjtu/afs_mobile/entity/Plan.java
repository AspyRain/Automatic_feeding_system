package cqjtu.afs_mobile.entity;


import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class Plan {
    private long id;
    private long device;
    private Date time;
    private int duration;
    private Date beginDate;
    private Date endDate;


    public Plan(long id, long device, Date time, int duration, Date beginDate, Date endDate) {
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

    public Date getTime() {
        return time;
    }

    public void setDate(Date time) {
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
        SimpleDateFormat dateFormat = new SimpleDateFormat("MM-dd", Locale.getDefault());
        if (flag==0){
            return dateFormat.format(getBeginDate());
        }else {
            return dateFormat.format(getEndDate());
        }
    }
    public String getTimeString(){
        SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
        return timeFormat.format(getTime());
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
