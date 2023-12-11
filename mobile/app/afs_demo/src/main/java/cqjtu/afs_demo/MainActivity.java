package cqjtu.afs_demo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextClock;
import android.Manifest;

import cqjtu.afs_demo.util.PermissionUtil;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private TextClock timeText;
    private TextClock dateText;
    private LinearLayout getTimeButton;
    private LinearLayout feed1Button;
    private LinearLayout feed2Button;
    private final String ip = "192.168.118.136";
    private final String port ="8080";

    private static final String[] PERMISSIONS=new String[]{
            Manifest.permission.INTERNET,//访问互联网

    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        timeText = findViewById(R.id.timeText);
        dateText = findViewById(R.id.dateText);
        feed1Button = findViewById(R.id.feed1_button);
        feed2Button = findViewById(R.id.feed2_button);
        getTimeButton = findViewById(R.id.get_time_button);

        getTimeButton.setOnClickListener(this);
        feed1Button.setOnClickListener(this);
        feed2Button.setOnClickListener(this);

        // 注册广播接收器
        PermissionUtil.checkPermission(this,PERMISSIONS,100);
    }
    private void button_clicked(View button, int clicked_resID) {
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
    public void onClick(View v) {
        if (v == getTimeButton) {
            button_clicked(getTimeButton, R.drawable.bar_clicked);
            sendMessage("1");  // 发送消息 "1" 给ESP01S
        } else if (v == feed1Button) {
            button_clicked(feed1Button, R.drawable.bar_clicked);
            sendMessage("2");  // 发送消息 "2" 给ESP01S
        } else if (v == feed2Button) {
            button_clicked(feed2Button, R.drawable.bar_clicked);
            sendMessage("3");  // 发送消息 "3" 给ESP01S
        }
    }

    private void sendMessage(final String message) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    String url = "http://" + ip + ":" + port;  // 拼接URL
                    okhttp3.OkHttpClient client = new okhttp3.OkHttpClient();
                    okhttp3.RequestBody body = okhttp3.RequestBody.create(message, okhttp3.MediaType.parse("text/plain"));

                    okhttp3.Request request = new okhttp3.Request.Builder()
                            .url(url)
                            .post(body)
                            .build();

                    okhttp3.Response response = client.newCall(request).execute();

                    // 处理响应，如果有需要的话
                    String responseBody = response.body().string();
                    // 在这里添加处理响应的代码

                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

}