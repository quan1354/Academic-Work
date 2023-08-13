package com.example.assignment;

import static com.example.assignment.Util.clear;
import static java.util.Objects.requireNonNull;

import android.Manifest;
import android.app.AlertDialog;
import android.app.Application;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.content.pm.PackageManager;
import android.icu.util.Calendar;
import android.net.Uri;
import android.os.Bundle;
import android.text.format.DateFormat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.DatePicker;
import android.widget.TimePicker;
import android.widget.Toast;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.databinding.DataBindingUtil;
import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.fragment.NavHostFragment;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.example.assignment.database.FoodDao;
import com.example.assignment.database.FoodRoomDatabase;
import com.example.assignment.databinding.AddFoodFragmentBinding;

import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;

public class AddFoodFragment extends Fragment {
    private AddFoodViewModel viewModel;
    private AddFoodFragmentBinding binding;

    @Override public View onCreateView(@NonNull final LayoutInflater inflater, final ViewGroup container, final Bundle savedInstanceState) {
        // connect view model
        Application application = requireActivity().getApplication();
        FoodDao dataSource = FoodRoomDatabase.getDatabase(application).foodDao();
        AddFoodViewModelFactory viewModelFactory = new AddFoodViewModelFactory(dataSource, application);
        viewModel = new ViewModelProvider(this, viewModelFactory).get(AddFoodViewModel.class);
        // observe form with liva data, do validation
        viewModel.name.observe(getViewLifecycleOwner(), value -> {
            if (viewModel.shouldShowNameError()) binding.name.setError("Please enter a name!");
            else binding.name.setError(null);
        });
        viewModel.group.observe(getViewLifecycleOwner(), value -> {
            if (viewModel.shouldShowFoodGroupError()) binding.foodGroup.setError("Please enter a group! (noodle/rice/meat)");
            else binding.foodGroup.setError(null);
        });
        viewModel.reporter.observe(getViewLifecycleOwner(), value -> {
            if (viewModel.shouldShowReporterError()) binding.reporter.setError("Please enter a reporter!");
            else binding.reporter.setError(null);
        });
        viewModel.date.observe(getViewLifecycleOwner(), value -> {
            if (viewModel.shouldShowDateError()) binding.date.setError("Please enter a date!");
            else binding.date.setError(null);
        });
        viewModel.time.observe(getViewLifecycleOwner(), value -> {
            if (viewModel.shouldShowTimeError()) binding.time.setError("Please enter a time!");
            else binding.time.setError(null);
        });
        viewModel.getImageUri().observe(getViewLifecycleOwner(), value ->
                Glide.with(this)
                        .load(value == null ? R.drawable.placeholder : value)
                        .into(binding.image)
        );
        viewModel.getImageAngle().observe(getViewLifecycleOwner(), value -> {
            if (viewModel.getImageUri().getValue() != null) {
                Glide.with(this)
                        .asBitmap()
                        .load(viewModel.getImageUri().getValue())
                        .transform(new BitmapRotation(value))
                        .diskCacheStrategy(DiskCacheStrategy.DATA)
                        .into(binding.image);
            }
        });
        // Alert user, for empty require field
        viewModel.hasError().observe(getViewLifecycleOwner(), value -> {
            if (!value) return;
            Toast.makeText(getContext(), "Please fill in all the required information!", Toast.LENGTH_LONG).show();
            viewModel.doneError();
        });
        // set up data binding
        binding = DataBindingUtil.inflate(inflater, R.layout.add_food_fragment, container, false);
        binding.setLifecycleOwner(getViewLifecycleOwner());
        binding.setViewModel(viewModel);

        //check weather is update form
        AddFoodFragmentArgs args = AddFoodFragmentArgs.fromBundle(requireArguments());
        viewModel.setFoodID(args.getId() == 0L ? 0L : args.getId());
        if(viewModel.getFoodID() != 0L){
            requireNonNull(((AppCompatActivity) requireActivity()).getSupportActionBar()).setTitle("Modify Food");
            binding.addButton.setText("Update");
            viewModel.setEvent("update");
            viewModel.takeFoodValue(viewModel.getFoodID());
        }

        // Confirmation add or modify food alert
        viewModel.canAdd().observe(getViewLifecycleOwner(), value -> {
            if (!value) return;
            new AlertDialog.Builder(getContext())
                    .setTitle(String.format("%s Confirmation", viewModel.getEvent().equalsIgnoreCase("update")? "update" : "add"))
                    .setMessage(String.format("Are you sure you want to %s %s?", viewModel.getEvent().equalsIgnoreCase("update")? "Update" : "Add", viewModel.name.getValue()))
                    .setIcon(android.R.drawable.ic_dialog_alert)
                    .setPositiveButton(android.R.string.yes, (dialog, which) -> {
                        try {
                            viewModel.confirmAdd(requireContext());
                        } catch (final IOException error) {
                            error.printStackTrace();
                        }
                        viewModel.doneAdd();
                        NavHostFragment.findNavController(AddFoodFragment.this).navigate(R.id.FoodListFragment);
                    })
                    .setNegativeButton(android.R.string.no, null).show();
        });

        // let user select date and time
        binding.dateEditText.setOnClickListener(view ->
                new DatePickerFragment(binding).show(requireActivity().getSupportFragmentManager(), "datePicker")
        );
        binding.timeEditText.setOnClickListener(view ->
                new TimePickerFragment(binding).show(requireActivity().getSupportFragmentManager(), "timePicker")
        );

        // get photo content with intent
        final ActivityResultLauncher<String> getContent = registerForActivityResult(
                new ActivityResultContracts.GetContent(), result -> {
                    if (result != null) viewModel.changeImage(result);
                }
        );
        binding.image.setOnClickListener(view -> getContent.launch("image/*"));

        // ask for photo permission
        final ActivityResultLauncher<String> requestPermission =
                registerForActivityResult(
                        new ActivityResultContracts.RequestPermission(), result -> {
                            if (!result) { Toast.makeText(getContext(), "Permission for accessing camera is needed to take a food photo!", Toast.LENGTH_SHORT).show();
                            }
                        }
                );

        // take picture, then upload to form
        final ActivityResultLauncher<Uri> takePicture = registerForActivityResult(
                new ActivityResultContracts.TakePicture(), result -> {
                    if (result) viewModel.changeImage(viewModel.getTempPictureUri());
                }
        );

        // capture a photo
        binding.takeFoodPhotoButton.setOnClickListener(view -> {
            if (
                    requireActivity()
                            .getPackageManager()
                            .hasSystemFeature(PackageManager.FEATURE_CAMERA_ANY)
            ) {
                requestPermission.launch(Manifest.permission.CAMERA);
                if (
                        ContextCompat.checkSelfPermission(
                                requireContext(), Manifest.permission.CAMERA
                        ) == PackageManager.PERMISSION_GRANTED
                ) {
                    clear(requireContext().getCacheDir());
                    takePicture.launch(viewModel.makeTempPictureUri(requireContext()));
                }
            } else {
                Toast.makeText(
                        getContext(),
                        "Sorry, your phone doesn't support taking a food photo :(",
                        Toast.LENGTH_SHORT
                ).show();
            }
        });

        // action event for buttons
        binding.rotateButton.setOnClickListener(view -> viewModel.rotateImage());
        binding.deleteButton.setOnClickListener(view -> viewModel.changeImage(null));
        binding.resetButton.setOnClickListener(view -> viewModel.reset());
        binding.addButton.setOnClickListener(view -> viewModel.add());

        return binding.getRoot();
    }

    // For select date view
    public static class DatePickerFragment extends DialogFragment implements DatePickerDialog.OnDateSetListener {
        private final AddFoodFragmentBinding binding;

        private DatePickerFragment(final AddFoodFragmentBinding binding) {
            this.binding = binding;
        }

        @NonNull @Override public Dialog onCreateDialog(final Bundle savedInstanceState) {
            final Calendar calendar = Calendar.getInstance();
            return new DatePickerDialog(
                    getActivity(),
                    this,
                    calendar.get(Calendar.YEAR),
                    calendar.get(Calendar.MONTH),
                    calendar.get(Calendar.DAY_OF_MONTH)
            );
        }

        @Override public void onDateSet(
                final DatePicker view, final int year, final int month, final int dayOfMonth
        ) {
            binding.dateEditText.setText(
                    LocalDate.of(year, month + 1, dayOfMonth).format(
                            DateTimeFormatter.ofPattern("dd/MM/yyyy")
                    )
            );
        }
    }
    // For select Time view
    public static class TimePickerFragment extends DialogFragment implements TimePickerDialog.OnTimeSetListener {
        private final AddFoodFragmentBinding binding;

        private TimePickerFragment(final AddFoodFragmentBinding binding) {
            this.binding = binding;
        }

        @NonNull @Override public Dialog onCreateDialog(final Bundle savedInstanceState) {
            final Calendar calendar = Calendar.getInstance();
            return new TimePickerDialog(
                    getActivity(),
                    this,
                    calendar.get(Calendar.HOUR_OF_DAY),
                    calendar.get(Calendar.MINUTE),
                    DateFormat.is24HourFormat(getActivity())
            );
        }

        @Override public void onTimeSet(
                final TimePicker view, final int hourOfDay, final int minute
        ) {
            binding.timeEditText.setText(
                    LocalTime.of(hourOfDay, minute).format(
                            DateTimeFormatter.ofPattern(
                                    DateFormat.is24HourFormat(getActivity()) ? "HH:mm" : "hh:mm a"
                            )
                    )
            );
        }
    }
}
