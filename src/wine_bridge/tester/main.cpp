#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "resource.h"
#include "rest.h"
#include "npifc.h"
#include "../unix_config_home.h"
#include <cstdlib>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#ifdef __MINGW32__
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

static void create_dir(const char *path)
{
  if(!path || !*path) return;
#ifdef _WIN32
  CreateDirectoryA(path, NULL);
#else
  mkdir(path, 0700);
#endif
}

static void ensure_parent_dirs(const char *filepath)
{
  if(!filepath) return;
  char buf[4096];
  snprintf(buf, sizeof(buf), "%s", filepath);
  char *last_slash = strrchr(buf, '/');
  char *last_bslash = strrchr(buf, '\\');
  if(last_bslash && (!last_slash || last_bslash > last_slash)) last_slash = last_bslash;
  if(!last_slash) return;
  *last_slash = '\0';
  // Only needed for POSIX-style paths; on Windows we'll skip recursive creation here
#ifndef _WIN32
  char partial[4096]; size_t k = 0;
  size_t i = 0; if(buf[0] == '/'){ partial[k++] = '/'; i = 1; }
  for(; buf[i] != '\0'; ++i){
    partial[k++] = buf[i];
    if(buf[i] == '/' && k > 1){ partial[k] = '\0'; create_dir(partial); }
  }
  if(k > 0 && partial[k-1] != '/'){ partial[k] = '\0'; create_dir(partial); }
#endif
}

static void append_log(const char *fmt, ...)
{
  const char *home = ltr_unix_home_for_config();
  char path[4096];
  snprintf(path, sizeof(path), "%s/.config/linuxtrack/tester_autofill.log", home);
  ensure_parent_dirs(path);
  FILE *f = fopen(path, "a");
  if(!f){
    // Fallback to WINEPREFIX
    const char *prefix = getenv("WINEPREFIX");
    if(prefix && *prefix){
      snprintf(path, sizeof(path), "%s/drive_c/linuxtrack/tester_autofill.log", prefix);
      ensure_parent_dirs(path);
      f = fopen(path, "a");
    }
  }
  if(!f){
    // Final fallback: same directory as the running executable (Wine path)
    char exe_path[1024];
    DWORD n = GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));
    if(n > 0 && n < sizeof(exe_path)){
      // Trim to directory
      char *p1 = strrchr(exe_path, '\\');
      char *p2 = strrchr(exe_path, '/');
      char *cut = p1; if(p2 && (!cut || p2 > cut)) cut = p2;
      if(cut){ *cut = '\0'; }
      snprintf(path, sizeof(path), "%s\\tester_autofill.log", exe_path);
      f = fopen(path, "a");
      if(!f){
        // Try forward slash variant
        snprintf(path, sizeof(path), "%s/tester_autofill.log", exe_path);
        f = fopen(path, "a");
      }
    }
  }
  if(!f){ return; }
  time_t t = time(NULL);
  struct tm *tmv = localtime(&t);
  char ts[64];
  if(tmv){ strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", tmv); } else { snprintf(ts, sizeof(ts), "-"); }
  fprintf(f, "%s [Tester.exe] ", ts);
  va_list ap; va_start(ap, fmt);
  vfprintf(f, fmt, ap);
  va_end(ap);
  fputc('\n', f);
  fflush(f);
  fclose(f);
}

static void normalize_game_name(const char *raw, char *out, size_t out_sz)
{
  if(!out || out_sz == 0){
    return;
  }
  out[0] = '\0';
  if(!raw || !*raw){
    return;
  }

  size_t j = 0;
  for(size_t i = 0; raw[i] != '\0' && j + 1 < out_sz; ++i){
    char ch = raw[i];
    if(ch >= 'A' && ch <= 'Z'){
      ch = (char)(ch - 'A' + 'a');
    }
    if(ch == '_' || ch == '-' || ch == ':' || ch == ';' || ch == ',' || ch == '.'){
      ch = ' ';
    }
    out[j++] = ch;
  }
  out[j] = '\0';
}

static void fix_assualt_typo(char *normalized)
{
  if(!normalized){
    return;
  }
  for(char *p = strstr(normalized, "assualt"); p != NULL; p = strstr(p + 1, "assualt")){
    memcpy(p, "assault", strlen("assault"));
  }
}

// Steam / storefront title "Arma: Cold War Assault" is Operation Flashpoint; generic
// name matching often mis-resolves to ArmA 2 (7502), wrong crypto keys -> no pose data.
static bool name_hints_arma_cwa_or_ofp(const char *normalized)
{
  if(!normalized || !*normalized){
    return false;
  }
  if(strstr(normalized, "operation flashpoint") != NULL){
    return true;
  }
  bool has_arma = strstr(normalized, "arma") != NULL;
  bool has_cold_war = strstr(normalized, "cold war") != NULL;
  return has_arma && has_cold_war;
}

static bool try_cwa_ofp_gamedata_aliases(const char *orig_key, const char *normalized,
                                         int *found_id, bool from_slug)
{
  if(!name_hints_arma_cwa_or_ofp(normalized) || !found_id){
    return false;
  }
  /* Order: storefront title first; "operation flashpoint" only as exact normalized
   * catalog match so fuzzy logic cannot pick "Operation Flashpoint 2 (inactive)". */
  static const char *aliases[] = {
    "arma cold war assault",
    "operation flashpoint resistance",
    "operation flashpoint cold war assault",
    "operation flashpoint",
  };
  for(size_t i = 0; i < sizeof(aliases) / sizeof(aliases[0]); ++i){
    if(game_data_find_id_by_normalized_exact(aliases[i], found_id) && *found_id > 0){
      if(from_slug){
        append_log("Using LTR_GAME_SLUG CWA/OFP alias '%s' => TrackIR ID=%d (slug='%s')",
                   aliases[i], *found_id, orig_key);
      }else{
        append_log("Using LTR_GAME_NAME CWA/OFP alias '%s' => TrackIR ID=%d (from '%s')",
                   aliases[i], *found_id, orig_key);
      }
      return true;
    }
  }
  return false;
}

enum {
  NAME_LOOKUP_NONE = 0,
  NAME_LOOKUP_ALIAS = 1,
  NAME_LOOKUP_CANDIDATE = 2
};

static int try_game_name_with_aliases(const char *env_name, int *found_id)
{
  if(!env_name || !*env_name || !found_id){
    return NAME_LOOKUP_NONE;
  }

  char normalized[512];
  normalize_game_name(env_name, normalized, sizeof(normalized));
  fix_assualt_typo(normalized);

  if(try_cwa_ofp_gamedata_aliases(env_name, normalized, found_id, false)){
    return NAME_LOOKUP_ALIAS;
  }

  if(game_data_find_id_by_name(env_name, found_id) && *found_id > 0){
    append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d (query='%s')",
               env_name, *found_id, env_name);
    return NAME_LOOKUP_CANDIDATE;
  }

  if(normalized[0] != '\0' && strcmp(normalized, env_name) != 0){
    if(game_data_find_id_by_name(normalized, found_id) && *found_id > 0){
      append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d (query='%s')",
                 env_name, *found_id, normalized);
      return NAME_LOOKUP_CANDIDATE;
    }
  }

  return NAME_LOOKUP_NONE;
}

static int try_game_slug_with_aliases(const char *env_slug, int *found_id)
{
  if(!env_slug || !*env_slug || !found_id){
    return NAME_LOOKUP_NONE;
  }

  char normalized[512];
  normalize_game_name(env_slug, normalized, sizeof(normalized));
  fix_assualt_typo(normalized);

  if(try_cwa_ofp_gamedata_aliases(env_slug, normalized, found_id, true)){
    return NAME_LOOKUP_ALIAS;
  }

  if(normalized[0] != '\0'){
    if(game_data_find_id_by_name(normalized, found_id) && *found_id > 0){
      append_log("Using LTR_GAME_SLUG match '%s' => TrackIR ID=%d (query='%s')",
                 env_slug, *found_id, normalized);
      return NAME_LOOKUP_CANDIDATE;
    }
  }

  return NAME_LOOKUP_NONE;
}

static void release_game_desc(game_desc_t *gd)
{
  if(gd && gd->name){
    free((void *)gd->name);
    gd->name = NULL;
  }
}

/* Same folding as game_data_find_id_by_normalized_exact's catalog names. */
static void normalize_catalog_name_for_match(char *out, size_t cap, const char *in)
{
  if(!out || cap < 2 || !in){
    if(out && cap > 0){
      out[0] = '\0';
    }
    return;
  }
  size_t j = 0;
  for(size_t i = 0; in[i] != '\0' && j + 1 < cap; ++i){
    char ch = in[i];
    if(ch >= 'A' && ch <= 'Z'){
      ch = (char)(ch - 'A' + 'a');
    }
    if(ch == '_' || ch == '-' || ch == ':' || ch == ';' || ch == ',' || ch == '.'
       || ch == '(' || ch == ')' || ch == '\t' || ch == '\r' || ch == '\n'){
      ch = ' ';
    }
    out[j++] = ch;
  }
  out[j] = '\0';
  char *w = out;
  bool prev_space = true;
  for(char *r = out; *r; ++r){
    if(*r == ' '){
      if(!prev_space){
        *w++ = ' ';
        prev_space = true;
      }
    }else{
      *w++ = *r;
      prev_space = false;
    }
  }
  if(w > out && w[-1] == ' '){
    --w;
  }
  *w = '\0';
}

static size_t longest_common_substring(const char *a, const char *b)
{
  size_t best = 0;
  if(!a || !b){
    return 0;
  }
  size_t la = strlen(a);
  size_t lb = strlen(b);
  for(size_t i = 0; i < la; ++i){
    for(size_t j = 0; j < lb; ++j){
      size_t k = 0;
      while(i + k < la && j + k < lb && a[i + k] == b[j + k]){
        ++k;
      }
      if(k > best){
        best = k;
      }
    }
  }
  return best;
}

/* Longest shared run divided by the longer normalized name. 1.0 is identical. */
static double catalog_name_similarity(const char *storefront, const char *catalog)
{
  char left[512];
  char right[512];
  normalize_catalog_name_for_match(left, sizeof(left), storefront ? storefront : "");
  normalize_catalog_name_for_match(right, sizeof(right), catalog ? catalog : "");
  size_t left_len = strlen(left);
  size_t right_len = strlen(right);
  size_t longer = left_len > right_len ? left_len : right_len;
  if(longer == 0){
    return 0.0;
  }
  return (double)longest_common_substring(left, right) / (double)longer;
}

/* weak_under_80: similarity below threshold on a fuzzy ID. !weak_under_80: no list entry. */
static void warn_game_list_issue(HWND hwnd, bool weak_under_80, const char *detail)
{
  const char *lead = weak_under_80
                         ? "The game name match is under 80%%.\r\n\r\n"
                         : "No matching game was found in the TrackIR game list.\r\n\r\n";
  char text[1280];
  snprintf(text, sizeof(text),
           "%s"
           "Either the game list is out of date, or TrackIR may not support this game. "
           "Check the NaturalPoint website. If the game is supported, update the game list "
           "from the Linuxtrack UI using the latest TrackIR installer from NaturalPoint.\r\n\r\n"
           "%s",
           lead,
           detail ? detail : "");
  append_log("Game list warning: %s", detail ? detail : "");
  MessageBoxA(hwnd, text, "Game list match", MB_OK | MB_ICONWARNING);
}

static int resolve_storefront_name(const char *name, const char *slug, int *out_id, const char **out_query)
{
  int found_id = -1;
  if(name && *name){
    int kind = try_game_name_with_aliases(name, &found_id);
    if(kind != NAME_LOOKUP_NONE){
      *out_id = found_id;
      *out_query = name;
      return kind;
    }
  }
  if(slug && *slug){
    int kind = try_game_slug_with_aliases(slug, &found_id);
    if(kind != NAME_LOOKUP_NONE){
      *out_id = found_id;
      *out_query = slug;
      return kind;
    }
  }
  if(name && *name){
    *out_query = name;
  }else if(slug && *slug){
    *out_query = slug;
  }else{
    *out_query = NULL;
  }
  return NAME_LOOKUP_NONE;
}

HINSTANCE hInst;
UINT_PTR timer = 0;

VOID CALLBACK TimerProcedure(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  (void) uMsg;
  (void) idEvent;
  (void) dwTime;
  tir_data_t td;
  int res = npifc_getdata(&td);
  if(res != 0){
    return;
  }
  char buf[64];
  snprintf(buf, sizeof(buf), "%.2f", td.pitch); SetDlgItemText(hwnd, IDC_PITCH, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.roll);  SetDlgItemText(hwnd, IDC_ROLL,  buf);
  snprintf(buf, sizeof(buf), "%.2f", td.yaw);   SetDlgItemText(hwnd, IDC_YAW,   buf);

  snprintf(buf, sizeof(buf), "%.2f", td.tx);    SetDlgItemText(hwnd, IDC_X1, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.ty);    SetDlgItemText(hwnd, IDC_Y1, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.tz);    SetDlgItemText(hwnd, IDC_Z1, buf);

  SetDlgItemInt(hwnd, IDC_S, td.status, true);
  SetDlgItemInt(hwnd, IDC_F, td.frame, true);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) lParam;
    switch(uMsg)
    {
        case WM_INITDIALOG:
            {
              // Default
              int initial_id = 2307;
              const char *env_name_dbg = getenv("LTR_GAME_NAME");
              const char *env_slug_dbg = getenv("LTR_GAME_SLUG");
              append_log("Autofill start: LTR_GAME_NAME='%s' LTR_GAME_SLUG='%s'",
                         env_name_dbg ? env_name_dbg : "<unset>",
                         env_slug_dbg ? env_slug_dbg : "<unset>");
              // Env override: LTR_GAME_ID
              char *env_id = getenv("LTR_GAME_ID");
              if(env_id && *env_id){
                int v = atoi(env_id);
                if(v > 0){ initial_id = v; printf("INFO: Using LTR_GAME_ID=%d\n", v); append_log("Using LTR_GAME_ID=%d", v); }
              }else{
                // Steam hint: SteamAppId / SteamGameId -> mapping file
                char *steam_appid = getenv("SteamAppId");
                if(!(steam_appid && *steam_appid)){
                  steam_appid = getenv("SteamGameId");
                }
                int mapped_id = -1;
                if(steam_appid && *steam_appid && game_data_find_id_by_steam_appid(steam_appid, &mapped_id) && mapped_id > 0){
                  initial_id = mapped_id;
                  printf("INFO: Using Steam mapping SteamAppId=%s => TrackIR ID=%d\n", steam_appid, mapped_id);
                  append_log("Using Steam mapping SteamAppId=%s => TrackIR ID=%d", steam_appid, mapped_id);
                }else{
                  char *env_name = getenv("LTR_GAME_NAME");
                  char *env_slug = getenv("LTR_GAME_SLUG");
                  const char *query = NULL;
                  int found_id = -1;
                  int kind = resolve_storefront_name(env_name, env_slug, &found_id, &query);
                  if(kind == NAME_LOOKUP_ALIAS){
                    initial_id = found_id;
                    printf("INFO: Using alias match '%s' => TrackIR ID=%d\n",
                           query ? query : "", found_id);
                  }else if(kind == NAME_LOOKUP_CANDIDATE){
                    initial_id = found_id;
                    game_desc_t gd;
                    memset(&gd, 0, sizeof(gd));
                    const char *catalog = NULL;
                    if(game_data_get_desc(found_id, &gd) && gd.name){
                      catalog = gd.name;
                    }
                    double similarity = catalog ? catalog_name_similarity(query, catalog) : 0.0;
                    append_log("Name similarity %.3f for '%s' vs catalog '%s' (ID=%d)",
                               similarity, query ? query : "", catalog ? catalog : "", found_id);
                    printf("INFO: Using name match '%s' => TrackIR ID=%d (similarity %.0f%%)\n",
                           query ? query : "", found_id, similarity * 100.0);
                    if(similarity < 0.80){
                      char detail[512];
                      if(catalog){
                        snprintf(detail, sizeof(detail),
                                 "Closest game list entry is '%s' (Application ID %d). "
                                 "That ID is filled in and can be changed before Start.",
                                 catalog, found_id);
                      }else{
                        snprintf(detail, sizeof(detail),
                                 "Closest Application ID is %d, but it has no name in the game list. "
                                 "That ID is filled in and can be changed before Start.",
                                 found_id);
                      }
                      warn_game_list_issue(hwndDlg, true, detail);
                    }
                    release_game_desc(&gd);
                  }else{
                    char detail[512];
                    if(query && *query){
                      snprintf(detail, sizeof(detail),
                               "No game list entry was found for '%s'. "
                               "The Application ID stays at 2307. Start will create that ID's profile, "
                               "or Default if 2307 is not in the game list.",
                               query);
                    }else{
                      snprintf(detail, sizeof(detail),
                               "No game name was provided. "
                               "The Application ID stays at 2307. Start will create that ID's profile, "
                               "or Default if 2307 is not in the game list.");
                    }
                    warn_game_list_issue(hwndDlg, false, detail);
                  }
                }
              }
              if(initial_id == 2307){ printf("INFO: Auto-fill not found; using default TrackIR ID=2307\n"); append_log("Auto-fill not found; using default TrackIR ID=2307"); }
              SetDlgItemInt(hwndDlg, IDC_APPID, (UINT)initial_id, true);
            }
            // Try to show NPClient signature if available
            {
              tir_signature_t sig;
              if(npifc_get_signature_and_version(&sig, NULL)){
                SetDlgItemText(hwndDlg, IDC_DLLSIG, sig.DllSignature);
                SetDlgItemText(hwndDlg, IDC_APPSIG, sig.AppSignature);
              }
            }
            return TRUE;

        case WM_CLOSE:
            if(timer != 0){
              KillTimer(hwndDlg, timer);
              timer = 0;
            }
            npifc_close();
            EndDialog(hwndDlg, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                /*
                 * TODO: Add more control ID's, when needed.
                 */
                case IDQUIT:
                    npifc_close();
                    EndDialog(hwndDlg, 0);
                    return TRUE;
                case IDSTART:
                  {
                    BOOL ok = FALSE;
                    UINT num = GetDlgItemInt(hwndDlg, IDC_APPID, &ok, FALSE);
                    if(!ok){
                      num = 2307;
                    }
                  append_log("Start pressed with AppID=%u", (unsigned int)num);
                  game_desc_t gd;
                  if(timer != 0){
                    KillTimer(hwndDlg, timer);
                    timer = 0;
                  }
                  if(game_data_get_desc(num, &gd)){
                    printf("Application ID: %d - %s\n", num, gd.name);
                    append_log("Resolved AppID=%u to '%s'", (unsigned int)num, gd.name);
                    release_game_desc(&gd);
                  }else{
                    printf("Unknown Application ID: %d\n", num);
                    append_log("game_data_get_desc failed for AppID=%u; continuing with direct NP init", (unsigned int)num);
                    MessageBoxA(hwndDlg,
                                "This Application ID is not in the TrackIR game list.\r\n\r\n"
                                "The profile will be Default, except Application IDs 8106 (Operation Flashpoint) "
                                "and 10601, 7502, and 7503 (ArmA). Encrypted pose data will not use this game's keys.\r\n\r\n"
                                "Tracking will still start.",
                                "Unknown Application ID",
                                MB_OK | MB_ICONWARNING);
                  }
                  if(npifc_init(hwndDlg, num)){
                    append_log("npifc_init succeeded for AppID=%u", (unsigned int)num);
                    UINT_PTR t = SetTimer(hwndDlg, 0, 50, TimerProcedure);
                    if(t == 0){
                      printf("Failed to start timer\n");
                      append_log("SetTimer failed for AppID=%u", (unsigned int)num);
                    }else{
                      timer = t;
                      append_log("SetTimer started (timer=%u) for AppID=%u", (unsigned int)t, (unsigned int)num);
                    }
                  }else{
                    append_log("npifc_init failed for AppID=%u", (unsigned int)num);
                  }
                  break;
                  }

            }
    }

    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  (void) hPrevInstance;
  (void) lpCmdLine;
  (void) nShowCmd;
  hInst = hInstance;
  
  // The user interface is a modal dialog box
  return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)(void*)DialogProc);
}


