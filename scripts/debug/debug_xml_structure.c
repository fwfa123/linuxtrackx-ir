#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <mxml.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>

//First 5 bytes is MD5 hash of "NaturalPoint"
static uint8_t secret_key[] = {0x0e, 0x9a, 0x63, 0x71, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t S[256] = {0};
static uint8_t rc4_i = 0;
static uint8_t rc4_j = 0;

static char *decoded = NULL;
static mxml_node_t *xml = NULL;
static mxml_node_t *tree = NULL;

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

static bool decrypt_file(const char *fname)
{
  size_t datlen;
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

  datlen = fst.st_size;
  if((decoded = (char *)malloc(datlen+1)) == NULL){
    printf("malloc failed!\n");
    fclose(inp);
    return false;
  }
  memset(decoded, 0, datlen+1);
  
  size_t len = fread(decoded, 1, datlen, inp);
  fclose(inp);
  
  if(len != datlen) {
    printf("Read error: expected %zu bytes, got %zu\n", datlen, len);
    free(decoded);
    return false;
  }
  
  // Decrypt
  for(size_t i = 0; i < datlen; ++i) {
    decoded[i] ^= rc4();
  }

  return true;
}

static bool game_data_init(const char *fname)
{
  if(!decrypt_file(fname)){
    printf("Error decrypting file!\n");
    return false;
  }
  
  // Save the raw decrypted XML to a file for examination
  FILE *xml_file = fopen("decrypted_sgl.xml", "w");
  if(xml_file != NULL) {
    fwrite(decoded, 1, strlen(decoded), xml_file);
    fclose(xml_file);
    printf("Raw decrypted XML saved to: decrypted_sgl.xml\n");
  } else {
    printf("Warning: Could not save raw XML file\n");
  }
  
  xml = mxmlNewXML("1.0");
  tree = mxmlLoadString(xml, decoded, MXML_TEXT_CALLBACK);
  return (tree != NULL);
}

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: %s <sgl.dat file>\n", argv[0]);
    return 1;
  }
  
  if(!game_data_init(argv[1])) {
    printf("Failed to initialize game data\n");
    return 1;
  }
  
  printf("Analyzing XML structure for ApplicationID elements...\n\n");
  
  mxml_node_t *game;
  int game_count = 0;
  int appid_count = 0;
  
  for(game = mxmlFindElement(tree, tree, "Game", NULL, NULL, MXML_DESCEND);
      game != NULL;
      game = mxmlFindElement(game, tree, "Game", NULL, NULL, MXML_DESCEND)){
    
    game_count++;
    const char *name = mxmlElementGetAttr(game, "Name");
    const char *id = mxmlElementGetAttr(game, "Id");
    
    mxml_node_t *appid = mxmlFindElement(game, game, "ApplicationID", NULL, NULL, MXML_DESCEND);
    if(appid != NULL) {
      appid_count++;
      printf("Game %d: %s (ID: %s)\n", game_count, name, id);
      
      mxml_node_t *child = mxmlGetFirstChild(appid);
      if(child != NULL) {
        const char *element_name = mxmlGetElement(child);
        const char *text_content = mxmlGetText(child, NULL);
        
        printf("  ApplicationID child element: '%s'\n", element_name ? element_name : "NULL");
        printf("  ApplicationID text content: '%s'\n", text_content ? text_content : "NULL");
        
        // Check node type
        mxml_type_t node_type = mxmlGetType(child);
        printf("  Node type: %d\n", node_type);
        
        // If it's a text node, try to get the text directly
        if(node_type == MXML_TEXT) {
          const char *direct_text = mxmlGetText(child, NULL);
          printf("  Direct text (MXML_TEXT): '%s'\n", direct_text ? direct_text : "NULL");
        }
        
        printf("\n");
      } else {
        printf("  ApplicationID has no children\n\n");
      }
      
      // Only show first 5 games with ApplicationID to avoid spam
      if(appid_count >= 5) {
        printf("... (showing first 5 games with ApplicationID)\n");
        break;
      }
    }
  }
  
  printf("Total games found: %d\n", game_count);
  printf("Games with ApplicationID: %d\n", appid_count);
  
  // Cleanup
  mxmlDelete(tree);
  free(decoded);
  
  return 0;
} 