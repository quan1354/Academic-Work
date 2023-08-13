package com.example.assignment;

import android.app.Application;
import android.content.Context;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.databinding.DataBindingUtil;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.example.assignment.database.FoodDao;
import com.example.assignment.database.FoodRoomDatabase;
import com.example.assignment.database.FoodTable;
import com.example.assignment.databinding.FragmentFoodDetailBinding;

public class FoodDetailFragment extends Fragment {
    ClickEvent clickEvent;
    FragmentFoodDetailBinding binding;
    FoodDetailViewModel foodDetailViewModel;
    FoodParcel foodParcel;
    @Override
    public View onCreateView(@NonNull final LayoutInflater inflater, final ViewGroup container, final Bundle savedInstanceState) {
        // associated view model
        Application application = requireActivity().getApplication();
        FoodDao foodDao = FoodRoomDatabase.getDatabase(application).foodDao();
        FoodDetailViewModelFactory foodDetailViewModelFactory = new FoodDetailViewModelFactory(foodDao);
        foodDetailViewModel = new ViewModelProvider(this, foodDetailViewModelFactory).get(FoodDetailViewModel.class);

        // get food data from parcel of navigation
        if(getArguments() != null){
            foodParcel = FoodDetailFragmentArgs.fromBundle(getArguments()).getParcel();
            changeFood(foodParcel);
        }

        // bind data
        binding = DataBindingUtil.inflate(inflater,R.layout.fragment_food_detail,container,false);
        binding.setLifecycleOwner(getViewLifecycleOwner());
        binding.setFoodDetailViewModel(foodDetailViewModel);

        // move to form when user want modify details
        binding.modifyBtn.setOnClickListener(view ->{
            clickEvent.ModifyActionBtn();
        });

        // share food action
        binding.shareBtn.setOnClickListener(view->{
            clickEvent.shareItem(foodDetailViewModel.getFood());
        });

        //For change image
        foodDetailViewModel.imageUri.observe(getViewLifecycleOwner(), value ->{
            Glide.with(this)
                    .asBitmap()
                    .load(value)
                    .transform(new BitmapRotation(foodDetailViewModel.getRotateAngle()))
                    .diskCacheStrategy(DiskCacheStrategy.DATA)
                    .into(binding.imageView);
        });


        return binding.getRoot();
    }
    @Override
    public void onAttach(@NonNull Context context) {
        try {
            if (context instanceof ClickEvent)
                clickEvent = (ClickEvent) context;
        }catch (ClassCastException e){
            throw new RuntimeException("Unable to cast context object to ClickListener");
        }
        super.onAttach(context);
    }

    // use for landscape, change detail data.
    public void changeFood(FoodTable foodTable){
        try{
            foodDetailViewModel.setFood(foodTable);
        }catch (Exception e){
            System.out.println("error");
        }
    }
}