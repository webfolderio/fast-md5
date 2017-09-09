package io.webfolder.fast.md5;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import org.apache.commons.codec.digest.DigestUtils;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class TestMd5 {

    private static final String  MD5_OF_EMPTY_STRING = "d41d8cd98f00b204e9800998ecf8427e";
    
    @Test
    public void test() throws IOException {
        assertEquals(DigestUtils.md5Hex(new FileInputStream(new File("LICENSE"))), Md5.generate("LICENSE"));

        assertEquals(MD5_OF_EMPTY_STRING, Md5.generate("emptyfile"));
    }
}
