package cqjtu.afs_mobile.util;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;

import cqjtu.afs_mobile.R;
import cqjtu.afs_mobile.entity.FeedingDevice;

public class FeedingDeviceSpinnerAdapter extends ArrayAdapter<FeedingDevice> {

    public FeedingDeviceSpinnerAdapter(Context context, List<FeedingDevice> devices) {
        super(context, 0, devices);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        return createView(position, convertView, parent);
    }

    @Override
    public View getDropDownView(int position, View convertView, ViewGroup parent) {
        return createView(position, convertView, parent);
    }

    private View createView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.item_device_spinner, parent, false);
        }

        TextView textView = convertView.findViewById(R.id.deviceName);
        FeedingDevice device = getItem(position);

        if (device != null) {
            textView.setText(device.getName());
            textView.setTag(device.getId()); // 将设备ID作为标签设置
        }

        return convertView;
    }
}
