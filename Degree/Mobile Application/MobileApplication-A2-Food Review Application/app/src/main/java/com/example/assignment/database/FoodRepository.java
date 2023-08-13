package com.example.assignment.database;

import android.app.Application;

import androidx.lifecycle.LiveData;

import java.util.List;

// create service for database instruction
public class FoodRepository {
    private FoodDao foodDao;
    private LiveData<List<FoodTable>> allFood;
    private LiveData<List<FoodTable>> searchData;


    public FoodRepository(Application application){
        FoodRoomDatabase db = FoodRoomDatabase.getDatabase(application);
        foodDao = db.foodDao();
        allFood = foodDao.getAllFood();
    }

    public LiveData<List<FoodTable>> getAllFood(){return allFood;}
    public void insert (FoodTable food){
        FoodRoomDatabase.databaseWriteExecutor.execute(()->{
            foodDao.insert(food);
        });
    }
    public void delete (FoodTable food){
        FoodRoomDatabase.databaseWriteExecutor.execute(()->{
            foodDao.delete(food);
        });
    }
//    public LiveData<List<FoodTable>> search (String query){
//        FoodRoomDatabase.databaseWriteExecutor.execute(()->{
//            this.searchData = foodDao.search(query);
//        });
//        return searchData;
//    }
    public void clear(){
        FoodRoomDatabase.databaseWriteExecutor.execute(()->{
            foodDao.clear();
        });
    }
}
