package cqjtu.afs_mobile.util;

import static cqjtu.afs_mobile.util.ToastUtil.showToast;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;

public class EspUtil {
    private String ip;
    private int port;
    private static final String TAG = "EspUtil";
    private boolean isReceivingData = false;
    private String receivedData;
    private EspDataListener dataListener;
    private Socket socket;
    private Context context;

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

    public EspUtil(Context context) {
        this.context = context;
    }

    public EspUtil() {
    }

    public void sendMessage(final String message, Context context, boolean isReceiveData) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    if (socket == null || socket.isClosed()) {
                        socket = new Socket(ip, port);
                    }
                    OutputStream outputStream = socket.getOutputStream();
                    // 发送消息
                    outputStream.write(message.getBytes());
                    if (isReceiveData){
                        startReceivingData();
                    }
                    if (!isReceivingData)stopReceivingData();
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
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public interface EspDataListener {
        void onDataReceived(String data);
    }
    public void testConnection(final ConnectionCallback callback) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // 尝试建立连接
                    if (socket == null || socket.isClosed()) {
                        socket = new Socket(ip, port);
                    }

                    // 如果连接成功，运行回调函数
                    if (socket.isConnected()) {
                        socket.close();
                        callback.onSuccess();
                    } else {
                        callback.onFailure("连接失败");
                    }

                } catch (IOException e) {
                    // 连接失败时的异常处理
                    e.printStackTrace();
                    callback.onFailure("连接失败：" + e.getMessage());
                }
            }
        }).start();
    }

    // 回调接口，用于处理连接成功或失败的情况
    public interface ConnectionCallback {
        void onSuccess();

        void onFailure(String errorMessage);
    }
    private class EspCommunicationTask extends AsyncTask<Void, String, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            try {
                if (socket == null || socket.isClosed()) {
                    socket = new Socket(ip, port);
                }
                InputStream inputStream = socket.getInputStream();
                InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
                BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

                // 在主线程中显示 Toast
                publishProgress("连接成功");
                checkClose();
                while (isReceivingData) {
                    String receivedData = bufferedReader.readLine();
                    if (receivedData != null) {
                        // 将耗时操作移动到主线程
                        publishProgress(receivedData);
                    }

                }
            } catch (IOException e) {
                if (isReceivingData) {
                    Log.e(TAG, "后台任务出错：" + e.getMessage());
                    e.printStackTrace();
                }
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            // 处理接收到的数据或显示 Toast
            if (values.length > 0) {
                Log.d(TAG, "接收到的数据：" + values[0]);

                // 在主线程中处理接收到的数据
                if ("连接成功".equals(values[0])) {
                    showToast("连接成功", context);
                } else {
                    showToast("接收到数据：" + values[0], context);
                    // 处理其他数据
                    if (dataListener != null) {
                        dataListener.onDataReceived(values[0]);
                    }
                }
            }
            // TODO: 处理接收到的数据（更新 UI 或执行其他操作）
        }
        public void checkClose() {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // 等待1秒
                        Thread.sleep(1000);

                        // 在新线程中检查socket是否已关闭
                        if (socket != null && !socket.isClosed()) {
                            socket.close();
                        }
                    } catch (InterruptedException | IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
}
