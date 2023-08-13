package com.example.assignment1;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Arrays;

public class ResultActivity extends AppCompatActivity {
    public static final String KEY_METHOD = "METHOD";
    public static final String KEY_HEIGHT = "HEIGHT";
    public static final String KEY_WEIGHT = "WEIGHT";
    public static final String KEY_AGE = "AGE";
    public static final String KEY_GENDER = "GENDER";
    public static final String KEY_INCH = "INCH";
    public static final String KEY_LBS = "LBS";
    ResultModelView viewModel = null;
    @SuppressLint("DefaultLocale")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);
        TextView method = findViewById(R.id.showMethod);
        TextView height = findViewById(R.id.showHeight);
        TextView weight = findViewById(R.id.showWeight);
        TextView ageGroup = findViewById(R.id.showAgeGroup);
        TextView gender = findViewById(R.id.showGender);
        TextView result = findViewById(R.id.resultLabel);
        Button btnAgain = findViewById(R.id.button_again);
        TextView calculation = findViewById(R.id.calculation);

        Intent intent = getIntent();
        String methodGet = intent.getStringExtra(KEY_METHOD);
        String heightGet = intent.getStringExtra(KEY_HEIGHT);
        String weightGet = intent.getStringExtra(KEY_WEIGHT);
        String ageGet = intent.getStringExtra(KEY_AGE);
        String genderGet = intent.getStringExtra(KEY_GENDER);
        String inch = intent.getStringExtra(KEY_INCH);
        String lbs = intent.getStringExtra(KEY_LBS);
        viewModel = new ViewModelProvider(this).get(ResultModelView.class);
        viewModel.setCalMethod(methodGet);
        viewModel.setInch(inch);
        viewModel.setLbs(lbs);
        viewModel.setHeight(heightGet);
        viewModel.setWeight(weightGet);
        viewModel.setAge(ageGet);
        viewModel.setGender(genderGet);
        viewModel.compute();
        viewModel.defineAgeGroup();

        ImageView image = findViewById(R.id.resultPicture);
        String [] lowerWeight = {"underweight","very severely underweight","severely underweight"};
        String [] healthy = {"healthy","normal"};
        String [] overWeight = {"overweight","mode moderately"};
        String [] obesity = {"obesity","very severely obese","severely obese"};
        if (Arrays.asList(lowerWeight).contains(viewModel.getStatus().toLowerCase())){
            image.setImageResource(R.drawable.lowerweight);
        }else if(Arrays.asList(healthy).contains(viewModel.getStatus().toLowerCase())){
            image.setImageResource(R.drawable.healthy);
        }else if(Arrays.asList(overWeight).contains(viewModel.getStatus().toLowerCase())){
            image.setImageResource(R.drawable.overweight);
        }else if(Arrays.asList(obesity).contains(viewModel.getStatus().toLowerCase())){
            image.setImageResource(R.drawable.obesity);
        }else{
            image.setImageResource(R.drawable.spongebob);
        }


        method.setText(getString(R.string.method_s,viewModel.getCalMethod()));
        height.setText(getString(R.string.height_s,viewModel.getHeight()));
        weight.setText(getString(R.string.weight_s,viewModel.getWeight()));
        ageGroup.setText(getString(R.string.age_group_s,viewModel.getAgeGroup()));
        gender.setText(getString(R.string.gender_s,viewModel.getGender()));
        result.setText(viewModel.getStatus());
        calculation.setText(getString(R.string.calculation_s,String.format("%.2f",viewModel.getResult())));

        btnAgain.setOnClickListener(view -> {
            Intent intent1 = new Intent(this,MainActivity.class);
            startActivity(intent1);
        });
    }


}