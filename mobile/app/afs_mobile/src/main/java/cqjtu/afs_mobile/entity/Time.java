package cqjtu.afs_mobile.entity;

import java.io.Serializable;

public class Time implements Serializable {
    private int hours;
    private int minute;

    public Time(int hours, int minute) {
        this.hours = hours;
        this.minute = minute;
    }

    public int getHours() {
        return hours;
    }

    public void setHours(int hours) {
        this.hours = hours;
    }

    public int getMinute() {
        return minute;
    }

    public void setMinute(int minute) {
        this.minute = minute;
    }
}
