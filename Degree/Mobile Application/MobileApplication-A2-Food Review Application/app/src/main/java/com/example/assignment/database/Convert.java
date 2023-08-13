package com.example.assignment.database;

import android.net.Uri;
import androidx.room.TypeConverter;
// For let database know convert between Url and String for storage purpose
public class Convert {
    @TypeConverter
    public static String uriToString(Uri image) {
        return image == null ? null : image.toString();
    }
    @TypeConverter
    public static Uri stringToUri(String image) {
        return image == null ? null : Uri.parse(image);
    }
}
