package com.example.assignment;

import java.io.File;
import java.util.Objects;

public class Util {
    private Util() { }
    // avoid store too many unused image
    public static void clear(final File directory) {
        for (final File file : Objects.requireNonNull(directory.listFiles())) file.delete();
    }
}
