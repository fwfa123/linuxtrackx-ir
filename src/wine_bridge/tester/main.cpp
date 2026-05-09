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

static bool try_game_name_with_aliases(const char *env_name, int *found_id)
{
  if(!env_name || !*env_name || !found_id){
    return false;
  }

  if(game_data_find_id_by_name(env_name, found_id) && *found_id > 0){
    append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d (query='%s')",
               env_name, *found_id, env_name);
    return true;
  }

  char normalized[512];
  normalize_game_name(env_name, normalized, sizeof(normalized));

  // Common typo seen in launcher metadata.
  for(char *p = strstr(normalized, "assualt"); p != NULL; p = strstr(p + 1, "assualt")){
    memcpy(p, "assault", strlen("assault"));
  }

  if(normalized[0] != '\0' && strcmp(normalized, env_name) != 0){
    if(game_data_find_id_by_name(normalized, found_id) && *found_id > 0){
      append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d (query='%s')",
                 env_name, *found_id, normalized);
      return true;
    }
  }

  if(strstr(normalized, "arma cold war ass") != NULL){
    *found_id = 10601; // ArmA
    append_log("Using hardcoded ARMA CWA fallback for '%s' => TrackIR ID=%d", env_name, *found_id);
    return true;
  }

  // ARMA CWA aliases often appear under different catalog names.
  if(strstr(normalized, "arma") != NULL && strstr(normalized, "cold war") != NULL){
    const char *aliases[] = {
      "arma cold war assault",
      "operation flashpoint",
      "operation flashpoint resistance"
    };
    for(size_t i = 0; i < sizeof(aliases) / sizeof(aliases[0]); ++i){
      if(game_data_find_id_by_name(aliases[i], found_id) && *found_id > 0){
        append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d (query='%s')",
                   env_name, *found_id, aliases[i]);
        return true;
      }
    }
  }

  return false;
}

static bool try_game_slug_with_aliases(const char *env_slug, int *found_id)
{
  if(!env_slug || !*env_slug || !found_id){
    return false;
  }

  char normalized[512];
  normalize_game_name(env_slug, normalized, sizeof(normalized));

  if(strstr(normalized, "arma cold war ass") != NULL){
    *found_id = 10601; // ArmA
    append_log("Using hardcoded ARMA CWA fallback for slug '%s' => TrackIR ID=%d", env_slug, *found_id);
    return true;
  }

  // Slugs are often hyphen/underscore separated - normalize already turns those into spaces.
  if(normalized[0] != '\0'){
    if(game_data_find_id_by_name(normalized, found_id) && *found_id > 0){
      append_log("Using LTR_GAME_SLUG match '%s' => TrackIR ID=%d (query='%s')",
                 env_slug, *found_id, normalized);
      return true;
    }
  }

  if(strstr(normalized, "arma") != NULL && strstr(normalized, "cold war") != NULL){
    const char *aliases[] = {
      "arma cold war assault",
      "operation flashpoint",
      "operation flashpoint resistance"
    };
    for(size_t i = 0; i < sizeof(aliases) / sizeof(aliases[0]); ++i){
      if(game_data_find_id_by_name(aliases[i], found_id) && *found_id > 0){
        append_log("Using LTR_GAME_SLUG match '%s' => TrackIR ID=%d (query='%s')",
                   env_slug, *found_id, aliases[i]);
        return true;
      }
    }
  }

  return false;
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

  snprintf(buf, sizeof(buf), "%.2f", td.padding[0]); SetDlgItemText(hwnd, IDC_X2, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.padding[1]); SetDlgItemText(hwnd, IDC_Y2, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.padding[2]); SetDlgItemText(hwnd, IDC_Z2, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.padding[3]); SetDlgItemText(hwnd, IDC_X3, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.padding[4]); SetDlgItemText(hwnd, IDC_Y3, buf);
  snprintf(buf, sizeof(buf), "%.2f", td.padding[5]); SetDlgItemText(hwnd, IDC_Z3, buf);
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
                  // Env hint: LTR_GAME_NAME -> lookup in gamedata.txt
                  char *env_name = getenv("LTR_GAME_NAME");
                  if(env_name && *env_name){
                    int found_id = -1;
                    if(try_game_name_with_aliases(env_name, &found_id)){
                      initial_id = found_id;
                      printf("INFO: Using LTR_GAME_NAME match '%s' => TrackIR ID=%d\n", env_name, found_id);
                    }
                  }
                  if(initial_id == 2307){
                    char *env_slug = getenv("LTR_GAME_SLUG");
                    if(env_slug && *env_slug){
                      int found_id = -1;
                      if(try_game_slug_with_aliases(env_slug, &found_id)){
                        initial_id = found_id;
                        printf("INFO: Using LTR_GAME_SLUG match '%s' => TrackIR ID=%d\n", env_slug, found_id);
                      }
                    }
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
                  }else{
                    printf("Unknown Application ID: %d\n", num);
                    append_log("game_data_get_desc failed for AppID=%u; continuing with direct NP init", (unsigned int)num);
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


