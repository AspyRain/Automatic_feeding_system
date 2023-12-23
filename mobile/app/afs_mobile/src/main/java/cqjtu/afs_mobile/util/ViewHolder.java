package cqjtu.afs_mobile.util;

import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import cqjtu.afs_mobile.R;
import cqjtu.afs_mobile.entity.FeedingDevice;

public class ViewHolder extends RecyclerView.ViewHolder {
    TextView feedingIdTextView;
    TextView feedingNameTextView;

    public ViewHolder(@NonNull View itemView) {
        super(itemView);
        feedingIdTextView = itemView.findViewById(R.id.feeding_id);
        feedingNameTextView = itemView.findViewById(R.id.feeding_name);
    }

    public void bind(FeedingDevice data) {
        // 将数据绑定到视图上
        feedingIdTextView.setText((int) data.getId());
        feedingNameTextView.setText(data.getName());
    }
}
