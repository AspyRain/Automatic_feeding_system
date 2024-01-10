package cqjtu.afs_mobile;

import static cqjtu.afs_mobile.util.JsonUtil.parseDeviceJsonString;
import static cqjtu.afs_mobile.util.ToastUtil.showToast;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.NumberPicker;
import android.widget.Spinner;
import android.widget.TimePicker;

import java.text.ParseException;
import java.time.LocalDate;
import java.util.Calendar;
import java.util.List;

import cqjtu.afs_mobile.entity.ConnectionInfo;
import cqjtu.afs_mobile.entity.Date;
import cqjtu.afs_mobile.entity.FeedingDevice;
import cqjtu.afs_mobile.entity.Plan;
import cqjtu.afs_mobile.entity.Time;
import cqjtu.afs_mobile.util.EspUtil;
import cqjtu.afs_mobile.util.FeedingDeviceAdapter;
import cqjtu.afs_mobile.util.FeedingDeviceSpinnerAdapter;
import cqjtu.afs_mobile.util.ToastUtil;


public class addPlan extends AppCompatActivity implements EspUtil.EspDataListener{
    private TimePicker tp_time;
    private NumberPicker beginMonthPicker;
    private NumberPicker beginDayPicker;
    private NumberPicker endMonthPicker;
    private NumberPicker endDayPicker;
    private Spinner deviceSpinner;
    private EditText planDuration;
    private LinearLayout sendButton;
    private ConnectionInfo connectionInfo;
    private List<FeedingDevice> feedingDevices;
    private EspUtil espUtil;
    private Plan plan = new Plan();
    private boolean isDeviceSelected;
    private long deviceId;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_plan);
        tp_time = findViewById(R.id.tp_time);
        beginMonthPicker = findViewById(R.id.beginMonthPicker);
        beginDayPicker = findViewById(R.id.beginDayPicker);
        endMonthPicker = findViewById(R.id.endMonthPicker);
        endDayPicker = findViewById(R.id.endDayPicker);
        planDuration = findViewById(R.id.planDuration);
        sendButton = findViewById(R.id.sendButton);
        planDuration = findViewById(R.id.planDuration);
        sendButton = findViewById(R.id.sendButton);
        deviceSpinner = findViewById(R.id.deviceSpinner);

        tp_time.setIs24HourView(true);
        beginMonthPicker.setMaxValue(12);
        beginMonthPicker.setMinValue(0);
        endMonthPicker.setMaxValue(12);
        endMonthPicker.setMinValue(0);
        beginDayPicker.setMaxValue(31);
        beginDayPicker.setMinValue(0);
        endDayPicker.setMaxValue(31);
        endDayPicker.setMinValue(0);
        beginMonthPicker.setValue(0);
        beginDayPicker.setValue(0);
        endMonthPicker.setValue(0);
        endDayPicker.setValue(0);
        isDeviceSelected=false;
        Intent intent = getIntent();
        espUtil = new EspUtil(this);
        if (intent != null){
            connectionInfo = new ConnectionInfo(intent.getStringExtra("IP"),intent.getIntExtra("Port",0));
            showToast("ip:"+connectionInfo.getIp()+"port:"+connectionInfo.getPort(),this);
            espUtil.setIp(connectionInfo.getIp());
            espUtil.setPort(connectionInfo.getPort());
            espUtil.setDataListener(this);
        }
        espUtil.sendMessage("{status:0,detail:{type:0}}",this,true);
        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 封装时间数据
                int hour = tp_time.getHour();
                int minute = tp_time.getMinute();

                // 设置计划时间
                plan.setTime(new Time(hour,minute));

                // 封装起始日期数据
                int beginMonth = beginMonthPicker.getValue();
                int beginDay = beginDayPicker.getValue();

                // 设置计划起始日期
                plan.setBeginDate(new Date(beginMonth,beginDay));

                // 封装结束日期数据
                int endMonth = endMonthPicker.getValue();
                int endDay = endDayPicker.getValue();

                // 设置计划结束日期
                plan.setEndDate(new Date(endMonth,endDay));

                // 封装设备ID和持续时间
                if (isDeviceSelected) {
                    plan.setDevice(deviceId);
                    String durationString = planDuration.getText().toString();
                    if (!durationString.isEmpty()) {
                        int duration = Integer.parseInt(durationString);
                        plan.setDuration(duration);
                        // 返回结果给上一个 Activity
                        Intent resultIntent = new Intent();
                        resultIntent.putExtra("plan",plan);
                        setResult(7, resultIntent);
                        addPlan.this.finish(); // 关闭当前 Activity
                    } else {
                        showToast("请输入持续时间", addPlan.this);
                    }
                } else {
                    showToast("请选择设备", addPlan.this);
                }
            }
        });



    }
    @Override
    public void onBackPressed() {
        // 在退出界面时执行的功能
        // 可以调用自定义的方法或写具体的逻辑
        espUtil.stopReceivingData();

        // 调用父类的方法以确保正常的退出处理
        super.onBackPressed();
    }
    @Override
    public void onDataReceived(String data) {
        espUtil.stopReceivingData();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                feedingDevices = parseDeviceJsonString(data);
                // 创建自定义适配器
                FeedingDeviceSpinnerAdapter adapter = new FeedingDeviceSpinnerAdapter(addPlan.this, feedingDevices);
                deviceSpinner.setAdapter(adapter);
                deviceSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {
                        // 获取所选设备的ID
                        addPlan.this.deviceId = adapter.getItemId(position+1);
                        addPlan.this.isDeviceSelected =true;
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parentView) {
                        addPlan.this.isDeviceSelected =false;
                    }
                });
            }
        });
    }
}