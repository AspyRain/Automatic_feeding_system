package cqjtu.afs_demo;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import cqjtu.afs_demo.util.EspUtil;

public class MainActivity extends AppCompatActivity implements View.OnClickListener ,EspUtil.EspDataListener{
    private LinearLayout getTimeButton;
    private LinearLayout feed1Button;
    private LinearLayout feed2Button;
    private EditText ip_text;
    private EditText port_text;
    private EditText chat_text;
    private Button send_button;
    private final String default_ip = "192.168.118.136";
    private final String default_port ="8080";
    private EspUtil espUtil;
    private TextView dataText;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        espUtil=new EspUtil();
        getTimeButton = findViewById(R.id.get_time_button);
        feed1Button = findViewById(R.id.feed1_button);
        feed2Button = findViewById(R.id.feed2_button);
        ip_text = findViewById(R.id.ip);
        port_text = findViewById(R.id.port);
        dataText = findViewById(R.id.receiveText);
        ip_text.setText(default_ip);
        port_text.setText(default_port);
        chat_text = findViewById(R.id.chat_text);
        send_button = findViewById(R.id.send_button);

        send_button.setOnClickListener(this);
        getTimeButton.setOnClickListener(this);
        feed1Button.setOnClickListener(this);
        feed2Button.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        String deviceNum = "";
        if (v!=send_button){
            if (v == getTimeButton) {
                button_clicked(getTimeButton,R.drawable.bar_clicked);
                deviceNum = "3";  // 发送消息 "1" 给ESP01S
            } else if (v == feed1Button) {
                button_clicked(feed1Button,R.drawable.bar_clicked);
                deviceNum = "1";  // 发送消息 "2" 给ESP01S
            } else if (v == feed2Button) {
                button_clicked(feed2Button,R.drawable.bar_clicked);
                deviceNum = "2";  // 发送消息 "3" 给ESP01S
            }
            String message="{status:2,detail:{device:"+deviceNum+"}}";
            String ip = ip_text.getText().toString();
            int port = Integer.parseInt(port_text.getText().toString());
            espUtil.setIp(ip);
            espUtil.setPort(port);
            espUtil.sendMessage(deviceNum.toString(),this,true);
        }
        else{
            String message= chat_text.getText().toString();
            chat_text.setText(null);

        }


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
    public void onDataReceived(String data) {
        dataText.setText(data);
    }
}
