package io.webfolder.fast.md5;

import static java.lang.String.format;
import static java.nio.channels.Channels.newInputStream;
import static java.nio.channels.FileChannel.open;
import static java.nio.file.LinkOption.NOFOLLOW_LINKS;
import static java.nio.file.StandardOpenOption.READ;

import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.nio.file.Paths;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class TestMd5 {

    private static final int BUFFER_SIZE = 1024 * 256;

    @Test
    public void test() throws IOException {
        Md5 md5 = new Md5();

        try (InputStream is = newInputStream(open(Paths.get("LICENSE"), NOFOLLOW_LINKS, READ))) {
            final byte[] buffer = new byte[BUFFER_SIZE];
            int read;
            while((read = is.read(buffer)) > 0) {
                md5.update(buffer, 0, read);
            }
        }

        byte[] hash = md5.getHash();
        String hashHexValue = format("%032x", new BigInteger(1, hash));

        assertEquals("19863bdab2ed96015a882387f357aab2", hashHexValue);

        System.out.println(hashHexValue);
    }
}
