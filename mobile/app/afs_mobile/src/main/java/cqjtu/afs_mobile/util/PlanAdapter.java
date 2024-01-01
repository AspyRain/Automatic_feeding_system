package cqjtu.afs_mobile.util;

import android.app.AlertDialog;
import android.content.Context;
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
    private ImageView deleteButton;

    public PlanAdapter(Context context, List<Plan> dataList,ConnectionInfo connectionInfo) {
        super(context, 0, dataList);
        inflater = LayoutInflater.from(context);
        this.context=context;
        this.connectionInfo=connectionInfo;
        espUtil=new EspUtil(context);
        espUtil.setIp(connectionInfo.getIp());
        espUtil.setPort(connectionInfo.getPort());
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        ViewHolder holder;

        if (convertView == null) {
            convertView = inflater.inflate(R.layout.item_feeding_device, parent, false);
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
            holder.planDuration.setText(data.getDuration());
        }
        final ImageView deleteButton=holder.deleteButton;
        deleteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                builder.setTitle("确认删除?");
                builder.setMessage("是否删除该计划");

            }
        });
        return convertView;
    }
    public void positiveAction(){

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
