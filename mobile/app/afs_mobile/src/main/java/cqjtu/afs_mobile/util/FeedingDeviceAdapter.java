package cqjtu.afs_mobile.util;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.Switch;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.List;

import cqjtu.afs_mobile.R;
import cqjtu.afs_mobile.entity.ConnectionInfo;
import cqjtu.afs_mobile.entity.FeedingDevice;

public class FeedingDeviceAdapter extends ArrayAdapter<FeedingDevice> {
    private LayoutInflater inflater;
    private ConnectionInfo connectionInfo;
    private EspUtil espUtil;
    private Context context;

    public FeedingDeviceAdapter(Context context, List<FeedingDevice> dataList,ConnectionInfo connectionInfo) {
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

        FeedingDevice data = getItem(position);

        if (data != null) {
            holder.feedingIdTextView.setText(String.valueOf(data.getId()));
            holder.feedingNameTextView.setText(data.getName());
        }
        final Switch deviceSwitch = holder.deviceSwitch;
        if (data != null) {
            deviceSwitch.setChecked(data.isSwitchChecked());
        }
        deviceSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                data.setSwitchChecked(isChecked);
                String message="{status:2,detail:{device:"+data.getId()+"}}";
                espUtil.sendMessage(message,context,false);
            }
        });
        return convertView;
    }


}
