package com.example.assignment;

import android.app.Application;

import androidx.annotation.NonNull;
import androidx.lifecycle.ViewModel;
import androidx.lifecycle.ViewModelProvider;

import com.example.assignment.database.FoodDao;

import kotlin.Suppress;


public class AddFoodViewModelFactory implements ViewModelProvider.Factory {
    private FoodDao dataSource;
    private Application application;

    AddFoodViewModelFactory(FoodDao foodDao, Application application){
        this.dataSource = foodDao;
        this.application = application;
    }

    @Suppress(names = "unchecked_cast")
    @NonNull
    @Override
    public <T extends ViewModel> T create(@NonNull Class<T> modelClass) {
        if (modelClass.isAssignableFrom(AddFoodViewModel.class)) {
            return (T) new AddFoodViewModel(dataSource, application);
        }else{
            throw new IllegalArgumentException("Unknown ViewModel class");
        }
    }
}