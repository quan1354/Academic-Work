package com.example.assignment.database;

import android.content.Context;
import android.net.Uri;

import androidx.annotation.NonNull;
import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;
import androidx.room.TypeConverters;
import androidx.sqlite.db.SupportSQLiteDatabase;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
/**
 * A database to store food information and global method to access database
 */
@Database(entities = {FoodTable.class}, version = 1, exportSchema = false)
@TypeConverters({Convert.class})
public abstract class FoodRoomDatabase extends RoomDatabase {
    public abstract FoodDao foodDao();
    private static volatile FoodRoomDatabase INSTANCE;
    private static final int NUMBER_OF_THREADS =4;
    public static final ExecutorService databaseWriteExecutor = Executors.newFixedThreadPool(NUMBER_OF_THREADS);
    public static FoodRoomDatabase getDatabase(final Context context) {
        if (INSTANCE == null) {
            synchronized (FoodRoomDatabase.class) {
                if (INSTANCE == null) {
                    // Create database here
                    INSTANCE = Room.databaseBuilder(context.getApplicationContext(),
                            FoodRoomDatabase.class, "food_database")
                            //.addTypeConverter(Convert)
                            .fallbackToDestructiveMigration()
                            .addCallback(sRoomDatabaseCallback)
                            .build();
                }
            }
        }
        return INSTANCE;
    }

    private static RoomDatabase.Callback sRoomDatabaseCallback =
            new RoomDatabase.Callback(){
                @Override
                public void onOpen (@NonNull SupportSQLiteDatabase db){
                    super.onOpen(db);
                    databaseWriteExecutor.execute(()->{
                        FoodDao dao = INSTANCE.foodDao();
                        dao.clear();
                        FoodTable food = new FoodTable("Tom Yam Mee","noodle","12-12-12","3.40pm","noodle","Quan",4, Uri.parse("https://img-global.cpcdn.com/recipes/d10f0f75e60aa8a2/680x482cq70/mee-tom-yam-resipi-foto-utama.jpg"),0);
                        FoodTable food2 = new FoodTable("Ramen","noodle","17-12-12","5.50pm","hello","Quan",4, Uri.parse("https://us.123rf.com/450wm/taa22/taa222006/taa22200600744/150449075-japanese-tonkotsu-ramen.jpg?ver=6"),0);
                        FoodTable food3 = new FoodTable("Laksa","noodle","17-12-12","5.50pm","hello","Quan",2, Uri.parse("https://s3media.freemalaysiatoday.com/wp-content/uploads/2021/05/Penang-Laksa-pinterest1.jpg"),0);
                        FoodTable food4 = new FoodTable("Maggie Soup","noodle","17-12-12","5.50pm","hello","Quan",2, Uri.parse("https://www.newmalaysiankitchen.com/wp-content/uploads/2017/05/Easy-Maggi-Goreng-5-Ingredients.jpg"),90);

                        dao.insert(food);
                        dao.insert(food2);
                        dao.insert(food3);
                        dao.insert(food4);
                    });
                }
            };
}
