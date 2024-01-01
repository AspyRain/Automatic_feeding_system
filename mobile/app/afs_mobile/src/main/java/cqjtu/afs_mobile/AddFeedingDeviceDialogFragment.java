package cqjtu.afs_mobile;

import android.app.Dialog;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;

public class AddFeedingDeviceDialogFragment extends DialogFragment {
    private EditText editTextDeviceName;
    @NonNull
    @Override
    public Dialog onCreateDialog(@Nullable Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(requireActivity());

        // 将布局填充到对话框中
        View dialogView = View.inflate(requireActivity(), R.layout.activity_add_feeding_device_dialog, null);
        builder.setView(dialogView);

        // 在布局中找到 EditText
        editTextDeviceName = dialogView.findViewById(R.id.editTextDeviceName);

        // 设置对话框按钮和点击事件
        builder.setPositiveButton("添加", (dialog, which) -> {
            // 获取 EditText 的文本
            String deviceName = editTextDeviceName.getText().toString();

            // 触发正面按钮点击的回调，并传递 deviceName
            sendPositiveResult(deviceName);
        });

        builder.setNegativeButton("取消", (dialog, which) -> {
            // 触发取消按钮点击的回调
            sendNegativeResult();
        });

        return builder.create();
    }
    public interface OnDialogResultListener {
        void onPositiveResult(String deviceName); // 可以根据需要传递参数
        void onNegativeResult();
    }

    private OnDialogResultListener resultListener;


    private void sendPositiveResult(String deviceName) {
        if (resultListener != null) {
            resultListener.onPositiveResult(deviceName);
        }
    }

    private void sendNegativeResult() {
        if (resultListener != null) {
            resultListener.onNegativeResult();
        }
    }

    public void setOnDialogResultListener(OnDialogResultListener listener) {
        this.resultListener = listener;
    }

}
