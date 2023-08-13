package com.example.assignment;

import android.app.Application;

import androidx.annotation.NonNull;
import androidx.lifecycle.ViewModel;
import androidx.lifecycle.ViewModelProvider;

import com.example.assignment.database.FoodDao;

import kotlin.Suppress;

public class FoodViewModelFactory implements ViewModelProvider.Factory {
    private FoodDao dataSource;
    private Application application;
    private  String label;

    FoodViewModelFactory(FoodDao foodDao, Application application,String label){
        this.dataSource = foodDao;
        this.application = application;
        this.label = label;
    }

    @Suppress(names = "unchecked_cast")
    @NonNull
    @Override
    public <T extends ViewModel> T create(@NonNull Class<T> modelClass) {
        if (modelClass.isAssignableFrom(FoodViewModel.class)) {
            return (T) new FoodViewModel(dataSource, application, label);
        }else{
            throw new IllegalArgumentException("Unknown ViewModel class");
        }
    }
}
