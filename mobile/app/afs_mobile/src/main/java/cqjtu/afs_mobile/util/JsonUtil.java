package cqjtu.afs_mobile.util;
import org.json.JSONArray;
import org.json.JSONObject;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.text.SimpleDateFormat;

import cqjtu.afs_mobile.entity.FeedingDevice;
import cqjtu.afs_mobile.entity.Plan;

public class JsonUtil {
    public static List<Plan> parsePlanJsonString(String jsonString) {
        List<Plan> plans = new ArrayList<>();

        try {
            JSONArray jsonArray = new JSONArray(jsonString);

            for (int i = 0; i < jsonArray.length(); i++) {
                JSONObject jsonObject = jsonArray.getJSONObject(i);

                long id = jsonObject.getLong("id");
                long device = jsonObject.getLong("device");

                JSONObject timeObject = jsonObject.getJSONObject("time");
                int hour = timeObject.getInt("h");
                int minute = timeObject.getInt("m");
                int second = timeObject.getInt("s");
                Date time = parseTime(hour, minute, second);

                int duration = jsonObject.getInt("duration");

                JSONObject beginDateObject = jsonObject.getJSONObject("beginDate");
                int beginYear = beginDateObject.getInt("year");
                int beginMonth = beginDateObject.getInt("month");
                int beginDay = beginDateObject.getInt("day");
                Date beginDate = parseDate(beginYear, beginMonth, beginDay);

                JSONObject endDateObject = jsonObject.getJSONObject("endDate");
                int endYear = endDateObject.getInt("year");
                int endMonth = endDateObject.getInt("month");
                int endDay = endDateObject.getInt("day");
                Date endDate = parseDate(endYear, endMonth, endDay);

                // 创建并添加Plan对象到列表
                Plan plan = new Plan(id, device, time, duration, beginDate, endDate);
                plans.add(plan);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return plans;
    }
    public static List<FeedingDevice> parseDeviceJsonString(String jsonString) {
        List<FeedingDevice> feedingDevices = new ArrayList<>();

        try {
            JSONArray jsonArray = new JSONArray(jsonString);

            for (int i = 0; i < jsonArray.length(); i++) {
                JSONObject jsonObject = jsonArray.getJSONObject(i);

                long id = jsonObject.getLong("id");
                String name = jsonObject.getString("name");

                // 创建并添加FeedingDevice对象到列表
                FeedingDevice feedingDevice = new FeedingDevice();
                feedingDevice.setId(id);
                feedingDevice.setName(name);
                feedingDevices.add(feedingDevice);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return feedingDevices;
    }
    public static Date parseTime(int hour, int minute, int second) throws ParseException {
        String timeString = hour + ":" + minute + ":" + second;
        SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
        return format.parse(timeString);
    }

    private static Date parseDate(int year, int month, int day) throws ParseException {
        String dateString = year + "-" + month + "-" + day;
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        return format.parse(dateString);
    }
    public static String planToJsonString(Plan plan){
        return "{"+Long.toString(plan.getDevice())+",:"+plan.getTime().getHours()+","+plan.getTime().getMinutes()+"," +
                plan.getTime().getSeconds()+"."+Integer.toString(plan.getDuration())+",:"+plan.getBeginDate().getYear()+","+plan.getBeginDate().getMonth()
                +","+plan.getBeginDate().getDay()+".:"+plan.getEndDate().getYear()+","+plan.getEndDate().getMonth()+","+plan.getEndDate().getDay()+"}";
    }
}
