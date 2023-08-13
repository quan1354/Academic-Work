package com.example.assignment.WelcomeScreen;

import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.navigation.fragment.NavHostFragment;

import com.example.assignment.R;

public class WelcomeFragment extends Fragment {
    private ImageView image;
    private Button button;
    private Handler handler;
    private boolean isSkip;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_login, container, false);
        ((AppCompatActivity) getActivity()).getSupportActionBar().setDisplayHomeAsUpEnabled(false);

        //animation
        image = view.findViewById(R.id.logo_id);
        Animation animation = AnimationUtils.loadAnimation(container.getContext(),R.anim.animation);
        image.startAnimation(animation);

        // Navigate to Home page when click skip button
        button = view.findViewById(R.id.skipLogo);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                navigateToMain();
                isSkip = true;
            }
        });

        // Show Logo for 10 seconds
        handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if(!isSkip) {
                    navigateToMain();
                }
            }
        },10000);
        return view;
    }

    // Done show Logo, avoid navigation have error occur
    public void navigateToMain(){
        try {
            NavHostFragment.findNavController(WelcomeFragment.this).navigate(R.id.action_loginFragment_to_noodleFragment);
            handler.removeCallbacksAndMessages(null);
        }catch (Exception e){
            System.out.println("Oops an unexpected Error has Occur");
        }
    }
}