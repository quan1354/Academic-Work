package com.example.assignment1;
import android.annotation.SuppressLint;

import androidx.lifecycle.ViewModel;

import java.util.Arrays;

public class ResultModelView extends ViewModel {
    public String calMethod;
    public String height;
    public String weight;
    public String age;
    public String gender;
    public String inch;
    public String lbs;
    public String status;
    public double result;
    public String ageGroup;

    public ResultModelView(){
        this.calMethod = null;
        this.height = "";
        this.weight = "";
        this.age="";
        this.gender = "";
        this.inch = "";
        this.lbs = "";
        this.status = "";
        this.result = 0;
        this.ageGroup = "";
    }

    public String getHeight() { return height; }
    public String getWeight() { return weight; }
    public String getAge() { return age; }
    public String getGender() { return gender; }
    public String getCalMethod() { return calMethod; }
    public String getInch() { return inch; }
    public String getLbs() { return lbs; }
    public double getResult() { return result; }
    public String getStatus() { return status; }
    public String getAgeGroup() { return ageGroup; }

    @SuppressLint("DefaultLocale")
    public void setHeight(String height) {
        if (calMethod.equals("Metric")){
            this.height = height;
        }else {
            double holder = (Double.parseDouble(height) * 12) + Double.parseDouble(inch);
            this.height = String.format("%.2f",holder);
        }
    }
    @SuppressLint("DefaultLocale")
    public void setWeight(String weight) {
        if (calMethod.equals("Metric")){
            this.weight = weight;
        }else{
            double holder = (Double.parseDouble(weight)*14) + Double.parseDouble(lbs);
            this.weight = String.format("%.2f",holder);
        }
    }
    public void setAge(String age) { this.age = age; }
    public void setGender(String gender) { this.gender = gender; }
    public void setCalMethod(String calMethod) { this.calMethod = calMethod; }
    public void setInch(String inch) { this.inch = inch; }
    public void setLbs(String lbs) { this.lbs = lbs; }
    public void setStatus(String status) { this.status = status; }
    public void setAgeGroup(String ageGroup) { this.ageGroup = ageGroup; }

    public void setResult(double result) { this.result = result; }

    public void compute(){
        double result=0.0;
        if (calMethod.equals("Metric")){
            result = Double.parseDouble(weight) / Math.pow(Double.parseDouble(height)/100,2);
        }else {
            result = (703 * ((Double.parseDouble(weight))) / (Math.pow((Double.parseDouble(height)),2)));
        }
        setResult(result);
    }
    public void defineAgeGroup(){
        int agePlacer = Integer.parseInt(age);
        String define="";
        if( agePlacer>= 2 && agePlacer < 21){
            if(gender.equalsIgnoreCase("Female")){
                switch (agePlacer){
                    case 2:
                        if(result < 14.4){
                            define = "UnderWeight";
                        }else if (result >=14.4 &&  result <= 18.0){
                            define = "Healthy";
                        }else if (result > 18.0 && result <= 19.1){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 3:
                        if(result < 13){
                            define = "UnderWeight";
                        }else if (result >=13 &&  result <= 17.0){
                            define = "Healthy";
                        }else if (result > 17.0 && result <= 18.3){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 4:
                        if(result < 13.7){
                            define = "UnderWeight";
                        }else if (result >=13.7 &&  result <= 16.8){
                            define = "Healthy";
                        }else if (result > 16.8 && result <= 18.0){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 5:
                        if(result < 13.5){
                            define = "UnderWeight";
                        }else if (result >=13.5 &&  result <= 16.8){
                            define = "Healthy";
                        }else if (result > 16.8 && result <= 18.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 6:
                        if(result < 13.4){
                            define = "UnderWeight";
                        }else if (result >=13.4 &&  result <= 17.1){
                            define = "Healthy";
                        }else if (result > 17.1 && result <= 18.3){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 7:
                        if(result < 13.4){
                            define = "UnderWeight";
                        }else if (result >=13.4 &&  result <= 17.6){
                            define = "Healthy";
                        }else if (result > 17.6 && result <= 19.6){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 8:
                        if(result < 13.5){
                            define = "UnderWeight";
                        }else if (result >=13.5 &&  result <= 18.3){
                            define = "Healthy";
                        }else if (result > 18.3 && result <= 20.6){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 9:
                        if(result < 13.8){
                            define = "UnderWeight";
                        }else if (result >=13.8 &&  result <= 19.0){
                            define = "Healthy";
                        }else if (result > 19.0 && result <= 21.8){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 10:
                        if(result < 14){
                            define = "UnderWeight";
                        }else if (result >=14 &&  result <= 19.9){
                            define = "Healthy";
                        }else if (result > 19.9 && result <= 22.9){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 11:
                        if(result < 14.4){
                            define = "UnderWeight";
                        }else if (result >=14.4 &&  result <= 20.8){
                            define = "Healthy";
                        }else if (result > 20.8 && result <= 24.1){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 12:
                        if(result < 14.8){
                            define = "UnderWeight";
                        }else if (result >=14.8 &&  result <= 21.7){
                            define = "Healthy";
                        }else if (result > 21.7 && result <= 25.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 13:
                        if(result < 15.3){
                            define = "UnderWeight";
                        }else if (result >=15.3 &&  result <= 22.5){
                            define = "Healthy";
                        }else if (result > 22.5 && result <= 26.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 14:
                        if(result < 15.8){
                            define = "UnderWeight";
                        }else if (result >=15.8 &&  result <= 23.3){
                            define = "Healthy";
                        }else if (result > 23.3 && result <= 27.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 15:
                        if(result < 16.3){
                            define = "UnderWeight";
                        }else if (result >=16.3 &&  result <= 24){
                            define = "Healthy";
                        }else if (result > 24 && result <= 28.1){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 16:
                        if(result < 16.8){
                            define = "UnderWeight";
                        }else if (result >=16.8 &&  result <= 24.6){
                            define = "Healthy";
                        }else if (result > 24.6 && result <= 28.9){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 17:
                        if(result < 17.2){
                            define = "UnderWeight";
                        }else if (result >=17.2 &&  result <= 25.2){
                            define = "Healthy";
                        }else if (result > 25.2 && result <= 29.6){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 18:
                        if(result < 17.6){
                            define = "UnderWeight";
                        }else if (result >=17.6 &&  result <= 25.7){
                            define = "Healthy";
                        }else if (result > 25.7 && result <= 30.3){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 19:
                        if(result < 17.8){
                            define = "UnderWeight";
                        }else if (result >=17.8 &&  result <= 26.1){
                            define = "Healthy";
                        }else if (result > 26.1 && result <= 31.0){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 20:
                        if(result < 17.8){
                            define = "UnderWeight";
                        }else if (result >=17.8 &&  result <= 26.5){
                            define = "Healthy";
                        }else if (result > 26.5 && result <= 31.8){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                }
            }else {
                switch (agePlacer){
                    case 2:
                        if(result < 14.8){
                            define = "UnderWeight";
                        }else if (result >=14.8 &&  result <= 18.2){
                            define = "Healthy";
                        }else if (result > 18.2 && result <= 19.4){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 3:
                        if(result < 14.3){
                            define = "UnderWeight";
                        }else if (result >=14.3 &&  result <= 17.3){
                            define = "Healthy";
                        }else if (result > 17.3 && result <= 18.3){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 4:
                        if(result < 14.0){
                            define = "UnderWeight";
                        }else if (result >=14.0 &&  result <= 16.9){
                            define = "Healthy";
                        }else if (result > 16.9 && result <= 17.8){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 5:
                        if(result < 13.9){
                            define = "UnderWeight";
                        }else if (result >=13.9 &&  result <= 16.8){
                            define = "Healthy";
                        }else if (result > 16.8 && result <= 17.9){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 6:
                        if(result < 13.7){
                            define = "UnderWeight";
                        }else if (result >=13.7 &&  result <= 17){
                            define = "Healthy";
                        }else if (result > 17 && result <= 18.4){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 7:
                        if(result < 13.7){
                            define = "UnderWeight";
                        }else if (result >=13.7 &&  result <= 17.4){
                            define = "Healthy";
                        }else if (result > 17.4 && result <= 19.1){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 8:
                        if(result < 13.8){
                            define = "UnderWeight";
                        }else if (result >=13.8 &&  result <= 17.9){
                            define = "Healthy";
                        }else if (result > 17.9 && result <= 19.5){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 9:
                        if(result < 14){
                            define = "UnderWeight";
                        }else if (result >=14 &&  result <= 18.6){
                            define = "Healthy";
                        }else if (result > 18.6 && result <= 21){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 10:
                        if(result < 14.2){
                            define = "UnderWeight";
                        }else if (result >=14.2 &&  result <= 19.4){
                            define = "Healthy";
                        }else if (result > 19.4 && result <= 22.1){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 11:
                        if(result < 14.5){
                            define = "UnderWeight";
                        }else if (result >=14.5 &&  result <= 20.2){
                            define = "Healthy";
                        }else if (result > 20.2 && result <= 23.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 12:
                        if(result < 15){
                            define = "UnderWeight";
                        }else if (result >=15 &&  result <= 21){
                            define = "Healthy";
                        }else if (result > 21 && result <= 24.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 13:
                        if(result < 15.4){
                            define = "UnderWeight";
                        }else if (result >=15.4 &&  result <= 21.8){
                            define = "Healthy";
                        }else if (result > 21.8 && result <= 25.1){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 14:
                        if(result < 16){
                            define = "UnderWeight";
                        }else if (result >=16 &&  result <= 22.60){
                            define = "Healthy";
                        }else if (result > 22.60 && result <= 26){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 15:
                        if(result < 16.5){
                            define = "UnderWeight";
                        }else if (result >=16.5 &&  result <= 23.4){
                            define = "Healthy";
                        }else if (result > 23.4 && result <= 26.8){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 16:
                        if(result < 17.1){
                            define = "UnderWeight";
                        }else if (result >=17.1 &&  result <= 24.2){
                            define = "Healthy";
                        }else if (result > 24.2 && result <= 27.5){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 17:
                        if(result < 17.6){
                            define = "UnderWeight";
                        }else if (result >=17.6 &&  result <= 24.9){
                            define = "Healthy";
                        }else if (result > 24.9 && result <= 28.2){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 18:
                        if(result < 18.2){
                            define = "UnderWeight";
                        }else if (result >=18.2 &&  result <= 25.6){
                            define = "Healthy";
                        }else if (result > 25.6 && result <= 28.9){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 19:
                        if(result < 18.7){
                            define = "UnderWeight";
                        }else if (result >=18.7 &&  result <= 26.3){
                            define = "Healthy";
                        }else if (result > 26.3 && result <= 29.7){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;
                    case 20:
                        if(result < 19.1){
                            define = "UnderWeight";
                        }else if (result >=19.1 &&  result <= 27.0){
                            define = "Healthy";
                        }else if (result > 27.0 && result <= 30.6){
                            define = "overWeight";
                        }else {
                            define = "obesity";
                        }
                        break;

                }
            }
        }else if(agePlacer >= 21){
            if(result < 15 ){
                define = "Very severely underweight";
            }else if(result>=15 && result<16){
                define = "Severely underweight";
            }else if(result>=16 && result<18.5){
                define = "underweight";
            }else if(result>=18.5 && result<25){
                define = "normal";
            }else if(result>=25 && result<30){
                define = "overweight";
            }else if(result>=30 && result<35){
                define = "Mode moderately";
            }else if(result>=35 && result<40){
                define = "Severely obese";
            }else {
                define = "Very severely obese";
            }
        }else{
            define="can't define age 1 / baby categorization";
        }
        if(agePlacer >=2 && agePlacer<=12){
            setAgeGroup("Children");
        }else if(agePlacer>12 && agePlacer<=21){
            setAgeGroup("Teenager");
        }else if(agePlacer>21){
            setAgeGroup("Adult");
        }else {
            setAgeGroup("Baby");
        }
        setStatus(define);
    }
    @Override
    public String toString() {
        return "ResultViewModel{" +
                "calMethod='" + calMethod + '\'' +
                ", height='" + height + '\'' +
                ", weight='" + weight + '\'' +
                ", age='" + age + '\'' +
                ", gender='" + gender + '\'' +
                ", inch='" + inch + '\'' +
                ", lbs='" + lbs + '\'' +
                '}';
    }
}
