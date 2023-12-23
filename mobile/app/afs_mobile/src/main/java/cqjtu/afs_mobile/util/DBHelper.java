package cqjtu.afs_mobile.util;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DBHelper extends SQLiteOpenHelper {
    private static final String DATABASE_NAME = "db_feeding";
    private static final int DATABASE_VERSION = 1;

    public DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        // 创建表的SQL语句
        String createTablePlans = "CREATE TABLE plans " +
                "(id INTEGER PRIMARY KEY, " +
                "device INTEGER, " +
                "time_hour INTEGER, " +
                "time_minute INTEGER, " +
                "time_second INTEGER, " +
                "duration INTEGER, " +
                "begin_date_month INTEGER, " +
                "begin_date_day INTEGER, " +
                "end_date_month INTEGER, " +
                "end_date_day INTEGER, " +
                "check ( begin_date_month<=12 and " +
                "end_date_mouth<=12 and " +
                "begin_date<=end_date_mouth and " +
                "time_hour<24 and " +
                "time_minute<60 and " +
                "time_second<60 ),"+
                "FOREIGN KEY (device) REFERENCES feeding_devices(id));";

        String createTableRecord = "CREATE TABLE record " +
                "(id INTEGER PRIMARY KEY, " +
                "plan_id INTEGER, " +
                "device INTEGER, " +
                "FOREIGN KEY (plan_id) REFERENCES plans(id), " +
                "FOREIGN KEY (device) REFERENCES feeding_devices(id));";

        String createTableFeedingDevices = "CREATE TABLE feeding_devices " +
                "(id INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "name TEXT);";


        // 执行创建表的SQL语句
        db.execSQL(createTableFeedingDevices);
        db.execSQL(createTablePlans);
        db.execSQL(createTableRecord);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // 升级数据库版本时的操作，可以根据需要进行处理
    }
}
