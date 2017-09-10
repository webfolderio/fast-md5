#include <jni.h>

#include <windows.h>
#include <Wincrypt.h>
#include <wchar.h>

#define BUFSIZE 32768
#define MD5LEN 16

/*
 * Class:     io_webfolder_fast_md5_Md5
 * Method:    generate
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_io_webfolder_fast_md5_Md5_generate(JNIEnv *env, jclass klass,
                                           jstring path) {
  BOOL bResult = FALSE;
  HCRYPTPROV hProv = 0;
  HCRYPTHASH hHash = 0;
  HANDLE hFile = NULL;
  BYTE rgbFile[BUFSIZE];
  DWORD cbRead = 0;
  BYTE rgbHash[MD5LEN];
  DWORD cbHash = 0;
  CHAR rgbDigits[] = "0123456789abcdef";

  const char *c_path = (char *)(*env)->GetStringUTFChars(env, path, NULL);

  const size_t size = strlen(c_path) + 1;
  LPCWSTR filename = malloc(sizeof(wchar_t) * (size + 1));
  mbstowcs(filename, c_path, size);

  (*env)->ReleaseStringUTFChars(env, path, c_path);

  hFile = CreateFileW(filename,
                        GENERIC_READ,
                        FILE_SHARE_READ | FILE_SHARE_WRITE, // Important!
                                                            // Although we do not change the content of the file, we must pass FILE_SHARE_WRITE flag.
                                                            // Otherwise winapi return ERROR_SHARING_VIOLATION error code if file is already opened for write.
                        NULL,                               // https://stackoverflow.com/questions/10473442/why-cant-i-open-a-file-for-reading-if-theoretically-i-should-be-allowed
                        OPEN_EXISTING,
                        FILE_FLAG_SEQUENTIAL_SCAN,
                        NULL);

  if (INVALID_HANDLE_VALUE == hFile) {
    return NULL;
  }

  // Get handle to the crypto provider
  if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL,
                           CRYPT_VERIFYCONTEXT)) {
    CloseHandle(hFile);
    return NULL;
  }

  if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
    CloseHandle(hFile);
    CryptReleaseContext(hProv, 0);
    return NULL;
  }

  while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL)) {
    if (0 == cbRead) {
      break;
    }

    if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      CloseHandle(hFile);
      return NULL;
    }
  }

  if (!bResult) {
    CryptReleaseContext(hProv, 0);
    CryptDestroyHash(hHash);
    CloseHandle(hFile);
    return NULL;
  }

  cbHash = MD5LEN;

  char c_hash[64];
  char *ap = c_hash;

  if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
    for (DWORD i = 0; i < cbHash; i++) {
      ap += sprintf(ap, "%c%c", rgbDigits[rgbHash[i] >> 4],
                    rgbDigits[rgbHash[i] & 0xf]);
    }
  } else {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);
    return NULL;
  }

  CryptDestroyHash(hHash);
  CryptReleaseContext(hProv, 0);
  CloseHandle(hFile);

  jstring j_hash = (*env)->NewStringUTF(env, c_hash);
  return j_hash;
}
