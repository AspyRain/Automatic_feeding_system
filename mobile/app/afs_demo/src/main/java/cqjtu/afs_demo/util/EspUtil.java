package cqjtu.afs_demo.util;

import static cqjtu.afs_demo.util.ToastUtil.showToast;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EspUtil {
    private String ip;
    private int port;
    private static final String TAG = "EspUtil";
    private ServerSocket serverSocket;
    private boolean isReceivingData = false;

    private String receivedData;
    private EspDataListener dataListener;

    public void setDataListener(EspDataListener listener) {
        this.dataListener = listener;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public EspUtil(String ip, int port) {
        this.ip = ip;
        this.port = port;
    }

    public EspUtil() {
    }

    public void sendMessage(final String message, Context context,boolean isReceiveData) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Socket socket = new Socket(ip, port);
                    OutputStream outputStream = socket.getOutputStream();
                    // 发送消息
                    outputStream.write(message.getBytes());
                    // 关闭连接
                    socket.close();
                    startReceivingData();

                    long startTime = System.currentTimeMillis();
                    while (true) {
                        if (System.currentTimeMillis() - startTime >= 2000){
                            showToast("发送失败!", context);
                            break;
                        }
                        if (isReceivingData) {
                            if ("OK".equals(receivedData)) {
                                showToast("发送成功!", context);
                                if (dataListener != null&&isReceiveData) {
                                    dataListener.onDataReceived(receivedData);
                                }
                            }
                            isReceivingData=false;
                            break;
                        }
                    }
                    stopReceivingData();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }
    public void startReceivingData() {
        isReceivingData = true;
        new EspCommunicationTask().execute();
    }

    public void stopReceivingData() {
        isReceivingData = false;
        try {
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
            }
        } catch (IOException e) {
            Log.e(TAG, "Error closing serverSocket: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public interface EspDataListener {
        void onDataReceived(String data);
    }

    private class EspCommunicationTask extends AsyncTask<Void, String, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            try {
                serverSocket = new ServerSocket(port);

                while (isReceivingData) {
                    Socket clientSocket = serverSocket.accept();
                    InputStream inputStream = clientSocket.getInputStream();
                    InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
                    BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

                    String receivedData = bufferedReader.readLine();

                    // 处理接收到的数据，可以通过 publishProgress 发送给 UI 线程
                    publishProgress(receivedData);

                    // 关闭连接
                    clientSocket.close();
                }
            } catch (IOException e) {
                if (isReceivingData) {
                    Log.e(TAG, "Error in background task: " + e.getMessage());
                    e.printStackTrace();
                }
            } finally {
                try {
                    if (serverSocket != null && !serverSocket.isClosed()) {
                        serverSocket.close();
                    }
                } catch (IOException e) {
                    Log.e(TAG, "Error closing serverSocket in finally block: " + e.getMessage());
                    e.printStackTrace();
                }
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            // 处理接收到的数据，更新 UI 或执行其他操作
            receivedData = values[0];
            isReceivingData=true;
            Log.d(TAG, "Received data: " + receivedData);

            // TODO: 在这里处理接收到的数据，例如更新UI或者执行其他操作
        }
    }
}
