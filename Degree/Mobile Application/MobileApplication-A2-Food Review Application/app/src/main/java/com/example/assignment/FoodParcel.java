package com.example.assignment;
import android.net.Uri;
import android.os.Parcel;
import android.os.Parcelable;

import com.example.assignment.database.FoodTable;

// Parcel class for navigation, can transfer food object
public class FoodParcel extends FoodTable implements Parcelable {
    long id;

    public FoodParcel(){
        this.id = 0L;
    }
    public FoodParcel(long id,String name, String group, String date,String time, String description,String reporter,float rating,Uri imageUri,float imageAngle){
        super(name,group,date,time,description,reporter,rating,imageUri,imageAngle);
        this.id = id;
    }

    public long getId() { return id; }
    public void setId(long id) { this.id = id; }

    protected FoodParcel(Parcel in) {
    }

    public static final Creator<FoodParcel> CREATOR = new Creator<FoodParcel>() {
        @Override
        public FoodParcel createFromParcel(Parcel in) {
            return new FoodParcel(in);
        }

        @Override
        public FoodParcel[] newArray(int size) {
            return new FoodParcel[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
    }
}
