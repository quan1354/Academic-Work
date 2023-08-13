package com.example.assignment1;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.example.assignment1.R;


public class MainActivity extends AppCompatActivity {
    private EditText height,weight,age,height_inch,weight_lbs;
    private CheckBox male,female;
    private Spinner spinner;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        spinner = findViewById(R.id.calSelection);
        height = findViewById(R.id.height_kgAndft);
        weight = findViewById(R.id.weight_cmAndst);
        height_inch = findViewById(R.id.height_Ins);
        weight_lbs = findViewById(R.id.weight_lbs);
        age = findViewById(R.id.age);
        Button btnCompute = findViewById(R.id.compute);
        Button clear = findViewById(R.id.clear);
        male = findViewById(R.id.male);
        female = findViewById(R.id.female);
        TextView lbsLabel = findViewById(R.id.lbsLabel);
        TextView inLabel = findViewById(R.id.InsLabel);
        btnCompute.setOnClickListener(view -> {
            String heightInput,weightInput,ageInput,inchInput,lbsInput;
            boolean isValid = true;
            heightInput = height.getText().toString().trim();
            weightInput = weight.getText().toString().trim();
            ageInput = age.getText().toString().trim();
            inchInput = height_inch.getText().toString().trim();
            lbsInput = weight_lbs.getText().toString().trim();
            if (spinner.getSelectedItem().equals("Metric") && (heightInput.isEmpty() || weightInput.isEmpty() || ageInput.isEmpty())){
                Toast.makeText(MainActivity.this, "Please fill in field, can't be empty", Toast.LENGTH_SHORT).show();
            }else if(spinner.getSelectedItem().equals("Imperial") && (heightInput.isEmpty() || weightInput.isEmpty() || ageInput.isEmpty()||inchInput.isEmpty()||lbsInput.isEmpty())){
                Toast.makeText(MainActivity.this, "Please fill in field, can't be empty", Toast.LENGTH_SHORT).show();
            }else {
                if (String.valueOf(spinner.getSelectedItem()).equals("Imperial")){

                    if (Double.parseDouble(inchInput) < 0 && !inchInput.isEmpty()){
                        height_inch.setError("inch can't less than 0");
                        height_inch.requestFocus();
                        isValid = false;
                    }
                    if (Double.parseDouble(lbsInput) < 0 && !lbsInput.isEmpty()){
                        weight_lbs.setError("lbs can't less than 0");
                        weight_lbs.requestFocus();
                        isValid = false;
                    }
                }

                if (Double.parseDouble(ageInput) < 0){
                    age.setError("age can't less than 0");
                    age.requestFocus();
                    isValid = false;
                }
                if (Double.parseDouble(heightInput) < 0 ){
                    height.setError("height can't less than 0");
                    height.requestFocus();
                    isValid = false;
                }
                if (Double.parseDouble(weightInput) < 0){
                    weight.setError("weight can't less than 0");
                    weight.requestFocus();
                    isValid = false;
                }
                if (!male.isChecked() && !female.isChecked()) {
                    Toast.makeText(MainActivity.this, "Please select your gender", Toast.LENGTH_SHORT).show();
                    isValid = false;
                }
                if (isValid){
                    String calType = String.valueOf(spinner.getSelectedItem());;
                    String gender = (male.isChecked()) ? "Male" : "Female";
                    Intent intent = new Intent(this,ResultActivity.class);
                    intent.putExtra(ResultActivity.KEY_METHOD, calType);
                    intent.putExtra(ResultActivity.KEY_HEIGHT,heightInput);
                    intent.putExtra(ResultActivity.KEY_WEIGHT,weightInput);
                    intent.putExtra(ResultActivity.KEY_AGE, ageInput);
                    intent.putExtra(ResultActivity.KEY_GENDER,gender);
                    intent.putExtra(ResultActivity.KEY_INCH, inchInput);
                    intent.putExtra(ResultActivity.KEY_LBS,lbsInput);
                    startActivity(intent);
                }
            }
            hideKeyboard();
        });
        male.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    female.setChecked(false);
                }
            }
        });
        female.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    male.setChecked(false);
                }
            }
        });
        TextView labelCM = findViewById(R.id.kgAndftLabel);
        TextView labelKG = findViewById(R.id.cmAndstLabel);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                switch (position) {
                    case 0:
                        height_inch.setVisibility(View.INVISIBLE);
                        weight_lbs.setVisibility(View.INVISIBLE);
                        lbsLabel.setVisibility(View.INVISIBLE);
                        inLabel.setVisibility(View.INVISIBLE);
                        labelCM.setText("CM");
                        labelKG.setText("KG");
                        break;
                    case 1:
                        height_inch.getText().clear();
                        weight_lbs.getText().clear();
                        height_inch.setVisibility(View.VISIBLE);
                        weight_lbs.setVisibility(View.VISIBLE);
                        lbsLabel.setVisibility(View.VISIBLE);
                        inLabel.setVisibility(View.VISIBLE);
                        labelCM.setText("ft");
                        labelKG.setText("st");
                        break;
                }
            }
            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            }
        });
        clear.setOnClickListener(view -> {
            clearInputFields();
        });
    }
    private void hideKeyboard() {
        View view = this.getCurrentFocus();
        if (view != null) {
            InputMethodManager manager  = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
            manager.hideSoftInputFromWindow(view.getWindowToken(), 0);
        }
    }
    private void clearInputFields() {
        height.getText().clear();
        weight.getText().clear();
        height_inch.getText().clear();
        weight_lbs.getText().clear();
        age.getText().clear();
        male.setChecked(false);
        female.setChecked(false);
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString("UNIT", String.valueOf(spinner.getSelectedItem()));
        outState.putString("HEIGHT",height.getText().toString().trim());
        outState.putString("WEIGHT",weight.getText().toString().trim());
        outState.putString("HEIGHT_INCH",height_inch.getText().toString().trim());
        outState.putString("WEIGHT_LBS",weight_lbs.getText().toString().trim());
        outState.putString("AGE",age.getText().toString().trim());
        outState.putBoolean("MALE",male.isChecked());
        outState.putBoolean("FEMALE",female.isChecked());
    }

    @Override
    protected void onRestoreInstanceState(@NonNull Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        if (savedInstanceState.getString("UNIT","").equalsIgnoreCase("Metric")){
            spinner.setSelection(0);
        }else {
            spinner.setSelection(1);
        }
        height.setText(savedInstanceState.getString("HEIGHT",""));
        weight.setText(savedInstanceState.getString("WEIGHT",""));
        height_inch.setText(savedInstanceState.getString("HEIGHT_INCH",""));
        weight_lbs.setText(savedInstanceState.getString("WEIGHT_LBS",""));
        weight_lbs.setText(savedInstanceState.getString("WEIGHT_LBS",""));
        age.setText(savedInstanceState.getString("AGE",""));
        male.setChecked(savedInstanceState.getBoolean("MALE"));
        male.setChecked(savedInstanceState.getBoolean("FEMALE"));
    }
}