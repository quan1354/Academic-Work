package com.example.assignment;

import android.app.Application;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.assignment.database.FoodDao;
import com.example.assignment.database.FoodRepository;
import com.example.assignment.database.FoodRoomDatabase;
import com.example.assignment.database.FoodTable;

import java.util.List;
// Data Use for food fragment
public class FoodViewModel extends ViewModel {
    private final FoodDao foodDao;
    private final Application application;
    private final FoodRepository foodRepository;
    private final String actionLabel;
    public LiveData<List<FoodTable>> allFood;
    public MutableLiveData<Boolean> showSnackBarEvent = new MutableLiveData<>(false);

    public FoodViewModel (FoodDao foodDao,Application application,String actionLabel){
        foodRepository = new FoodRepository(application);
        this.foodDao = foodDao;
        this.application = application;
        this.actionLabel = actionLabel;
        this.allFood = foodRepository.getAllFood();
    }

    // clear all data
    public void onClear() { foodRepository.clear();showSnackBarEvent.setValue(true); }
    // delete selected food
    public void deleteFood(FoodTable foodTable){ foodRepository.delete(foodTable); }
    // show info when all data deleted
    public void doneShowingSnackBar() { showSnackBarEvent.setValue(false);}
    // return result for search
    public LiveData<List<FoodTable>> search(String query){ return foodDao.search(query); }

}
