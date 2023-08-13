package com.example.assignment;

import androidx.annotation.NonNull;
import androidx.lifecycle.ViewModel;
import androidx.lifecycle.ViewModelProvider;

import com.example.assignment.database.FoodDao;

import kotlin.Suppress;

public class FoodDetailViewModelFactory implements ViewModelProvider.Factory {
    private FoodDao foodDao;

    FoodDetailViewModelFactory(FoodDao foodDao){
        this.foodDao = foodDao;
    }

    @Suppress(names = "unchecked_cast")
    @NonNull
    @Override
    public <T extends ViewModel> T create(@NonNull Class<T> modelClass) {
        if (modelClass.isAssignableFrom(FoodDetailViewModel.class)) {
            return (T) new FoodDetailViewModel(foodDao);
        }else{
            throw new IllegalArgumentException("Unknown ViewModel class");
        }
    }
}