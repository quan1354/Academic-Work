package com.example.assignment;

import android.app.Application;

import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.MutableLiveData;
// Value Use for main activity.getter and setter.
public class MainViewModel extends AndroidViewModel {
    public MutableLiveData<Boolean> isNavigateWelcome = new MutableLiveData<>(false);
    public MutableLiveData<Long> foodID = new MutableLiveData<>(-1L);
    public MutableLiveData<Boolean> isDualPane = new MutableLiveData<>(false);
    public MutableLiveData<Integer> orientation = new MutableLiveData<>();

    public MainViewModel(Application application){
        super(application);
    }
    public void isNavigateWelcomeScreen(){
        isNavigateWelcome.setValue(true);
    }

    public void setFoodID(long id) {
        foodID.setValue(id);
    }
    public long getFoodID(){
        return foodID.getValue();
    }

    public void setOrientation(int orient){orientation.setValue(orient);}

    public void setIsDualPane(boolean dualPane){isDualPane.setValue(dualPane);}
    public boolean getIsDualPane(){return isDualPane.getValue();}
}
