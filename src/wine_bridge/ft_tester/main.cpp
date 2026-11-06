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
#include "../unix_config_home.h"

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

static const char *catalog_name_for_id(int id, game_desc_t *gd)
{
  memset(gd, 0, sizeof(*gd));
  if(game_data_get_desc(id, gd) && gd->name && gd->name[0] != '\0'){
    return gd->name;
  }
  return NULL;
}

static void set_profile_title(HWND hwnd, const char *title)
{
  SetDlgItemTextA(hwnd, IDC_TITLE, (title && title[0] != '\0') ? title : "Default");
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) lParam;
    switch(uMsg)
    {
        case WM_INITDIALOG:
            SetDlgItemTextA(hwndDlg, IDC_TITLE, "Default");
            {
              const char *env_name = getenv("LTR_GAME_NAME");
              const char *env_slug = getenv("LTR_GAME_SLUG");
              const char *storefront = (env_name && *env_name) ? env_name : NULL;
              append_log("Autofill start: LTR_GAME_NAME='%s' LTR_GAME_SLUG='%s'",
                         env_name ? env_name : "<unset>",
                         env_slug ? env_slug : "<unset>");
              char *env_id = getenv("LTR_GAME_ID");
              bool named_from_list = false;
              int chosen_id = -1;
              if(env_id && *env_id){
                int v = atoi(env_id);
                if(v > 0){
                  chosen_id = v;
                  named_from_list = true;
                  printf("INFO: Using LTR_GAME_ID=%d\n", v);
                  append_log("Using LTR_GAME_ID=%d", v);
                }
              }
              if(!named_from_list){
                char *steam_appid = getenv("SteamAppId");
                if(!(steam_appid && *steam_appid)){
                  steam_appid = getenv("SteamGameId");
                }
                int mapped_id = -1;
                if(steam_appid && *steam_appid && game_data_find_id_by_steam_appid(steam_appid, &mapped_id) && mapped_id > 0){
                  chosen_id = mapped_id;
                  named_from_list = true;
                  printf("INFO: Using Steam mapping SteamAppId=%s => TrackIR ID=%d\n", steam_appid, mapped_id);
                  append_log("Using Steam mapping SteamAppId=%s => TrackIR ID=%d", steam_appid, mapped_id);
                }
              }
              if(named_from_list){
                game_desc_t gd;
                const char *catalog = catalog_name_for_id(chosen_id, &gd);
                set_profile_title(hwndDlg, catalog ? catalog : storefront);
                append_log("Profile title '%s' from trusted ID=%d",
                           catalog ? catalog : (storefront ? storefront : "Default"), chosen_id);
                release_game_desc(&gd);
              }else{
                const char *query = NULL;
                int found_id = -1;
                int kind = resolve_storefront_name(env_name, env_slug, &found_id, &query);
                if(kind == NAME_LOOKUP_NONE){
                  char detail[512];
                  if(query && *query){
                    snprintf(detail, sizeof(detail),
                             "No game list entry was found for '%s'. "
                             "The profile title will be '%s'.",
                             query, storefront ? storefront : "Default");
                  }else{
                    snprintf(detail, sizeof(detail),
                             "No game name was provided. The profile title will be Default.");
                  }
                  warn_game_list_issue(hwndDlg, false, detail);
                  set_profile_title(hwndDlg, storefront);
                  append_log("Auto-fill not found; profile title '%s'",
                             storefront ? storefront : "Default");
                }else{
                  game_desc_t gd;
                  const char *catalog = catalog_name_for_id(found_id, &gd);
                  double similarity = 1.0;
                  if(kind == NAME_LOOKUP_CANDIDATE){
                    similarity = catalog ? catalog_name_similarity(query, catalog) : 0.0;
                    append_log("Name similarity %.3f for '%s' vs catalog '%s' (ID=%d)",
                               similarity, query ? query : "", catalog ? catalog : "", found_id);
                  }
                  if(kind == NAME_LOOKUP_CANDIDATE && similarity < 0.80){
                    char detail[512];
                    if(catalog){
                      snprintf(detail, sizeof(detail),
                               "Closest game list entry is '%s' (Application ID %d). "
                               "That name is filled in as the profile title and can be changed before Start.",
                               catalog, found_id);
                    }else{
                      snprintf(detail, sizeof(detail),
                               "Closest Application ID is %d, but it has no name in the game list. "
                               "The profile title will be '%s' and can be changed before Start.",
                               found_id, storefront ? storefront : "Default");
                    }
                    warn_game_list_issue(hwndDlg, true, detail);
                    set_profile_title(hwndDlg, catalog ? catalog : storefront);
                  }else{
                    set_profile_title(hwndDlg, catalog ? catalog : storefront);
                    printf("INFO: Profile title '%s' from '%s' (ID=%d, similarity %.0f%%)\n",
                           catalog ? catalog : (storefront ? storefront : "Default"),
                           query ? query : "", found_id, similarity * 100.0);
                    append_log("Profile title '%s' from '%s' (ID=%d)",
                               catalog ? catalog : (storefront ? storefront : "Default"),
                               query ? query : "", found_id);
                  }
                  release_game_desc(&gd);
                }
              }
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


