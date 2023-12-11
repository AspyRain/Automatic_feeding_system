package cqjtu.afs_demo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private LinearLayout getTimeButton;
    private LinearLayout feed1Button;
    private LinearLayout feed2Button;
    private EditText ip_text;
    private EditText port_text;
    private final String default_ip = "192.168.118.136";
    private final String default_port ="8080";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getTimeButton = findViewById(R.id.get_time_button);
        feed1Button = findViewById(R.id.feed1_button);
        feed2Button = findViewById(R.id.feed2_button);
        ip_text = findViewById(R.id.ip);
        port_text = findViewById(R.id.port);
        ip_text.setText(default_ip);
        port_text.setText(default_port);

        getTimeButton.setOnClickListener(this);
        feed1Button.setOnClickListener(this);
        feed2Button.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        String message = "";
        if (v == getTimeButton) {
            button_clicked(getTimeButton,R.drawable.bar_clicked);
            message = "first";  // 发送消息 "1" 给ESP01S
        } else if (v == feed1Button) {
            button_clicked(feed1Button,R.drawable.bar_clicked);
            message = "second";  // 发送消息 "2" 给ESP01S
        } else if (v == feed2Button) {
            button_clicked(feed2Button,R.drawable.bar_clicked);
            message = "third";  // 发送消息 "3" 给ESP01S
        }

        sendMessage(message);
    }

    private void sendMessage(final String message) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    String ip = ip_text.getText().toString();
                    int port = Integer.parseInt(port_text.getText().toString());

                    Socket socket = new Socket(ip, port);
                    OutputStream outputStream = socket.getOutputStream();

                    // 发送消息
                    outputStream.write(message.getBytes());

                    // 关闭连接
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
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
}
