package cqjtu.afs_mobile;

import static cqjtu.afs_mobile.util.ToastUtil.showToast;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;

import cqjtu.afs_mobile.util.EspUtil;

public class Load extends AppCompatActivity {

    private LinearLayout enterButton;
    private EditText ip_text;
    private EditText port_text;
    private EspUtil espUtil;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_load);
        enterButton=findViewById(R.id.enterButton);
        ip_text = findViewById(R.id.ip_text);
        port_text = findViewById(R.id.port_text);
        espUtil = new EspUtil(this);
        enterButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                button_clicked(enterButton);
                String ip = ip_text.getText().toString();
                int port = Integer.parseInt(port_text.getText().toString());
                espUtil.setIp(ip);
                espUtil.setPort(port);
                espUtil.testConnection(new EspUtil.ConnectionCallback() {
                    @Override
                    public void onSuccess() {
                        // 连接成功的操作
                        showToast("连接成功", Load.this);
                        Intent loadIntent = new Intent(Load.this, MainActivity.class);
                        loadIntent.putExtra("IP", ip);
                        loadIntent.putExtra("Port", port);
                        Load.this.startActivity(loadIntent);
                    }

                    @Override
                    public void onFailure(String errorMessage) {
                        // 连接失败的操作
                        showToast(errorMessage, Load.this);
                    }
                });
            }
        });
    }
    private void button_clicked(View button) {
        Handler handler = new Handler();
        // 在新线程中执行操作
        new Thread(new Runnable() {
            @Override
            public void run() {
                // 在新线程中设置背景颜色
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        button.setBackgroundResource(R.drawable.rounded_corners_button);
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
                        button.setBackgroundResource(R.drawable.rounded_corners_button_clicked);
                    }
                });
            }
        }).start();
    }
}