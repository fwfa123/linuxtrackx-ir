#define _GNU_SOURCE
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include "rest.h"
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif


static ssize_t my_getline(char **lineptr, size_t *n, FILE *f)
{
#ifndef DARWIN
#ifdef __MINGW32__
  // MinGW doesn't have getline, implement a simple version
  char buffer[4096];
  if (fgets(buffer, sizeof(buffer), f) == NULL) {
    return -1;
  }
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len-1] == '\n') {
    buffer[len-1] = '\0';
    len--;
  }
  if (*lineptr == NULL) {
    *n = 4096;
    *lineptr = (char *)malloc(*n);
  } else if (*n < len + 1) {
    *n = len + 1;
    *lineptr = (char *)realloc(*lineptr, *n);
  }
  strcpy(*lineptr, buffer);
  return len;
#else
  return getline(lineptr, n, f);
#endif
#else
  size_t cnt = 0;
  char *res = fgetln(f, &cnt);
  if(res == NULL){
    return 0;
  }
  if(*lineptr == NULL){
    //there is no \0, so cnt has to be increased to contain it
    *n = ((cnt+1) > 4096) ? cnt+1 : 4096;
    *lineptr = (char *)malloc(*n);
  }else{
    if(*n < cnt+1){
      *n = ((cnt+1) > 4096) ? cnt+1 : 4096;
      *lineptr = (char *)realloc(*lineptr, *n);
    }
  }
  memcpy(*lineptr, res, cnt);
  (*lineptr)[cnt] = '\0';
  return cnt;
#endif
}


bool game_data_get_desc(int id, game_desc_t *gd)
{
  FILE *f = NULL;
  char *home = getenv("HOME");
  
  // Fix for Wine environment: getenv("HOME") can return NULL
  if (home == NULL) {
    printf("DEBUG: HOME is NULL, trying USERPROFILE...\n");
    // Fallback for Wine environments where HOME is not set
    home = getenv("USERPROFILE");
    if (home == NULL) {
      printf("DEBUG: USERPROFILE is NULL, using current directory\n");
      // Final fallback to current directory
      home = ".";
    }
  }
  
  char *path1 = (char *)malloc(200 + strlen(home));
  if (path1 == NULL) {
    printf("DEBUG: Memory allocation failed for path1!\n");
    return false;
  }
  
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/gamedata.txt", home);
    if((f = fopen(path1, "r"))== NULL){
      printf("Can't open data file '%s'!\n", path1);
      free(path1);
      return false;
    }
    int tmp_id;
    size_t tmp_str_size = 4096;
    size_t tmp_code_size = 4096;
    char *tmp_str = (char *)malloc(tmp_str_size);
    char *tmp_code = (char *)malloc(tmp_code_size);
    unsigned int c1, c2;
    int cnt;
    gd->name = NULL;
    while(!feof(f)){
      cnt = my_getline(&tmp_str, &tmp_str_size, f);
      if(cnt > 0){
        if(tmp_str_size > tmp_code_size){
          tmp_code = (char *)realloc(tmp_code, tmp_str_size);
        }
        cnt = sscanf(tmp_str, "%d \"%[^\"]\" (%08x%08x)", &tmp_id, tmp_code, &c1, &c2);
        if(cnt == 2){
          if(tmp_id == id){
            gd->name = strdup(tmp_code);
            gd->encrypted = false;
            gd->key1 = gd->key2 = 0;
            break;
          }
        }else if(cnt == 4){
          if(tmp_id == id){
            gd->name = strdup(tmp_code);
            gd->encrypted = true;
            gd->key1 = c1;
            gd->key2 = c2;
            break;
          }
        }
      }
    }
    fclose(f);
    free(tmp_code);
    free(tmp_str);
    free(path1);
  return gd->name != NULL;
}

static bool game_data_iterate(bool (*on_entry)(int id, const char *name, bool encrypted, uint32_t k1, uint32_t k2, void *ctx), void *ctx)
{
  FILE *f = NULL;
  char *home = getenv("HOME");
  if (home == NULL) {
    home = getenv("USERPROFILE");
    if (home == NULL) {
      home = ".";
    }
  }
  char *path1 = (char *)malloc(200 + strlen(home));
  if (path1 == NULL) return false;
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/gamedata.txt", home);
  if((f = fopen(path1, "r"))== NULL){
    free(path1);
    return false;
  }
  int tmp_id; size_t tmp_str_size = 4096; size_t tmp_code_size = 4096; char *tmp_str = (char *)malloc(tmp_str_size); char *tmp_code = (char *)malloc(tmp_code_size);
  unsigned int c1, c2; int cnt; bool ok = false;
  while(!feof(f)){
    cnt = my_getline(&tmp_str, &tmp_str_size, f);
    if(cnt > 0){
      if(tmp_str_size > tmp_code_size){ tmp_code = (char *)realloc(tmp_code, tmp_str_size); }
      cnt = sscanf(tmp_str, "%d \"%[^\"]\" (%08x%08x)", &tmp_id, tmp_code, &c1, &c2);
      if(cnt == 2){
        if(on_entry(tmp_id, tmp_code, false, 0, 0, ctx)){ ok = true; break; }
      }else if(cnt == 4){
        if(on_entry(tmp_id, tmp_code, true, c1, c2, ctx)){ ok = true; break; }
      }
    }
  }
  fclose(f);
  free(tmp_code); free(tmp_str); free(path1);
  return ok;
}

typedef struct {
  const char *query;
  int best_id;
  size_t best_score;
  bool found_contains;
  size_t best_contains_len;
  // Numeric-aware disambiguation: prefer candidates sharing the same first number as the query
  int query_first_number;
  bool best_contains_numeric_match;
} name_match_ctx_t;

static size_t score_match(const char *a, const char *b)
{
  // simple case-insensitive longest common substring score
  size_t best = 0;
  if (!a || !b) return 0;
  size_t la = strlen(a), lb = strlen(b);
  for(size_t i=0;i<la;i++){
    for(size_t j=0;j<lb;j++){
      size_t k=0;
      while(i+k<la && j+k<lb){
        char ca = a[i+k]; char cb = b[j+k];
        if(ca>='A'&&ca<='Z') ca = (char)(ca - 'A' + 'a');
        if(cb>='A'&&cb<='Z') cb = (char)(cb - 'A' + 'a');
        if(ca!=cb) break;
        k++;
      }
      if(k>best) best = k;
    }
  }
  return best;
}

static bool on_match_entry(int id, const char *name, bool encrypted, uint32_t k1, uint32_t k2, void *ctx)
{
  (void)encrypted; (void)k1; (void)k2;
  name_match_ctx_t *c = (name_match_ctx_t*)ctx;
  if (!c || !c->query || !name) return false;
  // exact, case-insensitive match wins immediately
  if(strcasecmp(name, c->query) == 0){ c->best_id = id; c->best_score = (size_t)-1; c->found_contains = true; c->best_contains_len = (size_t)-1; return true; }
  // contains check (case-insensitive)
  const char *q = c->query;
  size_t nq = strlen(q);
  char nbuf[4096]; size_t ni = 0; for(; name[ni] && ni < sizeof(nbuf)-1; ni++){ char ch = name[ni]; if(ch>='A'&&ch<='Z') ch = (char)(ch - 'A' + 'a'); nbuf[ni] = ch; } nbuf[ni] = 0;
  char qbuf[4096]; size_t qi = 0; for(; q[qi] && qi < sizeof(qbuf)-1; qi++){ char ch = q[qi]; if(ch>='A'&&ch<='Z') ch = (char)(ch - 'A' + 'a'); qbuf[qi] = ch; } qbuf[qi] = 0;
  if(strstr(nbuf, qbuf) != NULL){
    // Extract first integer from candidate name for numeric-aware preference
    int cand_num = 0; {
      const char *p = nbuf; while(*p && (*p < '0' || *p > '9')) p++; if(*p){ cand_num = 0; while(*p >= '0' && *p <= '9'){ cand_num = cand_num*10 + (*p - '0'); p++; } }
    }
    bool cand_numeric_match = (c->query_first_number > 0 && cand_num == c->query_first_number);
    // Prefer numeric matches over non-numeric when available; otherwise fall back to existing tie-breaks
    if(!c->found_contains){
      c->found_contains = true; c->best_contains_len = nq; c->best_id = id; c->best_contains_numeric_match = cand_numeric_match;
    }else{
      if(c->best_contains_numeric_match != cand_numeric_match){
        if(!c->best_contains_numeric_match && cand_numeric_match){
          // Upgrade to numeric-matching candidate
          c->best_contains_len = nq; c->best_id = id; c->best_contains_numeric_match = true;
        }
        // else keep current best (it already matches numeric where this one doesn't)
      }else{
        // Both candidates are same numeric class; use length/id tie-breakers
        if(nq > c->best_contains_len || (nq == c->best_contains_len && (c->best_id < 0 || id < c->best_id))){
          c->best_contains_len = nq; c->best_id = id;
        }
      }
    }
    return false;
  }
  size_t s = score_match(name, c->query);
  // Numeric-aware preference in fuzzy stage: boost score if candidate shares the same first number as the query
  if(c->query_first_number > 0){
    int cand_num = 0; {
      const char *p = nbuf; while(*p && (*p < '0' || *p > '9')) p++; if(*p){ while(*p >= '0' && *p <= '9'){ cand_num = cand_num*10 + (*p - '0'); p++; } }
    }
    if(cand_num == c->query_first_number){
      // Add a significant bonus to outweigh close fuzzy scores from similarly named titles
      s += 1000;
    }
  }
  if(s > c->best_score){
    c->best_score = s; c->best_id = id;
  }else if(s == c->best_score && s > 0){
    // Tie-break by lowest ID as per plan
    if(c->best_id < 0 || id < c->best_id){ c->best_id = id; }
  }
  return false; // continue to possibly find exact match
}

bool game_data_find_id_by_name(const char *name, int *out_id)
{
  if(!name || !out_id) return false;
  name_match_ctx_t ctx; ctx.query = name; ctx.best_id = -1; ctx.best_score = 0; ctx.found_contains = false; ctx.best_contains_len = 0; ctx.best_contains_numeric_match = false;
  // Extract first integer from query (e.g., "Falcon 4" -> 4) for numeric-aware preference
  ctx.query_first_number = 0; {
    const char *p = name; while(*p && (*p < '0' || *p > '9')) p++; if(*p){ int qn = 0; while(*p >= '0' && *p <= '9'){ qn = qn*10 + (*p - '0'); p++; } ctx.query_first_number = qn; }
  }
  bool found = game_data_iterate(on_match_entry, &ctx);
  if(!found){
    if(ctx.found_contains && ctx.best_id > 0){ found = true; }
    else if(ctx.best_score > 0 && ctx.best_id > 0){ found = true; }
  }
  if(found){ *out_id = ctx.best_id; return true; }
  return false;
}

static bool parse_steam_mapping_line(const char *line, char *appid_out, size_t appid_cap, int *ltr_id_out)
{
  if(!line || !appid_out || !ltr_id_out) return false;
  // Expected format: <steam_appid>=<ltr_id>
  const char *eq = strchr(line, '=');
  if(!eq) return false;
  size_t len = (size_t)(eq - line);
  // Trim trailing whitespace from appid
  while(len > 0 && (line[len-1] == ' ' || line[len-1] == '\t' || line[len-1] == '\r' || line[len-1] == '\n')) len--;
  // Skip leading whitespace
  size_t start = 0; while(start < len && (line[start] == ' ' || line[start] == '\t')) start++;
  size_t appid_len = len - start;
  if(appid_len == 0 || appid_len >= appid_cap) return false;
  memcpy(appid_out, line + start, appid_len);
  appid_out[appid_len] = '\0';
  // Parse ltr id integer, skipping whitespace
  const char *rhs = eq + 1; while(*rhs == ' ' || *rhs == '\t') rhs++;
  int id = atoi(rhs);
  if(id <= 0) return false;
  *ltr_id_out = id;
  return true;
}

bool game_data_find_id_by_steam_appid(const char *steam_appid, int *out_id)
{
  if(!steam_appid || !*steam_appid || !out_id) return false;
  const char *home = getenv("HOME");
  if (home == NULL) {
    home = getenv("USERPROFILE");
    if (home == NULL) {
      home = ".";
    }
  }
  char path[4096];
  snprintf(path, sizeof(path), "%s/.config/linuxtrack/tir_firmware/steam_to_trackir_id.txt", home);
  FILE *f = fopen(path, "r");
  if(!f){
    return false;
  }
  char line[1024];
  char map_appid[256];
  int ltr_id = -1;
  bool found = false;
  while(fgets(line, sizeof(line), f)){
    // Skip comments and empty lines
    const char *p = line; while(*p == ' ' || *p == '\t') p++;
    if(*p == '#' || *p == '\n' || *p == '\0') continue;
    if(parse_steam_mapping_line(p, map_appid, sizeof(map_appid), &ltr_id)){
      if(strcmp(map_appid, steam_appid) == 0){
        found = true;
        break;
      }
    }
  }
  fclose(f);
  if(found && ltr_id > 0){ *out_id = ltr_id; return true; }
  return false;
}

bool getSomeSeriousPoetry(char *verse1, char *verse2)
{
  bool res = true;
  char *home = getenv("HOME");
  
  // Fix for Wine environment: getenv("HOME") can return NULL
  if (home == NULL) {
    printf("DEBUG: HOME is NULL, trying USERPROFILE...\n");
    // Fallback for Wine environments where HOME is not set
    home = getenv("USERPROFILE");
    if (home == NULL) {
      printf("DEBUG: USERPROFILE is NULL, using current directory\n");
      // Final fallback to current directory
      home = ".";
    }
  }
  
  char *path1 = (char *)malloc(200 + strlen(home));
  char *path2 = (char *)malloc(200 + strlen(home));
  
  if (path1 == NULL || path2 == NULL) {
    printf("DEBUG: Memory allocation failed for path1 or path2!\n");
    if (path1) free(path1);
    if (path2) free(path2);
    return false;
  }
  
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/poem1.txt", home);
  sprintf(path2, "%s/.config/linuxtrack/tir_firmware/poem2.txt", home);
  FILE *f1 = fopen(path1, "rb");
  memset(verse1, 0, 200);
  if(f1 != NULL){
    if(fread(verse1, 1, 200, f1) == 0){
      printf("Cant read dll signature('%s')!\n", path1);
      res = false;
    }
    printf("DLL SIGNATURE: %s\n", verse1);
    fclose(f1);
  }else{
    printf("Can't open dll signature ('%s')!\n", path1);
    res = false;
  }
  free(path1);
  FILE *f2 = fopen(path2, "rb");
  memset(verse2, 0, 200);
  if(f2 != NULL){
    if(fread(verse2, 1, 200, f2) == 0){
      perror("fread");
      printf("Cant read app signature('%s')!\n", path2);
      res = false;
    }
    printf("APP SIGNATURE: %s\n", verse2);
    fclose(f2);
  }else{
    printf("Cant open app signature('%s')!\n", path2);
    res = false;
  }
  free(path2);
  return res;
}

bool getDebugFlag(const int flag)
{
  char *dbg_flags = getenv("LINUXTRACK_DBG");
  if(dbg_flags == NULL) return false;
  if(strchr(dbg_flags, flag) != NULL){
    return true;
  }else{
    return false;
  }
}

char *file_path(const char *file)
{
  HKEY  hkey   = 0;
  RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\NaturalPoint\\NATURALPOINT\\NPClient Location", 0,
    KEY_QUERY_VALUE, &hkey);
  if(!hkey){
    printf("Can't open registry key\n");
    return NULL;
  }

  BYTE path[1024];
  DWORD buf_len = 1024;
  LONG result = RegQueryValueEx(hkey, "Path", NULL, NULL, path, &buf_len);
  char *full_path = NULL;
  int res = -1;
  if(result == ERROR_SUCCESS && buf_len > 0){
    res = asprintf(&full_path, "%s\\%s", path, file);
  }
  RegCloseKey(hkey);
  if(res > 0){
    return full_path;
  }else{
    return NULL;
  }
}

bool tryExclusiveLock(const char *file)
{
  HANDLE f = CreateFile(file, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(f == INVALID_HANDLE_VALUE){
    return false;
  }
  OVERLAPPED overlapvar;
  overlapvar.Offset = 0;
  overlapvar.OffsetHigh = 0;
  overlapvar.hEvent = 0;
  bool res = LockFileEx(f, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 10, 0, &overlapvar);
  CloseHandle(f);
  return res;
}

bool sharedLock(const char *file)
{
  HANDLE f = CreateFile(file, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(f == INVALID_HANDLE_VALUE){
    return false;
  }
  OVERLAPPED overlapvar;
  overlapvar.Offset = 0;
  overlapvar.OffsetHigh = 0;
  overlapvar.hEvent = 0;
  bool res = LockFileEx(f, LOCKFILE_FAIL_IMMEDIATELY, 0, 10, 0, &overlapvar);
  CloseHandle(f);
  return res;
}

bool runFile(const char *file)
{
  (void) file;
  char *exe = file_path(file);
  if(exe == NULL){
    return false;
  }
  char *q_exe = NULL;
  if(asprintf(&q_exe, "\"%s\"", exe) < 0){
    free(exe);
    return false;
  }
  free(exe);
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  ZeroMemory(&pi, sizeof(pi));
printf("Going to run this: %s\n", q_exe);
  bool res = CreateProcess(NULL, q_exe, NULL, NULL, false, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
  if(!res){
    printf("Failed! (%ld)\n", (long)GetLastError());
  }
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  CloseHandle(si.hStdInput);
  CloseHandle(si.hStdOutput);
  CloseHandle(si.hStdError);
  free(q_exe);
  return res;
}

