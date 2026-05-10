#ifdef __MINGW32__

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../unix_config_home.h"

typedef struct {
  const char *key;
  const char *value;
} mingw_default_pref_t;

typedef struct {
  char section[64];
  char key[128];
  char value[256];
} mingw_pref_entry_t;

static const mingw_default_pref_t mingw_default_prefs[] = {
  {"Title", "Default"},
  {"Pitch-enabled", "Yes"},
  {"Pitch-inverted", "No"},
  {"Pitch-deadzone", "0.0"},
  {"Pitch-left-curvature", "0.5"},
  {"Pitch-right-curvature", "0.5"},
  {"Pitch-sensitivity", "5.000000"},
  {"Pitch-left-limit", "80.000000"},
  {"Pitch-right-limit", "80.000000"},
  {"Pitch-filter", "0.3"},
  {"Yaw-enabled", "Yes"},
  {"Yaw-inverted", "No"},
  {"Yaw-deadzone", "0.0"},
  {"Yaw-left-curvature", "0.5"},
  {"Yaw-right-curvature", "0.5"},
  {"Yaw-sensitivity", "5.000000"},
  {"Yaw-left-limit", "130.000000"},
  {"Yaw-right-limit", "130.000000"},
  {"Yaw-filter", "0.3"},
  {"Roll-enabled", "Yes"},
  {"Roll-inverted", "No"},
  {"Roll-deadzone", "0.0"},
  {"Roll-left-curvature", "0.5"},
  {"Roll-right-curvature", "0.5"},
  {"Roll-sensitivity", "1.500000"},
  {"Roll-left-limit", "45.000000"},
  {"Roll-right-limit", "45.000000"},
  {"Roll-filter", "0.3"},
  {"Xtranslation-enabled", "Yes"},
  {"Xtranslation-inverted", "No"},
  {"Xtranslation-deadzone", "0.0"},
  {"Xtranslation-left-curvature", "0.5"},
  {"Xtranslation-right-curvature", "0.5"},
  {"Xtranslation-sensitivity", "5.000000"},
  {"Xtranslation-left-limit", "300.000000"},
  {"Xtranslation-right-limit", "300.000000"},
  {"Xtranslation-filter", "0.3"},
  {"Ytranslation-enabled", "Yes"},
  {"Ytranslation-inverted", "No"},
  {"Ytranslation-deadzone", "0.0"},
  {"Ytranslation-left-curvature", "0.5"},
  {"Ytranslation-right-curvature", "0.5"},
  {"Ytranslation-sensitivity", "5.000000"},
  {"Ytranslation-left-limit", "300.000000"},
  {"Ytranslation-right-limit", "300.000000"},
  {"Ytranslation-filter", "0.3"},
  {"Ztranslation-enabled", "Yes"},
  {"Ztranslation-inverted", "No"},
  {"Ztranslation-deadzone", "0.0"},
  {"Ztranslation-left-curvature", "0.5"},
  {"Ztranslation-right-curvature", "0.5"},
  {"Ztranslation-sensitivity", "2.000000"},
  {"Ztranslation-left-limit", "300.000000"},
  {"Ztranslation-right-limit", "1.000000"},
  {"Ztranslation-filter", "0.7"},
  {NULL, NULL}
};

static char mingw_profile_section[64] = "Profile1";
static char mingw_profile_title[256] = "Default";
static mingw_pref_entry_t mingw_pref_entries[512];
static size_t mingw_pref_entry_count;
static bool mingw_prefs_loaded;

void ltr_int_log_message(const char *format, ...);

static char *mingw_strdup(const char *s)
{
  if(s == NULL){
    return NULL;
  }
  size_t len = strlen(s) + 1;
  char *copy = (char *)malloc(len);
  if(copy != NULL){
    memcpy(copy, s, len);
  }
  return copy;
}

static char *mingw_trim(char *s)
{
  while(*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'){
    ++s;
  }
  char *end = s + strlen(s);
  while(end > s && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n')){
    --end;
  }
  *end = '\0';
  return s;
}

static void mingw_unix_path_to_wine_z(const char *unix_path, char *wine_path, size_t wine_path_size)
{
  if(unix_path == NULL || wine_path == NULL || wine_path_size == 0){
    return;
  }
  size_t out = 0;
  if(wine_path_size > 3){
    wine_path[out++] = 'Z';
    wine_path[out++] = ':';
  }
  for(size_t in = 0; unix_path[in] != '\0' && out + 1 < wine_path_size; ++in){
    wine_path[out++] = (unix_path[in] == '/') ? '\\' : unix_path[in];
  }
  wine_path[out] = '\0';
}

static void mingw_pref_store(const char *section, const char *key, const char *value)
{
  if(section == NULL || key == NULL || value == NULL ||
     mingw_pref_entry_count >= sizeof(mingw_pref_entries) / sizeof(mingw_pref_entries[0])){
    return;
  }
  mingw_pref_entry_t *entry = &mingw_pref_entries[mingw_pref_entry_count++];
  snprintf(entry->section, sizeof(entry->section), "%s", section);
  snprintf(entry->key, sizeof(entry->key), "%s", key);
  snprintf(entry->value, sizeof(entry->value), "%s", value);
}

static FILE *mingw_open_prefs_file(char *opened_path, size_t opened_path_size)
{
  const char *home = ltr_unix_home_for_config();
  char path[512];
  if((size_t)snprintf(path, sizeof(path), "%s/.config/linuxtrack/linuxtrack1.conf", home) >= sizeof(path)){
    return NULL;
  }
  FILE *f = fopen(path, "r");
  if(f != NULL){
    if(opened_path != NULL && opened_path_size > 0){
      snprintf(opened_path, opened_path_size, "%s", path);
    }
    return f;
  }
  char wine_path[512];
  mingw_unix_path_to_wine_z(path, wine_path, sizeof(wine_path));
  f = fopen(wine_path, "r");
  if(f != NULL && opened_path != NULL && opened_path_size > 0){
    snprintf(opened_path, opened_path_size, "%s", wine_path);
  }
  return f;
}

static void mingw_load_prefs(void)
{
  if(mingw_prefs_loaded){
    return;
  }
  mingw_prefs_loaded = true;
  char opened_path[512] = {0};
  FILE *f = mingw_open_prefs_file(opened_path, sizeof(opened_path));
  if(f == NULL){
    ltr_int_log_message("prefs: linuxtrack1.conf not reachable, using built-in defaults\n");
    return;
  }

  char section[64] = "";
  char line[768];
  while(fgets(line, sizeof(line), f) != NULL){
    char *trimmed = mingw_trim(line);
    if(trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';'){
      continue;
    }
    if(trimmed[0] == '['){
      char *end = strchr(trimmed, ']');
      if(end != NULL){
        *end = '\0';
        snprintf(section, sizeof(section), "%s", mingw_trim(trimmed + 1));
      }
      continue;
    }
    char *eq = strchr(trimmed, '=');
    if(eq == NULL || section[0] == '\0'){
      continue;
    }
    *eq = '\0';
    char *key = mingw_trim(trimmed);
    char *value = mingw_trim(eq + 1);
    if(key[0] != '\0'){
      mingw_pref_store(section, key, value);
    }
  }
  fclose(f);
  ltr_int_log_message("prefs: loaded %u entries from %s\n",
                      (unsigned int)mingw_pref_entry_count, opened_path);
}

static char *mingw_pref_lookup(const char *section_name, const char *key_name)
{
  mingw_load_prefs();
  for(size_t i = 0; i < mingw_pref_entry_count; ++i){
    if(strcmp(mingw_pref_entries[i].section, section_name) == 0 &&
       strcmp(mingw_pref_entries[i].key, key_name) == 0){
      return mingw_strdup(mingw_pref_entries[i].value);
    }
  }
  return NULL;
}

void *ltr_int_my_malloc(size_t size)
{
  return calloc(1, size);
}

char *ltr_int_my_strdup(const char *s)
{
  return mingw_strdup(s);
}

char *ltr_int_my_strcat(const char *str1, const char *str2)
{
  if(str1 == NULL || str2 == NULL){
    return NULL;
  }
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  char *res = (char *)malloc(len1 + len2 + 1);
  if(res == NULL){
    return NULL;
  }
  memcpy(res, str1, len1);
  memcpy(res + len1, str2, len2 + 1);
  return res;
}

void ltr_int_valog_message(const char *format, va_list va)
{
  FILE *f = fopen("/tmp/linuxtrack_npclient.log", "a");
  if(f != NULL){
    fputs("axes: ", f);
    vfprintf(f, format, va);
    fclose(f);
  }
}

void ltr_int_log_message(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  ltr_int_valog_message(format, va);
  va_end(va);
}

char *ltr_int_get_key(const char *section_name, const char *key_name)
{
  if(section_name == NULL || key_name == NULL){
    return NULL;
  }
  char *stored = mingw_pref_lookup(section_name, key_name);
  if(stored != NULL){
    return stored;
  }
  if(strcmp(section_name, "Default") == 0){
    for(size_t i = 0; mingw_default_prefs[i].key != NULL; ++i){
      if(strcmp(mingw_default_prefs[i].key, key_name) == 0){
        return mingw_strdup(mingw_default_prefs[i].value);
      }
    }
  }
  if(strcmp(section_name, mingw_profile_section) == 0 && strcmp(key_name, "Title") == 0){
    return mingw_strdup(mingw_profile_title);
  }
  return NULL;
}

bool ltr_int_get_key_flt(const char *section_name, const char *key_name, float *val)
{
  char *str = ltr_int_get_key(section_name, key_name);
  if(str == NULL){
    return false;
  }
  char *end = NULL;
  float parsed = strtof(str, &end);
  bool ok = (end != str);
  free(str);
  if(ok && val != NULL){
    *val = parsed;
  }
  return ok;
}

bool ltr_int_get_key_int(const char *section_name, const char *key_name, int *val)
{
  char *str = ltr_int_get_key(section_name, key_name);
  if(str == NULL){
    return false;
  }
  char *end = NULL;
  long parsed = strtol(str, &end, 10);
  bool ok = (end != str);
  free(str);
  if(ok && val != NULL){
    *val = (int)parsed;
  }
  return ok;
}

bool ltr_int_change_key(const char *section_name, const char *key_name, const char *new_value)
{
  if(section_name != NULL && key_name != NULL && new_value != NULL &&
     strcmp(section_name, mingw_profile_section) == 0 && strcmp(key_name, "Title") == 0){
    snprintf(mingw_profile_title, sizeof(mingw_profile_title), "%s", new_value);
  }
  return true;
}

bool ltr_int_change_key_flt(const char *section_name, const char *key_name, float new_value)
{
  (void)section_name;
  (void)key_name;
  (void)new_value;
  return true;
}

bool ltr_int_change_key_int(const char *section_name, const char *key_name, int new_value)
{
  (void)section_name;
  (void)key_name;
  (void)new_value;
  return true;
}

char *ltr_int_find_section(const char *key_name, const char *value)
{
  if(key_name == NULL || value == NULL || strcmp(key_name, "Title") != 0){
    return NULL;
  }
  mingw_load_prefs();
  for(size_t i = 0; i < mingw_pref_entry_count; ++i){
    if(strcmp(mingw_pref_entries[i].key, key_name) == 0 &&
       strcmp(mingw_pref_entries[i].value, value) == 0){
      return mingw_strdup(mingw_pref_entries[i].section);
    }
  }
  if(strcmp(value, "Default") == 0){
    return mingw_strdup("Default");
  }
  if(strcmp(value, mingw_profile_title) == 0){
    return mingw_strdup(mingw_profile_section);
  }
  return NULL;
}

char *ltr_int_add_unique_section(const char *name_template)
{
  snprintf(mingw_profile_section, sizeof(mingw_profile_section), "%s1",
           (name_template != NULL && name_template[0] != '\0') ? name_template : "Profile");
  return mingw_strdup(mingw_profile_section);
}

void ltr_int_free_prefs(void) {}
bool ltr_int_read_prefs(const char *file, bool force_read)
{
  (void)file;
  (void)force_read;
  return true;
}
bool ltr_int_new_prefs(void) { return true; }
bool ltr_int_save_prefs(const char *fname)
{
  (void)fname;
  return true;
}
bool ltr_int_dump_prefs(const char *file_name)
{
  (void)file_name;
  return true;
}
bool ltr_int_need_saving(void) { return false; }
bool ltr_int_find_sections(const char *key_name, void *result)
{
  (void)key_name;
  (void)result;
  return false;
}
void ltr_int_prefs_changed(void) {}
void ltr_int_get_section_list(void *sections_ptr)
{
  (void)sections_ptr;
}

#endif
