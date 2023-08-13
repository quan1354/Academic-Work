package com.example.assignment;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.DiffUtil;
import androidx.recyclerview.widget.ListAdapter;
import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.example.assignment.database.FoodTable;
import com.example.assignment.databinding.MyrowBinding;

public class FoodAdapter extends ListAdapter<FoodTable,FoodAdapter.MyViewHolder>  {
    public Context context;
    public ClickEvent clickEvent;

    // Handle Click Event
    public interface ClickEvent{
        void onCLickListener(FoodTable foodTable);
        void onClickDeleteFood(FoodTable foodTable);
    }

    // Constructor
    public FoodAdapter(Context ct, ClickEvent clickEvent){
        super(new FoodDiffCallback());
        this.clickEvent = clickEvent;
        this.context = ct;
    }

    @NonNull
    @Override
    // To bind layout
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return MyViewHolder.from(parent);
    }

    @Override
    // assign data to view holder class constructor
    public void onBindViewHolder(@NonNull MyViewHolder holder, int position) {
        holder.bind(getItem(position),clickEvent,context);
    }

    //Create holder for view
    public static class MyViewHolder extends RecyclerView.ViewHolder {
        MyrowBinding binding;
        public MyViewHolder(MyrowBinding binding){
            super(binding.getRoot());
            this.binding = binding;
        }

        // assign data to layout
        public void bind(FoodTable food, ClickEvent clickEvent,Context context){
            binding.placerText.setText(food.getName());
            if(food.getImageUri()!=null){
                Glide.with(context)
                        .asBitmap()
                        .load(food.getImageUri())
                        .transform(new BitmapRotation(food.getImageAngle()))
                        .diskCacheStrategy(DiskCacheStrategy.DATA)
                        .into(binding.placerImage);
            }
            binding.hot.setVisibility(food.getRating() > 3 ? View.VISIBLE : View.INVISIBLE);
            binding.item.setOnClickListener(view->clickEvent.onCLickListener(food));
            binding.deleteBtn.setOnClickListener(view->clickEvent.onClickDeleteFood(food));
            binding.executePendingBindings();
        }

        // set up data binding
        public static MyViewHolder from(ViewGroup parent){
               LayoutInflater layoutInflater = LayoutInflater.from(parent.getContext());
               MyrowBinding binding = MyrowBinding.inflate(layoutInflater, parent,false);
               return new MyViewHolder(binding);
        }
    }
}

// Tell recycle view what changed, instead of go modify too much thing
class FoodDiffCallback extends DiffUtil.ItemCallback<FoodTable> {
    @Override
    public boolean areItemsTheSame(@NonNull FoodTable oldItem, @NonNull FoodTable newItem) {
        return oldItem.getName().equals(newItem.getName());
    }

    @Override
    public boolean areContentsTheSame(@NonNull FoodTable oldItem, @NonNull FoodTable newItem) {
        return oldItem == newItem;
    }
}





