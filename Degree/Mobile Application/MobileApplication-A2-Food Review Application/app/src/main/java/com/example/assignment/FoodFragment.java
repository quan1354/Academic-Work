package com.example.assignment;

import static java.util.Objects.requireNonNull;

import android.app.Application;
import android.content.Context;
import android.hardware.input.InputManager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SearchView;
import androidx.databinding.DataBindingUtil;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.example.assignment.database.FoodDao;
import com.example.assignment.database.FoodRoomDatabase;
import com.example.assignment.database.FoodTable;
import com.example.assignment.databinding.FragmentNoodleListBinding;
import com.google.android.material.snackbar.Snackbar;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;


public class FoodFragment extends Fragment {
    private FoodViewModel foodViewModel;
    private FoodAdapter foodAdapter;
    private ClickEvent clickEvent = null;
    private FragmentNoodleListBinding binding;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        //get fragment label
        String fragmentLabel = requireNonNull(requireNonNull(((AppCompatActivity) requireActivity()).getSupportActionBar()).getTitle()).toString();

        //Data binding
        binding = DataBindingUtil.inflate(inflater, R.layout.fragment_noodle_list, container, false);

        // let view model associate to this fragment
        Application application = requireActivity().getApplication();
        FoodDao dataSource = FoodRoomDatabase.getDatabase(application).foodDao();
        FoodViewModelFactory viewModelFactory = new FoodViewModelFactory(dataSource, application, fragmentLabel);
        foodViewModel = new ViewModelProvider(this, viewModelFactory).get(FoodViewModel.class);
        binding.setFoodViewModel(foodViewModel);

        // action For every event click from food list
        FoodAdapter.ClickEvent clickFood= new FoodAdapter.ClickEvent()  {
            @Override
            public void onCLickListener(FoodTable food) {
                if(clickEvent != null){
                    clickEvent.onClickListenFood(food);
                }
            }
            @Override
            public void onClickDeleteFood(FoodTable food) {
                foodViewModel.deleteFood(food);
            }
        };

        // set up Adapter
        binding.setLifecycleOwner(getViewLifecycleOwner());
        binding.noodleRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));
        foodAdapter = new FoodAdapter(getActivity(), clickFood);
        binding.noodleRecyclerView.setAdapter(foodAdapter);

        // Clear all data
        binding.button.setOnClickListener(view -> foodViewModel.onClear());
        foodViewModel.showSnackBarEvent.observe(getViewLifecycleOwner(), item -> {
            if(item){
                Snackbar.make(requireNonNull(getActivity()).findViewById(android.R.id.content),"All details data is gone",Snackbar.LENGTH_SHORT).show();
                foodViewModel.doneShowingSnackBar();
            }
        });

        //Observer for all food data
        foodViewModel.allFood.observe(getViewLifecycleOwner(), foodList -> {
            if(foodList!=null){
                foodList.sort((food1, food2) -> food1.getName().compareToIgnoreCase(food2.getName()));
                foodAdapter.submitList(foodList);
            }
        });
        // Search box event
        binding.searchBox.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                return false;
            }
            @Override
            public boolean onQueryTextChange(String query) {
                if(query != null){
                    searchDatabase(query);
                }
                return false;
            }
        });
        return binding.getRoot();
    }

    // Search function
    public void searchDatabase(String query){
        String placer = "%"+query+"%";
        foodViewModel.search(placer).observe(getViewLifecycleOwner(), foodList -> {
            if(foodList != null){
                foodList.sort((food1, food2) -> food1.getName().compareToIgnoreCase(food2.getName()));
                foodAdapter.submitList(foodList);
            }
        });
    }

    @Override
    public void onAttach(@NonNull Context context) {
        try {
            if (context instanceof ClickEvent)
                clickEvent = (ClickEvent) context;
        }catch (ClassCastException e){
            throw new RuntimeException("Unable to cast context object to ClickListener");
        }
        super.onAttach(context);
    }

}