package cqjtu.afs_mobile;

import static cqjtu.afs_mobile.util.JsonUtil.parseDeviceJsonString;
import static cqjtu.afs_mobile.util.ToastUtil.showToast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ListView;

import java.util.List;

import cqjtu.afs_mobile.entity.ConnectionInfo;
import cqjtu.afs_mobile.entity.FeedingDevice;
import cqjtu.afs_mobile.util.EspUtil;
import cqjtu.afs_mobile.util.FeedingDeviceAdapter;
import cqjtu.afs_mobile.util.ToastUtil;
import de.hdodenhof.circleimageview.CircleImageView;

public class Controller extends AppCompatActivity implements EspUtil.EspDataListener, AddFeedingDeviceDialogFragment.OnDialogResultListener {
    private LinearLayout add_button;
    private FeedingDeviceAdapter feedingDeviceAdapter;
    private List<FeedingDevice> feedingDevices;
    private String receivedData;
    private EspUtil espUtil;
    private ListView listView;
    private ConnectionInfo connectionInfo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller);
        listView = findViewById(R.id.listView);
        add_button = findViewById(R.id.add_feedingDevice);
        Intent intent = getIntent();
    //    add_button.findViewById(R.id.add_feedingDevice);
        espUtil = new EspUtil(this);

        if (intent != null) {
            connectionInfo=new ConnectionInfo(intent.getStringExtra("IP"),intent.getIntExtra("Port", 0));
            ToastUtil.showToast("ip:"+connectionInfo.getIp()+"port:"+ connectionInfo.getPort(),this);
            espUtil.setIp(connectionInfo.getIp());
            espUtil.setPort(connectionInfo.getPort());
            espUtil.setDataListener(this);
        }
        espUtil.sendMessage("{status:0,detail:{type:0}}",this,true);

        add_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v==add_button){
                    buttonClicked(v,R.mipmap.add_clicked);
                    // 使用 DialogFragment 显示对话框
                    AddFeedingDeviceDialogFragment dialogFragment = new AddFeedingDeviceDialogFragment();
                    dialogFragment.setOnDialogResultListener(Controller.this);
                    dialogFragment.show(getSupportFragmentManager(), "add_feeding_device_dialog");
                }
            }
        });
    }
    private void buttonClicked(View button, int clicked_resID) {
        Handler handler = new Handler();
        Drawable origin_res = button.getBackground();
        // 在新线程中执行操作
        new Thread(new Runnable() {
            @Override
            public void run() {
                // 在新线程中设置背景颜色
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        button.setBackgroundResource(clicked_resID);
                    }
                });

                // 延时 0.1 秒
                try {
                    Thread.sleep(100);  // 100 毫秒即 0.1 秒
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                // 在新线程中恢复背景颜色
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        button.setBackground(origin_res);
                    }
                });
            }
        }).start();
    }

    @Override
    public void onDataReceived(String data) {
        this.receivedData = data;
        espUtil.stopReceivingData();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                feedingDevices = parseDeviceJsonString(Controller.this.receivedData);
                // 设置 RecyclerView 的布局管理器
                // 初始化适配器
                feedingDeviceAdapter = new FeedingDeviceAdapter(Controller.this, feedingDevices,connectionInfo);

                // 将适配器设置给 ListView
                listView.setAdapter(feedingDeviceAdapter);
            }
        });
    }


    @Override
    public void onPositiveResult(String deviceName) {
        if (deviceName!=null){
            espUtil.sendMessage("{status:1,detail:{type:0,device:{deviceName:"+deviceName+" }}}",this,false);
            // 延时操作，延时 100 毫秒
            new Handler().postDelayed(() -> {
                espUtil.sendMessage("{status:0,detail:{type:0}}",this,true);
            }, 1500);
        }else {
            showToast("请输入值!",this);
        }
    }

    @Override
    public void onNegativeResult() {
        showToast("取消操作!",this);
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