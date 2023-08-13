package com.example.assignment;

import com.example.assignment.database.FoodTable;

public interface ClickEvent{
    void onClickListenFood(FoodTable foodTable);
    void ModifyActionBtn();
    void shareItem(FoodTable food);
}
