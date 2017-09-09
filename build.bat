del /Q src\main\resources\META-INF\fast-md5.dll
gcc src\main\c\md5-win.c -std=c99 -Wall -fPIC -O3 -I %JAVA_HOME%\include -I %JAVA_HOME%\include\win32 -shared -o src\main\resources\META-INF\fast-md5.dll
strip src\main\resources\META-INF\fast-md5.dll
