package com.example.assignment.database;

import android.net.Uri;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

// Table Information
@Entity(tableName = "FoodTable")
public class FoodTable {
    // auto generate unique ID
    @PrimaryKey(autoGenerate = true)
    private long foodId = 0L;
    // Food attribute
    @ColumnInfo(name = "name")
    private String name;
    @ColumnInfo(name = "group")
    private String group;
    @ColumnInfo(name = "date")
    private String date;
    @ColumnInfo(name = "time")
    private String time;
    @ColumnInfo(name = "description")
    private String description;
    @ColumnInfo(name = "reporter")
    private String reporter;
    @ColumnInfo(name = "rating")
    private float rating;
    @ColumnInfo(name = "imageUri")
    private Uri imageUri;
    @ColumnInfo(name = "imageAngle")
    private float imageAngle;

    public FoodTable(){
        this.name = "";
        this.group = "";
        this.date = "";
        this.time = "";
        this.description = "";
        this.reporter = "";
        this.rating = 0.f;
        this.imageUri = null;
        this.imageAngle = 0.f;
    }

    public FoodTable(String name, String group, String date,String time, String description,String reporter,float rating,Uri imageUri,float imageAngle){
        this.name = name;
        this.group = group;
        this.date = date;
        this.time = time;
        this.description = description;
        this.reporter = reporter;
        this.rating = rating;
        this.imageUri = imageUri;
        this.imageAngle = imageAngle;
    }


    //getter
    public String getName() { return name; }
    public String getGroup() { return group; }
    public String getTime() { return time; }
    public String getReporter() { return reporter; }
    public String getDate() { return date; }
    public float getRating() { return rating; }
    public String getDescription() { return description; }
    public float getImageAngle() { return imageAngle; }
    public long getFoodId() {return foodId; }
    public Uri getImageUri() { return imageUri; }
    //setter
    public void setGroup(String group) { this.group = group; }
    public void setReporter(String reporter) { this.reporter = reporter; }
    public void setRating(float rating) { this.rating = rating; }
    public void setDate(String date) { this.date = date; }
    public void setDescription(String description) { this.description = description; }
    public void setName(String name) { this.name = name; }
    public void setFoodId(long foodId) { this.foodId = foodId; }
    public void setImageAngle(float imageAngle) { this.imageAngle = imageAngle; }
    public void setImageUri(Uri imageUri) { this.imageUri = imageUri; }
    public void setTime(String time) { this.time = time; }
}


