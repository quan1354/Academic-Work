package com.example.assignment.database;

import android.net.Uri;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Update;

import java.util.List;


@Dao
public interface FoodDao {
    // operation for record
    @Insert
    void insert(FoodTable food);
    @Update
    void update(FoodTable food);
    @Delete
    void delete(FoodTable food);

    //Delete all food
    @Query("DELETE FROM FoodTable")
    void clear();
    //Get all record in ascending order
    @Query("SELECT * FROM FoodTable ORDER BY name ASC")
    LiveData<List<FoodTable>> getAllFood();
    //Select food base on id
    @Query("SELECT * FROM FoodTable WHERE foodId = :id")
    FoodTable getFoodWithID(long id);
    //For search food with query
    @Query("SELECT * FROM FoodTable WHERE name LIKE :searchQuery")
    LiveData<List<FoodTable>> search(String searchQuery);
}
