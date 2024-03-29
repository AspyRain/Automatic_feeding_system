package cqjtu.afs_mobile.entity;

import java.io.Serializable;

public class Date implements Serializable {
    private int month;
    private int day;

    public Date(int month, int day) {
        this.month = month;
        this.day = day;
    }

    public int getMonth() {
        return month;
    }

    public void setMonth(int month) {
        this.month = month;
    }

    public int getDay() {
        return day;
    }

    public void setDay(int day) {
        this.day = day;
    }
}
