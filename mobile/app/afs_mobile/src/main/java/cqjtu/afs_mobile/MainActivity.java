package cqjtu.afs_mobile;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private LinearLayout alarmClockButton;
    private LinearLayout controllerButton;
    private LinearLayout recorderButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        alarmClockButton = findViewById(R.id.alarmClockButton);
        controllerButton = findViewById(R.id.controllerButton);
        recorderButton = findViewById(R.id.recorderButton);
        alarmClockButton.setOnClickListener(this);
        controllerButton.setOnClickListener(this);
        recorderButton.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        button_clicked(v);
        if (v==controllerButton){
            Intent controllerIntent = new Intent(this, Controller.class); //前者为跳转前页面，后者为跳转后页面
            this.startActivity(controllerIntent);
        }
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
                        button.setBackgroundResource(R.color.button_green_clicked);
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
                        button.setBackgroundResource(R.color.main_bg);
                    }
                });
            }
        }).start();
    }


}