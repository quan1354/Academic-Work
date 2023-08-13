package com.example.assignment;

import android.app.Application;
import android.content.Context;
import android.icu.text.SimpleDateFormat;
import android.net.Uri;

import androidx.annotation.NonNull;
import androidx.core.content.FileProvider;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.assignment.database.FoodDao;
import com.example.assignment.database.FoodRepository;
import com.example.assignment.database.FoodRoomDatabase;
import com.example.assignment.database.FoodTable;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.util.Date;
import java.util.Objects;

// Data use for form
public class AddFoodViewModel extends ViewModel {
    private boolean canShowNameError = false;
    private boolean canShowDateError = false;
    private boolean canShowTimeError = false;
    private boolean canShowReporterError = false;
    private boolean canShowGroupError = false;
    private final Application application;
    private final FoodRepository foodRepository;
    private final FoodDao foodDao;
    private long foodID;
    private FoodTable food;
    private String event = "";

    public final MutableLiveData<String> name = new MutableLiveData<>("");
    public final MutableLiveData<String> group = new MutableLiveData<>("");
    public final MutableLiveData<String> date = new MutableLiveData<>("");
    public final MutableLiveData<String> time = new MutableLiveData<>("");
    public final MutableLiveData<String> description = new MutableLiveData<>("");
    public final MutableLiveData<String> reporter = new MutableLiveData<>("");
    public final MutableLiveData<Float> rating = new MutableLiveData<>(0.f);
    private final MutableLiveData<Uri> imageUri = new MutableLiveData<>();
    private final MutableLiveData<Float> imageAngle = new MutableLiveData<>(0.f);
    private final MutableLiveData<Boolean> _hasError = new MutableLiveData<>(false);
    private final MutableLiveData<Boolean> _canAdd = new MutableLiveData<>(false);
    private Uri tempPictureUri;

    public AddFoodViewModel (FoodDao foodDao, Application application){
        this.foodDao = foodDao;
        this.application = application;
        this.foodRepository = new FoodRepository(application);
    }

    //getter
    public LiveData<Boolean> hasError() { return _hasError; }
    public LiveData<Boolean> canAdd() {
        return _canAdd;
    }
    public LiveData<Uri> getImageUri() {
        return imageUri;
    }
    public LiveData<Float> getImageAngle() {
        return imageAngle;
    }
    public Uri getTempPictureUri() {
        return tempPictureUri;
    }

    //clear form
    public void reset() {
        canShowNameError = false;
        canShowDateError = false;
        canShowGroupError = false;
        canShowTimeError = false;
        canShowReporterError = false;
        name.setValue("");
        group.setValue("");
        reporter.setValue("");
        date.setValue("");
        time.setValue("");
        description.setValue("");
        rating.setValue(0.f);
        imageUri.setValue(null);
        imageAngle.setValue(0.f);
    }

    //check require field
    public boolean shouldShowNameError() {
        if (!Objects.equals(name.getValue(), "")) canShowNameError = true;
        return Objects.equals(name.getValue(), "") && canShowNameError;
    }

    public boolean shouldShowFoodGroupError() {
        if (!Objects.equals(group.getValue(), "")) canShowGroupError = true;
        return Objects.equals(group.getValue(), "") && canShowGroupError;
    }

    public boolean shouldShowDateError() {
        if (!Objects.equals(date.getValue(), "")) canShowDateError = true;
        return Objects.equals(date.getValue(), "") && canShowDateError;
    }

    public boolean shouldShowTimeError() {
        if (!Objects.equals(time.getValue(), "")) canShowTimeError = true;
        return Objects.equals(time.getValue(), "") && canShowTimeError;
    }

    public boolean shouldShowReporterError() {
        if (!Objects.equals(reporter.getValue(), "")) canShowReporterError = true;
        return Objects.equals(reporter.getValue(), "") && canShowReporterError;
    }

    // set image detail
    public void changeImage(final Uri uri) {
        imageUri.setValue(uri);
        imageAngle.setValue(0.f);
    }

    // file provider for image
    public Uri makeTempPictureUri(final Context context) {
        tempPictureUri = FileProvider.getUriForFile(
                context,
                "com.example.fileprovider",
                new File(
                        context.getCacheDir(),
                        String.format(
                                "%s.jpg", SimpleDateFormat.getDateTimeInstance().format(new Date())
                        )
                )
        );
        return tempPictureUri;
    }

    // control angle rotation
    public void rotateImage() {
        imageAngle.setValue((imageAngle.getValue() == null ? 0 : imageAngle.getValue()) + 90);
    }

    // For modify usage
    public void setFoodID(long foodID) {
        this.foodID = foodID;
    }
    public long getFoodID() { return foodID; }
    public void setEvent(String event) { this.event = event; }
    public String getEvent() { return event; }

    // check validation
    public void add() {
        canShowNameError = true;
        canShowDateError = true;
        canShowTimeError = true;
        canShowReporterError = true;
        canShowGroupError = true;

        // These lines of code just solely for triggering AddFoodFragment observers
        name.setValue(name.getValue());
        date.setValue(date.getValue());
        time.setValue(time.getValue());
        group.setValue(group.getValue());
        reporter.setValue(reporter.getValue());

        if (shouldShowNameError() || shouldShowDateError() || shouldShowTimeError() || shouldShowReporterError() || shouldShowFoodGroupError())
            _hasError.setValue(true);
        else
            _canAdd.setValue(true);
    }

    //action after click green button,
    public void confirmAdd(@NonNull final Context context) throws IOException {
        if (tempPictureUri != null) {
            try (
                    final InputStream picture = context.getContentResolver().openInputStream(tempPictureUri)
            ) {
                final File pictureFile = new File(
                        context.getFilesDir(),
                        String.format(
                                "%s.jpg", SimpleDateFormat.getDateTimeInstance().format(new Date())
                        )
                );
                Files.copy(picture, pictureFile.toPath());
                imageUri.setValue(
                        FileProvider.getUriForFile(
                                context, "com.example.fileprovider", pictureFile
                        )
                );
            }
        }
        // Add action
        if(foodID == 0L){
            food = new FoodTable(name.getValue(), Objects.requireNonNull(group.getValue()).toLowerCase(),date.getValue(),time.getValue(),description.getValue(),reporter.getValue(),rating.getValue(), imageUri.getValue(),imageAngle.getValue());
            foodRepository.insert(food);
        }else {
            // Update Action
            FoodRoomDatabase.databaseWriteExecutor.execute(()->{
                 food= foodDao.getFoodWithID(foodID);
                 food.setName(name.getValue());
                 food.setGroup(Objects.requireNonNull(group.getValue()).toLowerCase());
                 food.setDate(date.getValue());
                 food.setTime(time.getValue());
                 food.setDescription(description.getValue());
                 food.setReporter(reporter.getValue());
                 food.setRating(rating.getValue());
                 food.setImageUri(imageUri.getValue());
                 food.setImageAngle(imageAngle.getValue());
                 foodDao.update(food);
            });
        }
    }

    // retrieve back original data (modify)
    public void takeFoodValue(long foodID){
        FoodRoomDatabase.databaseWriteExecutor.execute(()->{
            food= foodDao.getFoodWithID(foodID);
            name.postValue(food.getName());
            group.postValue(food.getGroup());
            date.postValue(food.getDate());
            time.postValue(food.getTime());
            description.postValue(food.getDescription());
            reporter.postValue(food.getReporter());
            rating.postValue(food.getRating());
            imageUri.postValue(food.getImageUri());
            imageAngle.postValue(food.getImageAngle());
        });
    }

    public void doneError() {
        _hasError.setValue(false);
    }
    public void doneAdd() {
        _canAdd.setValue(false);
    }
}
