package cqjtu.afs_demo.util;

import static cqjtu.afs_demo.util.ToastUtil.showToast;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EspUtil {
    private int port;
    private static final String TAG = "EspUtil";
    private boolean isReceivingData = false;
    private String receivedData;
    private EspDataListener dataListener;
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private Context context;

    public void setDataListener(EspDataListener listener) {
        this.dataListener = listener;
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
                    if (clientSocket != null && !clientSocket.isClosed()) {
                        OutputStream outputStream = clientSocket.getOutputStream();
                        // 发送消息
                        outputStream.write(message.getBytes());
                    }
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
            if (clientSocket != null && !clientSocket.isClosed()) {
                clientSocket.close();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
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

                // 在主线程中显示 Toast
                publishProgress("等待连接...");

                // 等待连接
                clientSocket = serverSocket.accept();

                InputStream inputStream = clientSocket.getInputStream();
                InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
                BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

                // 在主线程中显示 Toast
                publishProgress("连接成功");

                while (isReceivingData) {
                    String receivedData = bufferedReader.readLine();
                    if (receivedData != null) {
                        // 将耗时操作移动到主线程
                        publishProgress(receivedData);
                    }
                    Thread.sleep(20);
                }
            } catch (IOException e) {
                if (isReceivingData) {
                    Log.e(TAG, "后台任务出错：" + e.getMessage());
                    e.printStackTrace();
                }
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            } finally {
                try {
                    if (serverSocket != null && !serverSocket.isClosed()) {
                        serverSocket.close();
                    }
                    if (clientSocket != null && !clientSocket.isClosed()) {
                        clientSocket.close();
                    }
                } catch (IOException e) {
                    Log.e(TAG, "在 finally 块中关闭 socket 出错：" + e.getMessage());
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
                if ("等待连接...".equals(values[0])) {
                    showToast("等待连接...", context);
                } else if ("连接成功".equals(values[0])) {
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
    }
}
