gcc -std=c99 -Wall -O3 src\main\c\md5-jni.c src\main\c\md5-compress.c -I %JAVA_HOME%\include -I %JAVA_HOME%\include\win32 -shared -o src\main\resources\META-INF\fast-md5-x64.dll