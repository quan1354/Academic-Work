package com.example.assignment;

import android.net.Uri;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.assignment.database.FoodDao;
import com.example.assignment.database.FoodRoomDatabase;
import com.example.assignment.database.FoodTable;

// Data use for Food detail fragment.
public class FoodDetailViewModel extends ViewModel {
    private FoodDao foodDao;
    public MutableLiveData<Long> id = new MutableLiveData<>(0L);
    public MutableLiveData<String> name = new MutableLiveData<>("");
    public MutableLiveData<String> group = new MutableLiveData<>("");
    public MutableLiveData<String> date = new MutableLiveData<>("");
    public MutableLiveData<String> time = new MutableLiveData<>("");
    public MutableLiveData<String> description = new MutableLiveData<>("");
    public MutableLiveData<String> reporter = new MutableLiveData<>("");
    public MutableLiveData<Float> rating = new MutableLiveData<>(0.f);
    public MutableLiveData<Uri> imageUri = new MutableLiveData<>();
    public MutableLiveData<Float> imageAngle = new MutableLiveData<>(0.f);
    public FoodTable food;

    public FoodDetailViewModel(FoodDao foodDao) { this.foodDao = foodDao; }

    public void setFood(FoodTable food) {
        this.food = food;
        id.setValue(food.getFoodId());
        name.setValue(food.getName());
        group.setValue(food.getGroup());
        date.setValue(food.getDate());
        time.setValue(food.getTime());
        description.setValue(food.getDescription());
        reporter.setValue(food.getReporter());
        rating.setValue(food.getRating());
        imageUri.setValue(food.getImageUri());
        imageAngle.setValue(food.getImageAngle());
    }


    public FoodTable getFood() {
        return food;
    }
    public float getRotateAngle(){return imageAngle.getValue();}

}


