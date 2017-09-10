
#!/bin/bash

JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64

rm -f src/main/resources/META-INF/libfast-md5.so
gcc src/main/c/md5-linux.c -std=c99 -Wall -fPIC -O3 -lssl -lcrypto -shared -Wno-return-type -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -o src/main/resources/META-INF/libfast-md5.so
strip src/main/resources/META-INF/libfast-md5.so
