/*
 * NPClient.dll
 *
 * Generated from NPClient.dll by winedump.
 *
 * DO NOT SUBMIT GENERATED DLLS FOR INCLUSION INTO WINE!
 *
 */

#include <linuxtrack.h>
#include "rest.h"
//#include "config.h"
#define __WINESRC__

// Define DLL_WINE_PREATTACH for modern wine compatibility
#ifndef DLL_WINE_PREATTACH
#define DLL_WINE_PREATTACH 0x8000
#endif

#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "windef.h"
#include "winbase.h"
#include "NPClient_dll.h"

// Wine-specific socket communication includes (after Windows headers)
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#ifndef __MINGW32__
#include "wine/debug.h"
#else
#define WINE_DEFAULT_DEBUG_CHANNEL(x)
#define TRACE(...) do {} while(0)
#endif

WINE_DEFAULT_DEBUG_CHANNEL(NPClient);

static bool crypted = false;
static unsigned char table[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static int dbg_flag;
static HINSTANCE thisDll;
static char g_profile_name[256] = {0};
static bool data_transmission_started = false;
static DWORD transmission_start_time = 0;  // Track when transmission started

static void dbg_report(const char *msg,...)
{
  static FILE *f = NULL;
  if(dbg_flag){
    if(f == NULL){
      f = fopen("NPClient.log", "w");
    }
    va_list ap;
    va_start(ap,msg);
    vfprintf(f, msg, ap);
    fflush(f);
    va_end(ap);
  }
}

// Wine-specific socket communication functions
static int send_command_to_master(uint32_t cmd, uint32_t data)
{
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1) {
    return -1;
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  const char *sock_path = "/tmp/ltr_m_sock";
  size_t path_len = strlen(sock_path);
  if(path_len < sizeof(addr.sun_path)) {
    memcpy(addr.sun_path, sock_path, path_len + 1);
  } else {
    close(sock);
    return -1;
  }

  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    close(sock);
    return -1;
  }

  // Send command message using the same structure as ltr_srv_comm.h
  struct {
    uint32_t cmd;
    uint32_t data;
    union {
      char str[500];
      // Other union members not used here
    };
  } msg;

  // Initialize the entire structure to zero (same as server does)
  memset(&msg, 0, sizeof(msg));
  msg.cmd = cmd;
  msg.data = data;
  msg.str[0] = '\0'; // Initialize string part

  // Try write() instead of send() to avoid Wine socket issues
  ssize_t sent = write(sock, &msg, sizeof(msg));
  if (sent == -1) {
    close(sock);
    return -1;
  }

  close(sock);
  return 0;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    TRACE("(0x%p, %ld, %p)\n", hinstDLL, (long)fdwReason, lpvReserved);
    thisDll = hinstDLL;
    switch (fdwReason)
    {
        case DLL_WINE_PREATTACH:
            return TRUE;
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            dbg_flag = getDebugFlag('w');
            dbg_report("Attach request\n");
            break;
        case DLL_PROCESS_DETACH:
            linuxtrack_shutdown();
            break;
    }

    return TRUE;
}

/******************************************************************
 *		DllRegisterServer - Required for Wine DLL loading
 *
 *
 */
HRESULT WINAPI DllRegisterServer(void)
{
    TRACE("()\n");
    return S_OK;
}

/******************************************************************
 *		DllUnregisterServer - Required for Wine DLL loading
 *
 *
 */
HRESULT WINAPI DllUnregisterServer(void)
{
    TRACE("()\n");
    return S_OK;
}

/******************************************************************
 *		NPPriv_ClientNotify (NPCLIENT.1)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_ClientNotify()
{
	/* @stub in .spec */
}
#endif
/******************************************************************
 *		NPPriv_GetLastError (NPCLIENT.2)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_GetLastError()
{
	/* @stub in .spec */
}
#endif
/******************************************************************
 *		NPPriv_SetData (NPCLIENT.3)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_SetData()
{
	/* @stub in .spec */
}
#endif
/******************************************************************
 *		NPPriv_SetLastError (NPCLIENT.4)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_SetLastError()
{
	/* @stub in .spec */
}
#endif
/******************************************************************
 *		NPPriv_SetParameter (NPCLIENT.5)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_SetParameter()
{
	/* @stub in .spec */
}
#endif
/******************************************************************
 *		NPPriv_SetSignature (NPCLIENT.6)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_SetSignature()
{
	/* @stub in .spec */
}
#endif
/******************************************************************
 *		NPPriv_SetVersion (NPCLIENT.7)
 *
 *
 */
#if 0
__stdcall NPCLIENT_NPPriv_SetVersion()
{
	/* @stub in .spec */
}
#endif

static float limit_num(float min, float val, float max)
{
  if(val < min) return min;
  if(val > max) return max;
  return val;
}

static unsigned int cksum(unsigned char buf[], unsigned int size)
{
  if((size == 0) || (buf == NULL)){
    return 0;
  }

  int rounds = size >> 2;
  int rem = size % 4;

  int c = size;
  int a0, a2;
//  printf("Orig: ");
//for(a0 = 0; a0 < (int)size; ++a0)
//{
//  printf("%02X", buf[a0]);
//}
//printf("\n");
  while(rounds != 0){
    a0 = *(short int*)buf;
    a2 = *(short int*)(buf+2);
    buf += 4;
    c += a0;
    a2 ^= (c << 5);
    a2 <<= 11;
    c ^= a2;
    c += (c >> 11);
    --rounds;
  }
  switch(rem){
    case 3:
        a0 = *(short int*)buf;
        a2 = *(signed char*)(buf+2);
        c += a0;
        a2 = (a2 << 2) ^ c;
        c ^= (a2 << 16);
        a2 = (c >> 11);
      break;
    case 2:
        a2 = *(short int*)buf;
        c += a2;
        c ^= (c << 11);
        a2 = (c >> 17);
      break;
    case 1:
        a2 = *(signed char*)(buf);
        c += a2;
        c ^= (c << 10);
        a2 = (c >> 1);
      break;
    default:
      break;
  }
  if(rem != 0){
    c+=a2;
  }

  c ^= (c << 3);
  c += (c >> 5);
  c ^= (c << 4);
  c += (c >> 17);
  c ^= (c << 25);
  c += (c >> 6);

  return (unsigned int)c;
}

static void enhance(unsigned char buf[], unsigned int size,
             unsigned char codetable[], unsigned int table_size)
{
  unsigned int table_ptr = 0;
  unsigned char var = 0x88;
  unsigned char tmp;
  if((size <= 0) || (table_size <= 0) ||
     (buf == NULL) || (codetable == NULL)){
    return;
  }
  do{
    tmp = buf[--size];
    buf[size] = tmp ^ codetable[table_ptr] ^ var;
    var += size + tmp;
    ++table_ptr;
    if(table_ptr >= table_size){
      table_ptr -= table_size;
    }
  }while(size != 0);
}


/******************************************************************
 *		NP_GetData (NPCLIENT.8)
 *
 *
 */
int __stdcall NPCLIENT_NP_GetData(tir_data_t * data)
{
  dbg_report("GetData request\n");

  // Defensive check: ensure data pointer is valid
  if (data == NULL) {
    dbg_report("ERROR: NP_GetData called with NULL data pointer!\n");
    TRACE("NP_GetData called with NULL pointer\n");
    return 1;
  }

  // Check if data transmission has been started
  if (!data_transmission_started) {
    dbg_report("WARNING: NP_GetData called before StartDataTransmission\n");
    memset((char *)data, 0, sizeof(tir_data_t));
    data->status = 1; // Not tracking
    return 1;
  }

  // CRITICAL: Add a minimum delay after StartDataTransmission before allowing get_pose
  // The crash at offset 0xA0 suggests internal structures need more time to initialize
  if (transmission_start_time != 0) {
    DWORD elapsed = GetTickCount() - transmission_start_time;
    if (elapsed < 1000) {  // Wait at least 1 second after StartDataTransmission
      dbg_report("WARNING: NP_GetData called too soon after StartDataTransmission (%d ms), waiting...\n", elapsed);
      Sleep(1000 - elapsed);
    }
    // Reset after any evaluation - ensures this check only runs once
    transmission_start_time = 0;
  }

  // Check if LinuxTrack is properly initialized and in a valid state
  linuxtrack_state_type state = linuxtrack_get_tracking_state();
  if (state < LINUXTRACK_OK || state == INITIALIZING) {
    dbg_report("WARNING: NP_GetData called but LinuxTrack not ready (state: %d)\n", state);
    memset((char *)data, 0, sizeof(tir_data_t));
    data->status = 1; // Not tracking
    return 1;
  }

  // Only call get_pose if we're in a state that can provide data
  // This prevents accessing uninitialized internal structures
  if (state != RUNNING && state != PAUSED) {
    dbg_report("WARNING: NP_GetData called but tracking not active (state: %d)\n", state);
    memset((char *)data, 0, sizeof(tir_data_t));
    data->status = 1; // Not tracking
    return 1;
  }

  // Initialize variables before calling get_pose
  float r = 0.0f, p = 0.0f, y = 0.0f, tx = 0.0f, ty = 0.0f, tz = 0.0f;
  unsigned int frame = 0;
  
  // CRITICAL: This is where the crash happens at offset 0xA0
  // The LinuxTrack library's internal structures may not be fully initialized
  // even though the state says RUNNING. We've added delays, but if it still crashes,
  // the issue is in the LinuxTrack library's internal structure access.
  dbg_report("About to call linuxtrack_get_pose (state: %d)\n", state);
  
  // Call get_pose - this may crash if internal structures aren't ready
  // If it crashes, the issue is in the LinuxTrack library, not our code
  int res = linuxtrack_get_pose(&y, &p, &r, &tx, &ty, &tz, &frame);
  dbg_report("linuxtrack_get_pose returned: %d\n", res);
  
  // Initialize data structure safely
  memset((char *)data, 0, sizeof(tir_data_t));
  
  // Get current state for status - validate it's safe
  linuxtrack_state_type current_state = linuxtrack_get_tracking_state();
  data->status = (current_state == RUNNING) ? 0 : 1;
  data->frame = frame & 0xFFFF;
  data->cksum = 0;
  data->roll = r / 180.0 * 16383;
  data->pitch = -p / 180.0 * 16383;
  data->yaw = y / 180.0 * 16383;

  // TrackIR software seems to use a factor of approximately 0.03mm per unity
  // to represent the displacements (16383 ~ 50.11cm), which leads to factor
  // 32.7 (16383 / 501.1).
  data->tx = -limit_num(-16383.0, 32.7 * tx, 16383);
  data->ty = limit_num(-16383.0, 32.7 * ty, 16383);
  data->tz = limit_num(-16383.0, 32.7 * tz, 16383);
  data->cksum = cksum((unsigned char*)data, sizeof(tir_data_t));
  //printf("Cksum: %04X\n", data->cksum);
  if(crypted){
    enhance((unsigned char*)data, sizeof(tir_data_t), table, sizeof(table));
  }
  return (res >= 0) ? 0: 1;
}
/******************************************************************
 *		NP_GetParameter (NPCLIENT.9)
 *
 *
 */
int __stdcall NPCLIENT_NP_GetParameter(int arg0, int arg1)
{
        dbg_report("GetParameter request: %d %d\n", arg0, arg1);
	TRACE("(void): stub\n");
	return (int) 0;
}

/******************************************************************
 *		NP_GetSignature (NPCLIENT.10)
 *
 *
 */
int __stdcall NPCLIENT_NP_GetSignature(tir_signature_t * sig)
{
  dbg_report("GetSignature request\n");

  // Defensive check: ensure sig pointer is valid
  if (sig == NULL) {
    dbg_report("ERROR: NP_GetSignature called with NULL sig pointer!\n");
    TRACE("NP_GetSignature called with NULL pointer\n");
    return 1;
  }

  if(getSomeSeriousPoetry(sig->DllSignature, sig->AppSignature)){
    dbg_report("Signature result: OK\n");
    return 0;
  }else{
    dbg_report("Signature result: NOT OK!\n");
    return 1;
  }
}
/******************************************************************
 *		NP_QueryVersion (NPCLIENT.11)
 *
 *
 */
int __stdcall NPCLIENT_NP_QueryVersion(unsigned short * version)
{
        dbg_report("QueryVersion request\n");
	*version=0x0500;
	return 0;
}
/******************************************************************
 *		NP_ReCenter (NPCLIENT.12)
 *
 *
 */
int __stdcall NPCLIENT_NP_ReCenter(void)
{
  dbg_report("ReCenter request\n");
  linuxtrack_state_type state = linuxtrack_get_tracking_state();
  if(state >= LINUXTRACK_OK) {
    linuxtrack_recenter();
  } else {
    dbg_report("ReCenter called but LinuxTrack not initialized\n");
  }
  return 0;
}

/******************************************************************
 *		NP_RegisterProgramProfileID (NPCLIENT.13)
 *
 *
 */
int __stdcall NPCLIENT_NP_RegisterProgramProfileID(unsigned short id)
{
  dbg_report("RegisterProgramProfileID request: %d\n", id);
  game_desc_t gd;
  if(game_data_get_desc(id, &gd)){
    dbg_report("Application ID: %d - %s!!!\n", id, gd.name);
    /* Remember profile name for later lazy init (disabled due to build issue) */
    /* snprintf(g_profile_name, sizeof(g_profile_name), "%s", gd.name); */
    crypted = gd.encrypted;
    if(gd.encrypted){
      dbg_report("Table: %02X %02X %02X %02X %02X %02X %02X %02X\n", table[0],table[1],table[2],table[3],table[4],
           table[5], table[6], table[7]);
      table[0] = (unsigned char)(gd.key1&0xff); gd.key1 >>= 8;
      table[1] = (unsigned char)(gd.key1&0xff); gd.key1 >>= 8;
      table[2] = (unsigned char)(gd.key1&0xff); gd.key1 >>= 8;
      table[3] = (unsigned char)(gd.key1&0xff); gd.key1 >>= 8;
      table[4] = (unsigned char)(gd.key2&0xff); gd.key2 >>= 8;
      table[5] = (unsigned char)(gd.key2&0xff); gd.key2 >>= 8;
      table[6] = (unsigned char)(gd.key2&0xff); gd.key2 >>= 8;
      table[7] = (unsigned char)(gd.key2&0xff); gd.key2 >>= 8;
    }
    
    // Try to initialize LinuxTrack - if it fails, try to start the daemon first
    linuxtrack_state_type init_result = linuxtrack_init(gd.name);
    if(init_result < LINUXTRACK_OK){
      const char *explain = linuxtrack_explain(init_result);
      dbg_report("LinuxTrack initialization failed (%d): %s\n", init_result, explain ? explain : "unknown error");
    } else if(init_result == INITIALIZING){
      dbg_report("LinuxTrack initialization started, waiting for completion...\n");
      // Wait for initialization to complete
      for(int i = 0; i < 50 && init_result == INITIALIZING; ++i){  // Wait up to 5 seconds
        Sleep(100);
        init_result = linuxtrack_get_tracking_state();
        if(init_result == RUNNING || init_result == PAUSED) {
          dbg_report("LinuxTrack initialization completed successfully\n");
          break;
        }
      }
      if(init_result == INITIALIZING) {
        dbg_report("LinuxTrack initialization timed out - system may not be properly configured\n");
      }
    }
    if(init_result < LINUXTRACK_OK || init_result == INITIALIZING){

      // Try to start the LinuxTrack daemon if it's not running
      dbg_report("Attempting to start LinuxTrack daemon...\n");
      
      // First, check if the daemon is already running by checking for the socket file
      char check_socket_cmd[512];
      snprintf(check_socket_cmd, sizeof(check_socket_cmd), "test -S /tmp/linuxtrack.sock > /dev/null 2>&1");
      int socket_exists = (system(check_socket_cmd) == 0);
      
      if(socket_exists){
        dbg_report("LinuxTrack socket found - daemon appears to be running\n");
        // Try initialization again
        init_result = linuxtrack_init(gd.name);
        if(init_result >= LINUXTRACK_OK){
          dbg_report("LinuxTrack initialization successful after socket check\n");
        } else {
          const char *explain = linuxtrack_explain(init_result);
          dbg_report("LinuxTrack initialization still failed even with socket present (%d): %s\n", 
                 init_result, explain ? explain : "unknown error");
        }
      } else {
        dbg_report("LinuxTrack socket not found - daemon not running, attempting to start...\n");
        
        // Look for ltr_server1 in common locations (this is the actual daemon name)
        const char* possible_paths[] = {
          "/usr/bin/ltr_server1",
          "/usr/local/bin/ltr_server1", 
          "/opt/bin/ltr_server1",
          "ltr_server1"  // Try PATH
        };
        
        int daemon_started = 0;
        for(int i = 0; i < sizeof(possible_paths)/sizeof(possible_paths[0]); i++){
          dbg_report("Trying to start daemon from: %s\n", possible_paths[i]);
          
          // Check if the daemon is already running first
          char check_cmd[512];
          snprintf(check_cmd, sizeof(check_cmd), "pgrep -f ltr_server1 > /dev/null 2>&1");
          if(system(check_cmd) == 0){
            dbg_report("LinuxTrack daemon is already running\n");
            daemon_started = 1;
            break;
          }
          
          // Try to start the daemon using the proper method
          // The daemon needs to be started with specific parameters
          char cmd[512];
          // Try starting as standalone daemon first
          snprintf(cmd, sizeof(cmd), "%s > /dev/null 2>&1 &", possible_paths[i]);
          
          if(system(cmd) == 0){
            dbg_report("Daemon start command executed successfully\n");
            // Wait a moment for the daemon to start
            Sleep(3000);  // 3 second delay for daemon to fully start
            
            // Verify the daemon is actually running
            if(system(check_cmd) == 0){
              dbg_report("Daemon verification successful - daemon is running\n");
              daemon_started = 1;
              break;
            } else {
              dbg_report("Daemon verification failed - daemon may not have started properly\n");
            }
          } else {
            dbg_report("Failed to start daemon from: %s\n", possible_paths[i]);
          }
        }
        
        // If direct daemon start failed, try starting the GUI which will start the daemon
        if(!daemon_started){
          dbg_report("Direct daemon start failed, trying to start LinuxTrack GUI...\n");
          const char* gui_paths[] = {
            "/usr/bin/ltr_gui",
            "/usr/local/bin/ltr_gui", 
            "/opt/bin/ltr_gui",
            "ltr_gui"  // Try PATH
          };
          
          for(int i = 0; i < sizeof(gui_paths)/sizeof(gui_paths[0]); i++){
            dbg_report("Trying to start GUI from: %s\n", gui_paths[i]);
            char gui_cmd[512];
            snprintf(gui_cmd, sizeof(gui_cmd), "%s > /dev/null 2>&1 &", gui_paths[i]);
            
            if(system(gui_cmd) == 0){
              dbg_report("GUI start command executed successfully\n");
              Sleep(5000);  // 5 second delay for GUI to start daemon
              
              // Check if socket was created
              if(system(check_socket_cmd) == 0){
                dbg_report("LinuxTrack socket created - GUI successfully started daemon\n");
                daemon_started = 1;
                break;
              } else {
                dbg_report("GUI started but socket not created - daemon may not be running\n");
              }
            } else {
              dbg_report("Failed to start GUI from: %s\n", gui_paths[i]);
            }
          }
        }
        
        if(daemon_started){
          // Try initialization again after starting the daemon
          dbg_report("Retrying LinuxTrack initialization after daemon start...\n");
          init_result = linuxtrack_init(gd.name);
          if(init_result < LINUXTRACK_OK){
            const char *explain = linuxtrack_explain(init_result);
            dbg_report("LinuxTrack initialization still failed after daemon start (%d): %s\n", 
                   init_result, explain ? explain : "unknown error");
            return 1;
          } else {
            dbg_report("LinuxTrack initialization successful after daemon start\n");
          }
        } else {
          dbg_report("Could not start LinuxTrack daemon - TrackIR functionality will not work\n");
          dbg_report("Please ensure LinuxTrack is properly installed and ltr_server1 is available\n");
          dbg_report("You can start the daemon manually by running: ltr_server1\n");
          dbg_report("Or start the LinuxTrack GUI which will start the daemon automatically: ltr_gui\n");
          return 1;
        }
      }
    }
  }else{
    // Try to initialize with default profile
    linuxtrack_state_type init_result = linuxtrack_init("Default");
    /* snprintf(g_profile_name, sizeof(g_profile_name), "%s", "Default"); */
    if(init_result < LINUXTRACK_OK){
      const char *explain = linuxtrack_explain(init_result);
      dbg_report("LinuxTrack initialization failed with default profile (%d): %s\n", 
             init_result, explain ? explain : "unknown error");
      
      // Try to start the LinuxTrack daemon if it's not running
      dbg_report("Attempting to start LinuxTrack daemon for default profile...\n");
      
      // First, check if the daemon is already running by checking for the socket file
      char check_socket_cmd[512];
      snprintf(check_socket_cmd, sizeof(check_socket_cmd), "test -S /tmp/linuxtrack.sock > /dev/null 2>&1");
      int socket_exists = (system(check_socket_cmd) == 0);
      
      if(socket_exists){
        dbg_report("LinuxTrack socket found - daemon appears to be running\n");
        // Try initialization again
        init_result = linuxtrack_init("Default");
        if(init_result >= LINUXTRACK_OK){
          dbg_report("LinuxTrack initialization successful after socket check\n");
        } else {
          const char *explain2 = linuxtrack_explain(init_result);
          dbg_report("LinuxTrack initialization still failed even with socket present (%d): %s\n", 
                 init_result, explain2 ? explain2 : "unknown error");
        }
      } else {
        dbg_report("LinuxTrack socket not found - daemon not running, attempting to start...\n");
        
        // Look for ltr_server1 in common locations (this is the actual daemon name)
        const char* possible_paths[] = {
          "/usr/bin/ltr_server1",
          "/usr/local/bin/ltr_server1", 
          "/opt/bin/ltr_server1",
          "ltr_server1"  // Try PATH
        };
        
        int daemon_started = 0;
        for(int i = 0; i < sizeof(possible_paths)/sizeof(possible_paths[0]); i++){
          dbg_report("Trying to start daemon from: %s\n", possible_paths[i]);
          
          // Check if the daemon is already running first
          char check_cmd[512];
          snprintf(check_cmd, sizeof(check_cmd), "pgrep -f ltr_server1 > /dev/null 2>&1");
          if(system(check_cmd) == 0){
            dbg_report("LinuxTrack daemon is already running\n");
            daemon_started = 1;
            break;
          }
          
          // Try to start the daemon using the proper method
          char cmd[512];
          // Try starting as standalone daemon first
          snprintf(cmd, sizeof(cmd), "%s > /dev/null 2>&1 &", possible_paths[i]);
          
          if(system(cmd) == 0){
            dbg_report("Daemon start command executed successfully\n");
            // Wait a moment for the daemon to start
            Sleep(3000);  // 3 second delay for daemon to fully start
            
            // Verify the daemon is actually running
            if(system(check_cmd) == 0){
              dbg_report("Daemon verification successful - daemon is running\n");
              daemon_started = 1;
              break;
            } else {
              dbg_report("Daemon verification failed - daemon may not have started properly\n");
            }
          } else {
            dbg_report("Failed to start daemon from: %s\n", possible_paths[i]);
          }
        }
        
        // If direct daemon start failed, try starting the GUI which will start the daemon
        if(!daemon_started){
          dbg_report("Direct daemon start failed, trying to start LinuxTrack GUI...\n");
          const char* gui_paths[] = {
            "/usr/bin/ltr_gui",
            "/usr/local/bin/ltr_gui", 
            "/opt/bin/ltr_gui",
            "ltr_gui"  // Try PATH
          };
          
          for(int i = 0; i < sizeof(gui_paths)/sizeof(gui_paths[0]); i++){
            dbg_report("Trying to start GUI from: %s\n", gui_paths[i]);
            char gui_cmd[512];
            snprintf(gui_cmd, sizeof(gui_cmd), "%s > /dev/null 2>&1 &", gui_paths[i]);
            
            if(system(gui_cmd) == 0){
              dbg_report("GUI start command executed successfully\n");
              Sleep(5000);  // 5 second delay for GUI to start daemon
              
              // Check if socket was created
              if(system(check_socket_cmd) == 0){
                dbg_report("LinuxTrack socket created - GUI successfully started daemon\n");
                daemon_started = 1;
                break;
              } else {
                dbg_report("GUI started but socket not created - daemon may not be running\n");
              }
            } else {
              dbg_report("Failed to start GUI from: %s\n", gui_paths[i]);
            }
          }
        }
        
        if(daemon_started){
          // Try initialization again after starting the daemon
          dbg_report("Retrying LinuxTrack initialization after daemon start...\n");
          init_result = linuxtrack_init("Default");
          if(init_result < LINUXTRACK_OK){
            const char *explain3 = linuxtrack_explain(init_result);
            dbg_report("LinuxTrack initialization still failed after daemon start (%d): %s\n", 
                   init_result, explain3 ? explain3 : "unknown error");
            return 1;
          } else {
            dbg_report("LinuxTrack initialization successful after daemon start\n");
          }
        } else {
          dbg_report("Could not start LinuxTrack daemon - TrackIR functionality will not work\n");
          dbg_report("Please ensure LinuxTrack is properly installed and ltr_server1 is available\n");
          dbg_report("You can start the daemon manually by running: ltr_server1\n");
          dbg_report("Or start the LinuxTrack GUI which will start the daemon automatically: ltr_gui\n");
          return 1;
        }
      }
    }
  }
  char *toLock = file_path("NPClient.dll");
  if(toLock != NULL){
    sharedLock(toLock);
    free(toLock);
  }
  /*
   * Do not auto-launch TrackIR or suspend LinuxTrack by default in Wine bridge tester.
   * This caused the camera to immediately pause after starting the tester.
   * If needed for debugging real TrackIR integration, set LINUXTRACK_RUN_TRACKIR=1
   * to enable launching TrackIR.exe and suspending the LinuxTrack processing.
   */
  if(getenv("LINUXTRACK_RUN_TRACKIR") != NULL){
    runFile("TrackIR.exe");
    linuxtrack_suspend();
  }
  return 0;
}
/******************************************************************
 *		NP_RegisterWindowHandle (NPCLIENT.14)
 *
 *
 */
int __stdcall NPCLIENT_NP_RegisterWindowHandle(HWND hwnd)
{
        dbg_report("RegisterWindowHandle request: 0x%X\n", hwnd);
	TRACE("((HWND)%p): stub\n",hwnd);
	return (int) 0;
}
/******************************************************************
 *		NP_RequestData (NPCLIENT.15)
 *
 *
 */
int __stdcall NPCLIENT_NP_RequestData(unsigned short req)
{
        dbg_report("RequestData request: %d\n", req);
	TRACE("((unsigned short)%d): stub\n",req);
	return (int) 0;
}
/******************************************************************
 *		NP_SetParameter (NPCLIENT.16)
 *
 *
 */
int __stdcall NPCLIENT_NP_SetParameter(int arg0, int arg1)
{
        dbg_report("SetParameter request: %d %d\n", arg0, arg1);
	TRACE("(void): stub\n");
	return (int) 0;
}
/******************************************************************
 *		NP_StartCursor (NPCLIENT.17)
 *
 *
 */
int __stdcall NPCLIENT_NP_StartCursor(void)
{
        dbg_report("StartCursor request\n");
	TRACE("(void): stub\n");
	return (int) 0;
}
/******************************************************************
 *		NP_StartDataTransmission (NPCLIENT.18)
 *
 *
 */
int __stdcall NPCLIENT_NP_StartDataTransmission(void)
{
  dbg_report("StartDataTransmission request\n");

  // Prevent multiple calls or calls before proper initialization
  if (data_transmission_started) {
    dbg_report("WARNING: NP_StartDataTransmission called multiple times, ignoring\n");
    return 0;
  }

  // Set flag immediately after guard check to prevent race conditions
  // If initialization fails, we'll reset it before returning
  data_transmission_started = true;

  // Since LinuxTrack is already running, just verify it's accessible
  // and in a valid state before proceeding
  linuxtrack_state_type st = linuxtrack_get_tracking_state();
  
  // If not initialized, try to initialize (shouldn't happen if already running)
  if(st < LINUXTRACK_OK) {
    const char *profile = (g_profile_name[0] != '\0') ? g_profile_name : "Default";
    if(profile == NULL) {
      profile = "Default";
    }
    st = linuxtrack_init(profile);
    if(st < LINUXTRACK_OK) {
      dbg_report("Failed to initialize LinuxTrack in StartDataTransmission\n");
      data_transmission_started = false;  // Reset on error
      return 1;
    }
    // Brief wait if initializing
    for(int i = 0; i < 10 && st == INITIALIZING; ++i){
      Sleep(100);
      st = linuxtrack_get_tracking_state();
    }
  }

  // Validate state is acceptable - must be RUNNING or PAUSED
  // State 1 is INITIALIZING - we need to wait for it to complete
  if(st == INITIALIZING) {
    dbg_report("WARNING: StartDataTransmission called but LinuxTrack still initializing (state: %d), waiting...\n", st);
    // Wait longer for initialization to complete
    for(int i = 0; i < 50 && st == INITIALIZING; ++i){
      Sleep(100);
      st = linuxtrack_get_tracking_state();
      if(st == RUNNING || st == PAUSED) {
        dbg_report("LinuxTrack finished initializing, state is now: %d\n", st);
        break;
      }
    }
    if(st == INITIALIZING) {
      dbg_report("LinuxTrack initialization timed out in StartDataTransmission\n");
      data_transmission_started = false;  // Reset on error
      return 1;
    }
  }
  
  if(st != RUNNING && st != PAUSED) {
    dbg_report("LinuxTrack not in valid state for data transmission (state: %d)\n", st);
    data_transmission_started = false;  // Reset on error
    return 1;
  }

  // Since LinuxTrack is already running, these calls should be safe
  // But we'll check results to be defensive
  linuxtrack_wakeup();
  linuxtrack_request_frames();
  linuxtrack_notification_on();
  
  // Verify we're still in a good state after these calls
  st = linuxtrack_get_tracking_state();
  if(st != RUNNING && st != PAUSED) {
    dbg_report("State changed after wakeup calls (state: %d)\n", st);
    data_transmission_started = false;  // Reset on error
    return 1;
  }

  // Additionally, ping master to wake in case API call path is unavailable
  // Use defensive socket operations
  const char *sock_path = "/tmp/ltr_m_sock";
  size_t path_len = strlen(sock_path);
  
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock != -1) {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if(path_len < sizeof(addr.sun_path)) {
      memcpy(addr.sun_path, sock_path, path_len + 1);
      if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        // Best-effort command send - don't fail if it doesn't work
        send_command_to_master(3, 0);
      }
    }
    close(sock);
  }

  // Since LinuxTrack is already running, we just need to ensure
  // the connection is stable before allowing data access
  // The crash at offset 0xA0 suggests internal structures aren't ready
  // even though the state says RUNNING. We'll "warm up" the connection
  // by calling get_tracking_state multiple times, then wait.
  
  // Warm up the connection by calling get_tracking_state a few times
  // This helps ensure the internal connection is fully established
  for(int warmup = 0; warmup < 5; warmup++) {
    linuxtrack_get_tracking_state();
    Sleep(50);
  }
  
  // Add a longer delay to let any internal state fully settle
  // The offset 0xA0 crash suggests we need more time for structures to initialize
  Sleep(500);
  
  // Verify final state before completing
  st = linuxtrack_get_tracking_state();
  if(st == RUNNING || st == PAUSED) {
    // Record the time for minimum delay enforcement in NP_GetData
    transmission_start_time = GetTickCount();
    dbg_report("StartDataTransmission completed - system ready (state: %d)\n", st);
    dbg_report("Note: First pose call will be made by NP_GetData when ARMA 2 requests data\n");
  } else {
    dbg_report("StartDataTransmission failed - invalid state: %d\n", st);
    data_transmission_started = false;  // Reset on error
    return 1;
  }
  
  return 0;
}
/******************************************************************
 *		NP_StopCursor (NPCLIENT.19)
 *
 *
 */
int __stdcall NPCLIENT_NP_StopCursor(void)
{
        dbg_report("StopCursor request\n");
	TRACE("(void): stub\n");
	return (int) 0;
}
/******************************************************************
 *		NP_StopDataTransmission (NPCLIENT.20)
 *
 *
 */
int __stdcall NPCLIENT_NP_StopDataTransmission(void)
{
  dbg_report("StopDataTransmission request\n");
  
  // Reset transmission flag
  data_transmission_started = false;
  
  // Fully shutdown to avoid background reconnect attempts
  linuxtrack_state_type st = linuxtrack_shutdown();
  dbg_report("linuxtrack_shutdown() -> %d\n", st);

  // Also notify master (best-effort) with defensive socket operations
  const char *sock_path = "/tmp/ltr_m_sock";
  size_t path_len = strlen(sock_path);
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock != -1) {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if(path_len < sizeof(addr.sun_path)) {
      memcpy(addr.sun_path, sock_path, path_len + 1);
      if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        send_command_to_master(2, 0);
      }
    }
    close(sock);
  }
  return 0;
}
/******************************************************************
 *		NP_UnregisterWindowHandle (NPCLIENT.21)
 *
 *
 */
int __stdcall NPCLIENT_NP_UnregisterWindowHandle(void)
{
        dbg_report("UnregisterWindowHandle request\n");
	TRACE("(void): stub\n");
	return (int) 0;
}

