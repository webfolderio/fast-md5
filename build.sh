#!/bin/bash

JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64

gcc -std=c99 -Wall -fPIC -O3 src/main/c/md5-jni.c src/main/c/md5-compress.c -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -shared -o src/main/resources/META-INF/libfast-md5.so
