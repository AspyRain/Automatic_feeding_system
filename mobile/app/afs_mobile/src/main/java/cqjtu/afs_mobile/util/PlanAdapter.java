package cqjtu.afs_mobile.util;

import static cqjtu.afs_mobile.util.ToastUtil.showToast;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

import cqjtu.afs_mobile.R;
import cqjtu.afs_mobile.entity.ConnectionInfo;
import cqjtu.afs_mobile.entity.FeedingDevice;
import cqjtu.afs_mobile.entity.Plan;

public class PlanAdapter extends ArrayAdapter<Plan> {
    private LayoutInflater inflater;
    private ConnectionInfo connectionInfo;
    private EspUtil espUtil;
    private Context context;

    public PlanAdapter(Context context, List<Plan> dataList, ConnectionInfo connectionInfo) {
        super(context, 0, dataList);
        inflater = LayoutInflater.from(context);
        this.context = context;
        this.connectionInfo = connectionInfo;
        espUtil = new EspUtil(context);
        espUtil.setIp(connectionInfo.getIp());
        espUtil.setPort(connectionInfo.getPort());
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        ViewHolder holder;

        if (convertView == null) {
            convertView = inflater.inflate(R.layout.item_plan, parent, false);
            holder = new ViewHolder(convertView);
            convertView.setTag(holder);
        } else {
            holder = (ViewHolder) convertView.getTag();
        }

        Plan data = getItem(position);

        if (data != null) {
            holder.planId.setText(String.valueOf(data.getId()));
            holder.deviceId.setText(String.valueOf(data.getDevice()));
            holder.planTime.setText(data.getTimeString());
            holder.beginDate.setText(data.getDateString(0));
            holder.endDate.setText(data.getDateString(1));
            holder.planDuration.setText(Integer.toString(data.getDuration())+"s");
        }
        final ImageView deleteButton = holder.deleteButton;
        if (data != null) {
            deleteButton.setTag(data.getId());
        }
        deleteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showConfirmationDialog(context);
            }
        });
        return convertView;
    }
    private void showConfirmationDialog(Context context) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);

        // 设置对话框标题
        builder.setTitle("确认删除吗");

        // 设置对话框消息
        builder.setMessage("是否删除该Plan");

        // 设置确认按钮
        builder.setPositiveButton("确定", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                positiveAction(id);
                dialog.dismiss(); // 关闭对话框
            }
        });

        // 设置取消按钮
        builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                showToast("取消删除!",context);
                dialog.dismiss(); // 关闭对话框
            }
        });

        // 创建并显示对话框
        AlertDialog dialog = builder.create();
        dialog.show();
    }

    public void positiveAction(int id){
        espUtil.sendMessage("{status:3,detail:{type:1,id:"+id+"}}",context,false);
        new Handler().postDelayed(() -> {
            espUtil.sendMessage("{status:0,detail:{type:1}}",context,true);
        }, 1500);
    }
    public static class ViewHolder extends RecyclerView.ViewHolder {
        TextView planTime;
        TextView beginDate;
        TextView endDate;
        TextView planDuration;
        TextView planId;
        TextView deviceId;
        ImageView deleteButton;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            planTime = itemView.findViewById(R.id.planTime);
            beginDate = itemView.findViewById(R.id.beginDate);
            endDate = itemView.findViewById(R.id.endDate);
            planDuration = itemView.findViewById(R.id.planDuration);
            planId = itemView.findViewById(R.id.planId);
            deviceId = itemView.findViewById(R.id.deviceId);
            deleteButton = itemView.findViewById(R.id.deletePlan);
        }
    }


}