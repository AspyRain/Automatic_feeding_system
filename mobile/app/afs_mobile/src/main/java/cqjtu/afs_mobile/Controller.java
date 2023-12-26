package cqjtu.afs_mobile;

import static cqjtu.afs_mobile.util.ToastUtil.showToast;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;

import java.util.List;

import cqjtu.afs_mobile.entity.FeedingDevice;
import cqjtu.afs_mobile.util.EspUtil;
import cqjtu.afs_mobile.util.FeedingDeviceAdapter;
import de.hdodenhof.circleimageview.CircleImageView;

public class Controller extends AppCompatActivity implements EspUtil.EspDataListener {
    private CircleImageView add_button;
    private FeedingDeviceAdapter feedingDeviceAdapter;
    private List<FeedingDevice> feedingDevices;
    private String receivedData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller);

        add_button.findViewById(R.id.add_feedingDevice);
        feedingDeviceAdapter=new FeedingDeviceAdapter(this,feedingDevices);
        add_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v==add_button){
                    buttonClicked(v,R.mipmap.add_clicked);
                    Intent addFeedingIntent = new Intent(Controller.this, AddFeedingDeviceDialog.class); //前者为跳转前页面，后者为跳转后页面
                    startActivity(addFeedingIntent);
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
        this.receivedData=data;
        showToast("收到消息："+receivedData,this);
    }
}