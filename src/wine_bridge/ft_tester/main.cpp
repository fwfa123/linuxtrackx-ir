#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdio>
#include <stdint.h>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#ifdef __MINGW32__
#include <direct.h>
#define MKDIR(p, m) _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(p, m) mkdir(p, m)
#endif
#include <cstdlib>

#include "resource.h"
#include "../client/rest.h"

// Use shared helper from rest.c instead of duplicating parsing logic

HINSTANCE hInst;
UINT_PTR timer = 0;

HMODULE ftclient;
static void ensure_parent_dirs(const char *filepath)
{
  if(!filepath) return;
  char buf[4096];
  snprintf(buf, sizeof(buf), "%s", filepath);
  // strip filename
  char *last_slash = strrchr(buf, '/');
  if(!last_slash) return;
  *last_slash = '\0';
  // create recursively
  char tmp[4096]; size_t len = strlen(buf);
  size_t i = 0; size_t j = 0;
  while(i < len){
    if(buf[i] == '/'){
      if(j > 0){ tmp[j] = '\0'; MKDIR(tmp, 0700); }
    }else{
      tmp[j++] = buf[i];
    }
    i++;
  }
  if(j > 0){ tmp[j] = '\0'; MKDIR(tmp, 0700); }
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
      char *p1 = strrchr(exe_path, '\\');
      char *p2 = strrchr(exe_path, '/');
      char *cut = p1; if(p2 && (!cut || p2 > cut)) cut = p2;
      if(cut){ *cut = '\0'; }
      snprintf(path, sizeof(path), "%s\\tester_autofill.log", exe_path);
      f = fopen(path, "a");
      if(!f){ snprintf(path, sizeof(path), "%s/tester_autofill.log", exe_path); f = fopen(path, "a"); }
    }
  }
  if(!f){ return; }
  time_t t = time(NULL);
  struct tm *tmv = localtime(&t);
  char ts[64];
  if(tmv){ strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", tmv); } else { snprintf(ts, sizeof(ts), "-"); }
  fprintf(f, "%s [FT_Tester] ", ts);
  va_list ap; va_start(ap, fmt);
  vfprintf(f, fmt, ap);
  va_end(ap);
  fputc('\n', f);
  fflush(f);
  fclose(f);
}

typedef struct
{
	unsigned int dataID;
	int res_x; int res_y;
	float yaw; // positive yaw to the left
	float pitch;// positive pitch up
	float roll;// positive roll to the left
	float x;
	float y;
	float z;
    // raw pose with no smoothing, sensitivity, response curve etc.
	float ryaw;
	float rpitch;
	float rroll;
	float rx;
	float ry;
	float rz;
    // raw points, sorted by Y, origin top left corner
	float x0, y0;
	float x1, y1;
	float x2, y2;
	float x3, y3;
}FreeTrackData;


typedef bool (WINAPI *importGetData)(FreeTrackData * data);
typedef char *(WINAPI *importGetDllVersion)(void);
typedef void (WINAPI *importReportName)(char *name);
typedef char *(WINAPI *importProvider)(void);

importGetData getData;
importGetDllVersion getDllVersion;
importReportName	reportName;
importProvider provider;


char *client_path()
{
  HKEY  hkey   = 0;
  RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Freetrack\\FreetrackClient", 0,
    KEY_QUERY_VALUE, &hkey);
  if(!hkey){
    printf("Can't open registry key\n");
    return NULL;
  }

  BYTE path[1024];
  DWORD buf_len = 1024;
  LONG result = RegQueryValueEx(hkey, "Path", NULL, NULL, path, &buf_len);
  char *full_path = (char *)malloc(2048);
  if(result == ERROR_SUCCESS && buf_len > 0){
    sprintf(full_path, "%s\\FreeTrackClient.dll", path);
  }
  RegCloseKey(hkey);
  return full_path;
}


bool start(HWND hwnd)
{
  char *libname = client_path();
  if(libname == NULL){
    printf("Freetrack client not found!\n");
    return false;
  }
  ftclient = LoadLibrary(libname);
  if(ftclient == NULL){
    printf("Couldn't load Freetrack client library '%s'!\n", libname);
    return false;
  }
  printf("Freetrack client library %s loaded.\n", client_path());


  getData = (importGetData)(void*)GetProcAddress(ftclient, "FTGetData");
  getDllVersion = (importGetDllVersion)(void*)GetProcAddress(ftclient, "FTGetDllVersion");
  reportName = (importReportName)(void*)GetProcAddress(ftclient, "FTReportName");
  provider = (importProvider)(void*)GetProcAddress(ftclient, "FTProvider");

  if((getData == NULL) || (getDllVersion == NULL) || (reportName == NULL) || (provider == NULL)){
    printf("Couldn't load Freetrack client functions!\n");
    FreeLibrary(ftclient);
    return false;
  }

  printf("Dll version: %s\n", getDllVersion());
  printf("Provider: %s\n", provider());
  char title[1024];
  GetDlgItemText(hwnd, IDC_TITLE, title, 1020);
  reportName(title);
  return true;
}

void reportError(std::string msg)
{
  MessageBoxA(0, "FreeTrack client test", msg.c_str(), 0);
}
VOID CALLBACK TimerProcedure(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  (void) uMsg;
  (void) idEvent;
  (void) dwTime;
  FreeTrackData d;
  getData(&d);
  SetDlgItemInt(hwnd, IDC_PITCH, d.pitch, true);
  SetDlgItemInt(hwnd, IDC_ROLL, d.roll, true);
  SetDlgItemInt(hwnd, IDC_YAW, d.yaw, true);

  SetDlgItemInt(hwnd, IDC_X, d.x, true);
  SetDlgItemInt(hwnd, IDC_Y, d.y, true);
  SetDlgItemInt(hwnd, IDC_Z, d.z, true);

  SetDlgItemInt(hwnd, IDC_RPITCH, d.rpitch, true);
  SetDlgItemInt(hwnd, IDC_RROLL, d.rroll, true);
  SetDlgItemInt(hwnd, IDC_RYAW, d.ryaw, true);

  SetDlgItemInt(hwnd, IDC_RX, d.rx, true);
  SetDlgItemInt(hwnd, IDC_RY, d.ry, true);
  SetDlgItemInt(hwnd, IDC_RZ, d.rz, true);
  
  std::ostringstream s; 
  s.str(std::string());
  s<<"("<<std::fixed<<std::setprecision(1)<<d.x0<<"; "<<d.y0<<")";
  SetDlgItemText(hwnd, IDC_PT0, s.str().c_str());

  s.str(std::string());
  s<<"("<<std::fixed<<std::setprecision(1)<<d.x1<<"; "<<d.y1<<")";
  SetDlgItemText(hwnd, IDC_PT1, s.str().c_str());

  s.str(std::string());
  s<<"("<<std::fixed<<std::setprecision(1)<<d.x2<<"; "<<d.y2<<")";
  SetDlgItemText(hwnd, IDC_PT2, s.str().c_str());

  s.str(std::string());
  s<<"("<<std::fixed<<std::setprecision(1)<<d.x3<<"; "<<d.y3<<")";
  SetDlgItemText(hwnd, IDC_PT3, s.str().c_str());

  s.str(std::string());
  s<<d.res_x<<"x"<<d.res_y;
  SetDlgItemText(hwnd, IDC_RES, s.str().c_str());
  SetDlgItemInt(hwnd, IDC_NUM, d.dataID, true);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) lParam;
    switch(uMsg)
    {
        case WM_INITDIALOG:
            SetDlgItemText(hwndDlg, IDC_TITLE, "Default");
            {
              int initial_id = 2307;
              char *env_id = getenv("LTR_GAME_ID");
              if(env_id && *env_id){
                int v = atoi(env_id);
                if(v > 0){ initial_id = v; printf("INFO: Using LTR_GAME_ID=%d\n", v); append_log("Using LTR_GAME_ID=%d", v); }
              }else{
                // Steam hint first
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
              SetDlgItemInt(hwndDlg, IDC_NUM, (UINT)initial_id, TRUE);
            }
            return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                /*
                 * TODO: Add more control ID's, when needed.
                 */
                case IDQUIT:
                    FreeLibrary(ftclient);
                    EndDialog(hwndDlg, 0);
                    return TRUE;
                case IDC_START:
                  start(hwndDlg);
//l                  int ok;
//                  int num = GetDlgItemInt(hwndDlg, IDC_APPID, (BOOL*)&ok, false);
                  if(timer != 0){
                    KillTimer(hwndDlg, timer);
                    timer = 0;
                  }
                  timer = SetTimer(hwndDlg, 0, 50, TimerProcedure);
                  break;

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
  return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);
}


