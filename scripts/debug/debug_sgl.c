#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>

//First 5 bytes is MD5 hash of "NaturalPoint"
static uint8_t secret_key[] = {0x0e, 0x9a, 0x63, 0x71, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t S[256] = {0};
static uint8_t rc4_i = 0;
static uint8_t rc4_j = 0;

static void ksa(uint8_t key[], size_t len)
{
  unsigned int i, j;
  for(i = 0; i < 256; ++i){
    S[i] = i;
  }
  j = 0;
  for(i = 0; i < 256; ++i){
    j = (j + S[i] + key[i % len]) % 256;
    uint8_t tmp = S[i];
    S[i] = S[j];
    S[j] = tmp;
  }
  rc4_i = rc4_j = 0;
}

static uint8_t rc4()
{
  rc4_i += 1;
  rc4_j += S[rc4_i];
  uint8_t tmp = S[rc4_i];
  S[rc4_i] = S[rc4_j];
  S[rc4_j] = tmp;
  return S[(S[rc4_i] + S[rc4_j]) % 256];
}

static bool decrypt_file(const char *fname, char **decoded, size_t *datlen)
{
  ksa(secret_key, 16);
  FILE *inp;
  struct stat fst;

  if((inp = fopen(fname, "rb")) == NULL){
    printf("Can't open input file '%s'\n", fname);
    return false;
  }

  if(fstat(fileno(inp), &fst) != 0){
    fclose(inp);
    printf("Cannot stat file '%s'\n", fname);
    return false;
  }

  *datlen = fst.st_size;
  if((*decoded = (char *)malloc(*datlen+1)) == NULL){
    printf("malloc failed!\n");
    fclose(inp);
    return false;
  }
  memset(*decoded, 0, *datlen+1);
  
  size_t len = fread(*decoded, 1, *datlen, inp);
  fclose(inp);
  
  if(len != *datlen) {
    printf("Read error: expected %zu bytes, got %zu\n", *datlen, len);
    free(*decoded);
    return false;
  }
  
  // Decrypt
  for(size_t i = 0; i < *datlen; ++i) {
    (*decoded)[i] ^= rc4();
  }

  return true;
}

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: %s <sgl.dat file>\n", argv[0]);
    return 1;
  }
  
  char *decoded = NULL;
  size_t datlen = 0;
  
  if(!decrypt_file(argv[1], &decoded, &datlen)) {
    printf("Failed to decrypt file\n");
    return 1;
  }
  
  printf("Decrypted %zu bytes\n", datlen);
  printf("First 1000 characters:\n");
  printf("----------------------------------------\n");
  
  // Print first 1000 chars to see the XML structure
  size_t print_len = (datlen > 1000) ? 1000 : datlen;
  for(size_t i = 0; i < print_len; i++) {
    putchar(decoded[i]);
  }
  printf("\n----------------------------------------\n");
  
  // Search for ApplicationID elements
  printf("\nSearching for ApplicationID elements...\n");
  char *pos = decoded;
  int appid_count = 0;
  while((pos = strstr(pos, "ApplicationID")) != NULL) {
    appid_count++;
    printf("Found ApplicationID at position %ld: ", pos - decoded);
    
    // Print the context around this ApplicationID element
    size_t start = (pos - decoded > 100) ? pos - decoded - 100 : 0;
    size_t end = (pos - decoded + 200 < datlen) ? pos - decoded + 200 : datlen;
    
    for(size_t i = start; i < end; i++) {
      if(i == pos - decoded) printf(">>>");
      putchar(decoded[i]);
      if(i == pos - decoded + 12) printf("<<<");
    }
    printf("\n\n");
    
    pos += 12; // Move past this occurrence
  }
  
  printf("Total ApplicationID elements found: %d\n", appid_count);
  
  // Also search for any elements that might contain "null"
  printf("\nSearching for any elements containing 'null'...\n");
  pos = decoded;
  int null_count = 0;
  while((pos = strstr(pos, "null")) != NULL) {
    null_count++;
    printf("Found 'null' at position %ld: ", pos - decoded);
    
    // Print the context around this null
    size_t start = (pos - decoded > 50) ? pos - decoded - 50 : 0;
    size_t end = (pos - decoded + 100 < datlen) ? pos - decoded + 100 : datlen;
    
    for(size_t i = start; i < end; i++) {
      if(i == pos - decoded) printf(">>>");
      putchar(decoded[i]);
      if(i == pos - decoded + 4) printf("<<<");
    }
    printf("\n\n");
    
    pos += 4; // Move past this occurrence
  }
  
  printf("Total 'null' occurrences found: %d\n", null_count);
  
  free(decoded);
  return 0;
} 