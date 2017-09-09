#include <jni.h>
#include <stdint.h>
#include <openssl/ssl.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define READ_SIZE 32768

/*
 * Class:     io_webfolder_fast_md5_Md5
 * Method:    generate
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_io_webfolder_fast_md5_Md5_generate(JNIEnv *env, jclass klass,
                                           jstring path) {
  int FD;
  char file_buf[READ_SIZE];
  int ret;
  EVP_MD_CTX md5ctx;
  unsigned char md5val[EVP_MAX_MD_SIZE];
  unsigned int md5len;

  const char *c_path = (char *)(*env)->GetStringUTFChars(env, path, NULL);

  if ((FD = open(c_path, O_RDONLY)) < 0) {
    (*env)->ReleaseStringUTFChars(env, path, c_path);
    return NULL;
  }

  EVP_DigestInit(&md5ctx, EVP_md5());

  while ((ret = read(FD, file_buf, READ_SIZE)) > 0) {
    EVP_DigestUpdate(&md5ctx, file_buf, ret);
  }

  if (ret < 0) {
    (*env)->ReleaseStringUTFChars(env, path, c_path);
    return NULL;
  }

  close(FD);
  (*env)->ReleaseStringUTFChars(env, path, c_path);

  EVP_DigestFinal(&md5ctx, md5val, &md5len);
  EVP_MD_CTX_cleanup(&md5ctx);

  char c_hash[64];
  char *ap = c_hash;

  for (int i = 0; i < md5len; i++) {
    ap += sprintf(ap, "%02x", md5val[i]);
  }

  jstring j_hash = (*env)->NewStringUTF(env, c_hash);
  return j_hash;
}
