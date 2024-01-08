package cqjtu.afs_mobile;

import static cqjtu.afs_mobile.util.JsonUtil.parseDeviceJsonString;
import static cqjtu.afs_mobile.util.JsonUtil.parsePlanJsonString;
import static cqjtu.afs_mobile.util.JsonUtil.planToJsonString;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

import cqjtu.afs_mobile.entity.ConnectionInfo;
import cqjtu.afs_mobile.entity.Plan;
import cqjtu.afs_mobile.util.EspUtil;
import cqjtu.afs_mobile.util.FeedingDeviceAdapter;
import cqjtu.afs_mobile.util.PlanAdapter;
import cqjtu.afs_mobile.util.ToastUtil;

public class AlarmClock extends AppCompatActivity implements EspUtil.EspDataListener{
    private EspUtil espUtil;
    private ListView listView;
    private LinearLayout addPlan;
    private String receivedData;
    private ConnectionInfo connectionInfo;
    private PlanAdapter planAdapter;
    private List<Plan> plans;
    private Plan newPlan;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alarm_clock);
        listView = findViewById(R.id.listView);
        addPlan = findViewById(R.id.addPlan);
        Intent intent = getIntent();
        espUtil = new EspUtil(this);
        plans=new ArrayList<>();
        if (intent != null){
            connectionInfo = new ConnectionInfo(intent.getStringExtra("IP"),intent.getIntExtra("Port",0));
            ToastUtil.showToast("ip:"+connectionInfo.getIp()+"port:"+connectionInfo.getPort(),this);
            espUtil.setIp(connectionInfo.getIp());
            espUtil.setPort(connectionInfo.getPort());
            espUtil.setDataListener(this);
        }
        addPlan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent addPlanIntent = new Intent(AlarmClock.this,addPlan.class);
                addPlanIntent.putExtra("IP", connectionInfo.getIp());
                addPlanIntent.putExtra("Port", connectionInfo.getPort());
                AlarmClock.this.startActivityForResult(addPlanIntent,7);
            }
        });
        espUtil.sendMessage("{status:0,detail:{type:1}}",this,true);
    }

    @Override
    public void onDataReceived(String data) {
        this.receivedData = data;
        espUtil.stopReceivingData();
        if ("OK".equals(this.receivedData)){
            this.newPlan.setId(plans.size());
            plans.add(this.newPlan);
            planAdapter.notifyDataSetChanged();
        }
        else {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                        AlarmClock.this.plans=parsePlanJsonString(AlarmClock.this.receivedData);
                        // 设置 RecyclerView 的布局管理器
                        // 初始化适配器
                        planAdapter = new PlanAdapter(AlarmClock.this, plans,connectionInfo);

                        // 将适配器设置给 ListView
                        listView.setAdapter(planAdapter);
                }
            });
        }
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == 7 && resultCode == 7) {
            Plan plan=(Plan) data.getSerializableExtra("plan");
            AlarmClock.this.newPlan=plan;
            //发送计划数据到设备
            espUtil.sendMessage("{status:1,detail:{type:1," + planToJsonString(newPlan) + "}}", AlarmClock.this, true);
        }
    }
    @Override
    public void onBackPressed() {
        // 在退出界面时执行的功能
        // 可以调用自定义的方法或写具体的逻辑
        espUtil.stopReceivingData();

        // 调用父类的方法以确保正常的退出处理
        super.onBackPressed();
    }

}