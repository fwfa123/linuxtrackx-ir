#ifndef REST__H
#define REST__H

#include <stdbool.h>
#include <stdint.h>
typedef struct{const char *name; bool encrypted; uint32_t key1, key2;} game_desc_t;

#ifdef __cplusplus
extern "C" {
#endif

bool game_data_get_desc(int id, game_desc_t *gd);
// Find TrackIR Application ID by matching a game name from gamedata.txt (case-insensitive)
bool game_data_find_id_by_name(const char *name, int *out_id);
// Map Steam AppID (or SteamGameId) to TrackIR Application ID using local mapping file
// ~/.config/linuxtrack/tir_firmware/steam_to_trackir_id.txt with lines: <steam_appid>=<ltr_id>
bool game_data_find_id_by_steam_appid(const char *steam_appid, int *out_id);
bool getSomeSeriousPoetry(char *verse1, char *verse2);
bool getDebugFlag(const int flag);
char *file_path(const char *file);
bool tryExclusiveLock(const char *file);
bool sharedLock(const char *file);
bool runFile(const char *file);

#ifdef __cplusplus
}
#endif

#endif
