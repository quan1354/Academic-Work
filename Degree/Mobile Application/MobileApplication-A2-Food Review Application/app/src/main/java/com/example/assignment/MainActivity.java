package com.example.assignment;

import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.ShareActionProvider;
import androidx.core.view.MenuItemCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.fragment.NavHostFragment;
import androidx.navigation.ui.NavigationUI;

import com.example.assignment.database.FoodTable;
import com.example.assignment.databinding.ActivityMainBinding;
import com.example.assignment.databinding.FragmentFoodDetailBinding;

import org.w3c.dom.Text;

import java.io.Serializable;

public class MainActivity extends AppCompatActivity implements ClickEvent{
    private ShareActionProvider shareActionProvider;
    private ActivityMainBinding binding;
    private  MainViewModel mainViewModel;
    private DrawerLayout drawerLayout;
    private NavController navController;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //view binding
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        // associate with View Model
        mainViewModel = new ViewModelProvider(this).get(MainViewModel.class);

        //show Logo
        NavHostFragment navHostFragment = (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.myNavHostFragment);
        navController = navHostFragment.getNavController();
        mainViewModel.isNavigateWelcome.observe(this, value ->{
            if(!value){
                navController.navigate(R.id.loginFragment);
                mainViewModel.isNavigateWelcomeScreen();
            }
        });

        // set up drawer
        drawerLayout = binding.drawerLayout;
        NavigationUI.setupActionBarWithNavController(this,navController,drawerLayout);
        NavigationUI.setupWithNavController(binding.navView,navController);
        
        // check portrait or landscape view
        mainViewModel.setOrientation(getResources().getConfiguration().orientation);
        mainViewModel.orientation.observe(this, value ->{
            if(value == Configuration.ORIENTATION_LANDSCAPE) {
                mainViewModel.setFoodID(-1L);
                mainViewModel.setIsDualPane(true);
            } else {
                mainViewModel.setIsDualPane(false);
            }
        });
    }

    // give drawer can navigation back
    @Override
    public boolean onSupportNavigateUp() {
        NavHostFragment navHostFragment = (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.myNavHostFragment);
        NavController navController = navHostFragment.getNavController();
        return NavigationUI.navigateUp(navController,drawerLayout);
    }

    // For share action
    private void setIntent(String text){
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_TEXT,text);
        shareActionProvider.setShareIntent(intent);
    }

    // Create Menu
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        MenuItem shareButton = menu.findItem(R.id.action_share);
        MenuItem addButton = menu.findItem(R.id.addFoodFragment);
        // navigate to add form when clicked add record
        addButton.setOnMenuItemClickListener(menuItem1 -> {
            try {
                FoodFragmentDirections.ActionNoodleFragmentToAddFoodFragment action = FoodFragmentDirections.actionNoodleFragmentToAddFoodFragment(0L);
                Navigation.findNavController(findViewById(R.id.myNavHostFragment)).navigate(action);
            }catch (Exception e){
                Log.d("Navigate Error", "not navigate from home page");
            }
            return false;
        });
        // share with intent when share button is clicked
        shareActionProvider = (ShareActionProvider) MenuItemCompat.getActionProvider(shareButton);
        setIntent("This is share context");
        return super.onCreateOptionsMenu(menu);
    }

    // Perform action whenever user clicked food, and avoid clash
    @Override
    public void onClickListenFood(FoodTable food) {
        FoodDetailFragment detailFragment = (FoodDetailFragment) getSupportFragmentManager().findFragmentById(R.id.myNavHostFragment2);
        try {
            if(mainViewModel.getIsDualPane()){
                if (detailFragment != null) {
                    // save food id to main activity, change detail fragment of details show (Landscape)
                    mainViewModel.setFoodID(food.getFoodId());
                    detailFragment.changeFood(food);
                }
            }else{
                // save food id to main activity, user package in saveArgs for change details show, and navigate to details fragment (Portrait)
                mainViewModel.setFoodID(food.getFoodId());
                FoodParcel foodParcel = new FoodParcel(food.getFoodId(),food.getName(),food.getGroup(),food.getDate(),food.getTime(),food.getDescription(),food.getReporter(),food.getRating(),food.getImageUri(),food.getImageAngle());
                FoodFragmentDirections.ActionFoodListFragmentToFoodDetailFragment action = FoodFragmentDirections.actionFoodListFragmentToFoodDetailFragment(foodParcel);
                Navigation.findNavController(findViewById(R.id.FoodListFragment)).navigate(action);
            }
        }catch (Exception e){
            System.out.println("Oops an Unexpected error has Occur");
        }
    }

    @Override
    public void ModifyActionBtn() {
        try {
            if(mainViewModel.getIsDualPane()){
                // Alert user when ever user did not selected food for modify, navigate to modify form with food id
                if(mainViewModel.getFoodID() != -1L){
                    FoodFragmentDirections.ActionNoodleFragmentToAddFoodFragment action = FoodFragmentDirections.actionNoodleFragmentToAddFoodFragment(mainViewModel.getFoodID());
                    Navigation.findNavController(findViewById(R.id.myNavHostFragment)).navigate(action);
                }else{
                    Toast.makeText(this,"Please select a food", Toast.LENGTH_SHORT).show();
                }
            }else{
                // navigate from details fragment to modify form with food ID.
                FoodDetailFragmentDirections.ActionFoodDetailFragmentToAddFoodFragment2 action = FoodDetailFragmentDirections.actionFoodDetailFragmentToAddFoodFragment2(mainViewModel.getFoodID());
                Navigation.findNavController(findViewById(R.id.foodDetail)).navigate(action);
            }
        }catch (Exception e){
            System.out.println("Oops an Unexpected error has Occur");
        }
    }

    // For share food purpose
    @Override
    public void shareItem(FoodTable food) {
        try {
            if(mainViewModel.getFoodID() != -1L){
                Intent intent = new Intent(Intent.ACTION_SEND);
                intent.setType("text/plain");
                intent.putExtra(Intent.EXTRA_TEXT,"I would like to share you about this food, Let have a try xd !!!\n" +
                        "Food ->"  + food.getName() + "\n" +
                        "Group ->" + food.getGroup() + "\n");
                intent.putExtra(Intent.EXTRA_SUBJECT, "My Best Food");
                Intent.createChooser(intent,"Send To...");
                startActivity(intent);
            }else{
                Toast.makeText(this,"Please select a food", Toast.LENGTH_SHORT).show();
            }
        }catch (Exception exception){
            System.out.println("Oops an Unexpected error has Occur");
        }
    }
}