package cqjtu.afs_mobile.util;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

import cqjtu.afs_mobile.R;
import cqjtu.afs_mobile.entity.FeedingDevice;

public class FeedingDeviceAdapter extends RecyclerView.Adapter<ViewHolder> {

    private List<FeedingDevice> dataList;
    private LayoutInflater inflater;

    public FeedingDeviceAdapter(Context context, List<FeedingDevice> dataList) {
        this.inflater = LayoutInflater.from(context);
        this.dataList = dataList;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = inflater.inflate(R.layout.item_feeding_device, parent, false);
        return new ViewHolder(view);
    }


    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        FeedingDevice data = dataList.get(position);
        holder.bind(data);
    }

    @Override
    public int getItemCount() {
        return dataList.size();
    }


}
