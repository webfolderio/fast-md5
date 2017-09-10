package io.webfolder.fast.md5;

import static java.lang.System.getProperty;
import static java.lang.System.load;
import static java.nio.file.Files.copy;
import static java.nio.file.Files.createTempFile;
import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;
import static java.util.Locale.ENGLISH;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Path;

public class Md5 {

    static {
        String os = getProperty("os.name").toLowerCase(ENGLISH);
        boolean win = os.contains("windows");
        if (win) {
            loadLibrary("META-INF/fast-md5.dll");
        } else {
            try {
                loadLibrary("META-INF/libfast-md5-debian.so");
            } catch (Throwable t) {
                // ignored
            }
            loadLibrary("META-INF/libfast-md5-rhel.so");
        }
    }

    private static void loadLibrary(String lib) {
        Path libFile;
        ClassLoader cl = Md5.class.getClassLoader();
        String os = getProperty("os.name").toLowerCase(ENGLISH);
        boolean win = os.contains("windows");
        try (InputStream is = cl.getResourceAsStream(lib)) {
            libFile = createTempFile("libfast-md5", win ? ".dll" : ".so");
            copy(is, libFile, REPLACE_EXISTING);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        libFile.toFile().deleteOnExit();
        load(libFile.toAbsolutePath().toString());
    }

    public static native String generate(String path);
}
