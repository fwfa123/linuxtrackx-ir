#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "resource.h"
#include "rest.h"
#include "npifc.h"
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
  const char *home = getenv("HOME");
  if(!home) home = getenv("USERPROFILE");
  if(!home) home = ".";
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
                    if(game_data_find_id_by_name(env_name, &found_id) && found_id > 0){
                      initial_id = found_id;
                      printf("INFO: Using LTR_GAME_NAME match '%s' => TrackIR ID=%d\n", env_name, found_id);
                      append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d", env_name, found_id);
                    }
                  }
                }
              }
              if(initial_id == 2307){ printf("INFO: Auto-fill not found; using default TrackIR ID=2307\n"); append_log("Auto-fill not found; using default TrackIR ID=2307"); }
              SetDlgItemInt(hwndDlg, IDC_APPID, (UINT)initial_id, true);
            }
            // Try to show NPClient signature/version if available
            {
              tir_signature_t sig;
              unsigned short ver = 0;
              if(npifc_get_signature_and_version(&sig, &ver)){
                SetDlgItemText(hwndDlg, IDC_DLLSIG, sig.DllSignature);
                SetDlgItemText(hwndDlg, IDC_APPSIG, sig.AppSignature);
                SetDlgItemInt(hwndDlg, IDC_VER, ver, FALSE);
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
                  game_desc_t gd;
                  if(timer != 0){
                    KillTimer(hwndDlg, timer);
                    timer = 0;
                  }
                  if(game_data_get_desc(num, &gd)){
                    printf("Application ID: %d - %s\n", num, gd.name);
                    if(npifc_init(hwndDlg, num)){
                      UINT_PTR t = SetTimer(hwndDlg, 0, 50, TimerProcedure);
                      if(t == 0){
                        printf("Failed to start timer\n");
                      }else{
                        timer = t;
                      }
                    }
                  }else{
                    printf("Unknown Application ID: %d\n", num);
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


