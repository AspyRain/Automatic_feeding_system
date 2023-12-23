package cqjtu.afs_mobile.util;

import android.annotation.SuppressLint;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.time.LocalTime;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import cqjtu.afs_mobile.entity.FeedingDevice;
import cqjtu.afs_mobile.entity.Plan;

public class DatabaseManager {
    private DBHelper dbHelper;
    private SQLiteDatabase database;

    public DatabaseManager(Context context) {
        dbHelper = new DBHelper(context);
    }

    public void openDatabase() {
        database = dbHelper.getWritableDatabase();
    }

    public void closeDatabase() {
        dbHelper.close();
    }

    public long insertFeedingDevice(String name) {
        ContentValues values = new ContentValues();
        values.put("name", name);
        return database.insert("feeding_devices", null, values);
    }

    public long insertPlan(Plan plan) {
        ContentValues values = new ContentValues();
        values.put("device", plan.getDevice());
        values.put("time_hour", plan.getTime().getHours());
        values.put("time_minute", plan.getTime().getMinutes());
        values.put("time_second", plan.getTime().getSeconds());
        values.put("duration", plan.getDuration());
        values.put("begin_date_month", plan.getBeginDate().getMonth());
        values.put("begin_date_day", plan.getBeginDate().getDate());
        values.put("end_date_month", plan.getEndDate().getMonth());
        values.put("end_date_day", plan.getEndDate().getDate());

        return database.insert("plans", null, values);
    }

    @SuppressLint("Range")
    public List<FeedingDevice> queryFeedingDevices() {
        Cursor cursor =database.query("feeding_devices", null, null, null, null, null, null);
        List<FeedingDevice> feedingDevices=new ArrayList<>();
        while (cursor.moveToNext()) {
            FeedingDevice feedingDevice=new FeedingDevice();
            feedingDevice.setId(cursor.getInt(cursor.getColumnIndex("id")));
            feedingDevice.setName(cursor.getString(cursor.getColumnIndex("name")));
            feedingDevices.add(feedingDevice);
        }
        return feedingDevices;
    }
    public long insertFeedingDevice(FeedingDevice feedingDevice) {
        ContentValues values = new ContentValues();
        values.put("name", feedingDevice.getName());

        return database.insert("feeding_devices", null, values);
    }




}
