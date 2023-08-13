package com.example.assignment;

import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.util.Log;
import androidx.annotation.NonNull;
import com.bumptech.glide.load.engine.bitmap_recycle.BitmapPool;
import com.bumptech.glide.load.resource.bitmap.BitmapTransformation;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;

public class BitmapRotation extends BitmapTransformation {
    private final float angle;

    // Picture rotate angle
    public BitmapRotation(final float angle) {
        this.angle = angle;
    }

    // For transform image
    @Override protected Bitmap transform(
            @NonNull final BitmapPool pool,
            @NonNull final Bitmap toTransform,
            final int outWidth,
            final int outHeight
    ) {
        final Matrix matrix = new Matrix();
        matrix.postRotate(angle);
        Log.d("BitmapRotation", String.valueOf(angle));
        return Bitmap.createBitmap(
                toTransform,
                0,
                0,
                toTransform.getWidth(),
                toTransform.getHeight(),
                matrix,
                true
        );
    }

    @Override public void updateDiskCacheKey(@NonNull final MessageDigest messageDigest) {
        messageDigest.update(
                "com.example.assignment.BitmapRotation".getBytes(StandardCharsets.UTF_8)
        );
    }
}
