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


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    TRACE("(0x%p, %d, %p)\n", hinstDLL, fdwReason, lpvReserved);
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
  float r, p, y, tx, ty, tz;
  unsigned int frame;
  int res = linuxtrack_get_pose(&y, &p, &r, &tx, &ty, &tz, &frame);
  memset((char *)data, 0, sizeof(tir_data_t));
  data->status = (linuxtrack_get_tracking_state() == RUNNING) ? 0 : 1;
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
  if(getSomeSeriousPoetry(sig->DllSignature, sig->AppSignature)){
    printf("Signature result: OK\n");
    return 0;
  }else{
    printf("Signature result: NOT OK!\n");
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
  linuxtrack_recenter();
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
    printf("Application ID: %d - %s!!!\n", id, gd.name);
    if(game_data_get_desc(id, &gd)){
      crypted = gd.encrypted;
      if(gd.encrypted){
        printf("Table: %02X %02X %02X %02X %02X %02X %02X %02X\n", table[0],table[1],table[2],table[3],table[4],
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
    }
    
    // Try to initialize LinuxTrack - if it fails, try to start the daemon first
    linuxtrack_state_type init_result = linuxtrack_init(gd.name);
    if(init_result < LINUXTRACK_OK){
      printf("LinuxTrack initialization failed (%d): %s\n", init_result, linuxtrack_explain(init_result));
      
      // Try to start the LinuxTrack daemon if it's not running
      printf("Attempting to start LinuxTrack daemon...\n");
      
      // First, check if the daemon is already running by checking for the socket file
      char check_socket_cmd[512];
      snprintf(check_socket_cmd, sizeof(check_socket_cmd), "test -S /tmp/linuxtrack.sock > /dev/null 2>&1");
      int socket_exists = (system(check_socket_cmd) == 0);
      
      if(socket_exists){
        printf("LinuxTrack socket found - daemon appears to be running\n");
        // Try initialization again
        init_result = linuxtrack_init(gd.name);
        if(init_result >= LINUXTRACK_OK){
          printf("LinuxTrack initialization successful after socket check\n");
        } else {
          printf("LinuxTrack initialization still failed even with socket present (%d): %s\n", 
                 init_result, linuxtrack_explain(init_result));
        }
      } else {
        printf("LinuxTrack socket not found - daemon not running, attempting to start...\n");
        
        // Look for ltr_server1 in common locations (this is the actual daemon name)
        const char* possible_paths[] = {
          "/usr/bin/ltr_server1",
          "/usr/local/bin/ltr_server1", 
          "/opt/bin/ltr_server1",
          "ltr_server1"  // Try PATH
        };
        
        int daemon_started = 0;
        for(int i = 0; i < sizeof(possible_paths)/sizeof(possible_paths[0]); i++){
          printf("Trying to start daemon from: %s\n", possible_paths[i]);
          
          // Check if the daemon is already running first
          char check_cmd[512];
          snprintf(check_cmd, sizeof(check_cmd), "pgrep -f ltr_server1 > /dev/null 2>&1");
          if(system(check_cmd) == 0){
            printf("LinuxTrack daemon is already running\n");
            daemon_started = 1;
            break;
          }
          
          // Try to start the daemon using the proper method
          // The daemon needs to be started with specific parameters
          char cmd[512];
          // Try starting as standalone daemon first
          snprintf(cmd, sizeof(cmd), "%s > /dev/null 2>&1 &", possible_paths[i]);
          
          if(system(cmd) == 0){
            printf("Daemon start command executed successfully\n");
            // Wait a moment for the daemon to start
            Sleep(3000);  // 3 second delay for daemon to fully start
            
            // Verify the daemon is actually running
            if(system(check_cmd) == 0){
              printf("Daemon verification successful - daemon is running\n");
              daemon_started = 1;
              break;
            } else {
              printf("Daemon verification failed - daemon may not have started properly\n");
            }
          } else {
            printf("Failed to start daemon from: %s\n", possible_paths[i]);
          }
        }
        
        // If direct daemon start failed, try starting the GUI which will start the daemon
        if(!daemon_started){
          printf("Direct daemon start failed, trying to start LinuxTrack GUI...\n");
          const char* gui_paths[] = {
            "/usr/bin/ltr_gui",
            "/usr/local/bin/ltr_gui", 
            "/opt/bin/ltr_gui",
            "ltr_gui"  // Try PATH
          };
          
          for(int i = 0; i < sizeof(gui_paths)/sizeof(gui_paths[0]); i++){
            printf("Trying to start GUI from: %s\n", gui_paths[i]);
            char gui_cmd[512];
            snprintf(gui_cmd, sizeof(gui_cmd), "%s > /dev/null 2>&1 &", gui_paths[i]);
            
            if(system(gui_cmd) == 0){
              printf("GUI start command executed successfully\n");
              Sleep(5000);  // 5 second delay for GUI to start daemon
              
              // Check if socket was created
              if(system(check_socket_cmd) == 0){
                printf("LinuxTrack socket created - GUI successfully started daemon\n");
                daemon_started = 1;
                break;
              } else {
                printf("GUI started but socket not created - daemon may not be running\n");
              }
            } else {
              printf("Failed to start GUI from: %s\n", gui_paths[i]);
            }
          }
        }
        
        if(daemon_started){
          // Try initialization again after starting the daemon
          printf("Retrying LinuxTrack initialization after daemon start...\n");
          init_result = linuxtrack_init(gd.name);
          if(init_result < LINUXTRACK_OK){
            printf("LinuxTrack initialization still failed after daemon start (%d): %s\n", 
                   init_result, linuxtrack_explain(init_result));
            return 1;
          } else {
            printf("LinuxTrack initialization successful after daemon start\n");
          }
        } else {
          printf("Could not start LinuxTrack daemon - TrackIR functionality will not work\n");
          printf("Please ensure LinuxTrack is properly installed and ltr_server1 is available\n");
          printf("You can start the daemon manually by running: ltr_server1\n");
          printf("Or start the LinuxTrack GUI which will start the daemon automatically: ltr_gui\n");
          return 1;
        }
      }
    }
  }else{
    // Try to initialize with default profile
    linuxtrack_state_type init_result = linuxtrack_init("Default");
    if(init_result < LINUXTRACK_OK){
      printf("LinuxTrack initialization failed with default profile (%d): %s\n", 
             init_result, linuxtrack_explain(init_result));
      
      // Try to start the LinuxTrack daemon if it's not running
      printf("Attempting to start LinuxTrack daemon for default profile...\n");
      
      // First, check if the daemon is already running by checking for the socket file
      char check_socket_cmd[512];
      snprintf(check_socket_cmd, sizeof(check_socket_cmd), "test -S /tmp/linuxtrack.sock > /dev/null 2>&1");
      int socket_exists = (system(check_socket_cmd) == 0);
      
      if(socket_exists){
        printf("LinuxTrack socket found - daemon appears to be running\n");
        // Try initialization again
        init_result = linuxtrack_init("Default");
        if(init_result >= LINUXTRACK_OK){
          printf("LinuxTrack initialization successful after socket check\n");
        } else {
          printf("LinuxTrack initialization still failed even with socket present (%d): %s\n", 
                 init_result, linuxtrack_explain(init_result));
        }
      } else {
        printf("LinuxTrack socket not found - daemon not running, attempting to start...\n");
        
        // Look for ltr_server1 in common locations (this is the actual daemon name)
        const char* possible_paths[] = {
          "/usr/bin/ltr_server1",
          "/usr/local/bin/ltr_server1", 
          "/opt/bin/ltr_server1",
          "ltr_server1"  // Try PATH
        };
        
        int daemon_started = 0;
        for(int i = 0; i < sizeof(possible_paths)/sizeof(possible_paths[0]); i++){
          printf("Trying to start daemon from: %s\n", possible_paths[i]);
          
          // Check if the daemon is already running first
          char check_cmd[512];
          snprintf(check_cmd, sizeof(check_cmd), "pgrep -f ltr_server1 > /dev/null 2>&1");
          if(system(check_cmd) == 0){
            printf("LinuxTrack daemon is already running\n");
            daemon_started = 1;
            break;
          }
          
          // Try to start the daemon using the proper method
          char cmd[512];
          // Try starting as standalone daemon first
          snprintf(cmd, sizeof(cmd), "%s > /dev/null 2>&1 &", possible_paths[i]);
          
          if(system(cmd) == 0){
            printf("Daemon start command executed successfully\n");
            // Wait a moment for the daemon to start
            Sleep(3000);  // 3 second delay for daemon to fully start
            
            // Verify the daemon is actually running
            if(system(check_cmd) == 0){
              printf("Daemon verification successful - daemon is running\n");
              daemon_started = 1;
              break;
            } else {
              printf("Daemon verification failed - daemon may not have started properly\n");
            }
          } else {
            printf("Failed to start daemon from: %s\n", possible_paths[i]);
          }
        }
        
        // If direct daemon start failed, try starting the GUI which will start the daemon
        if(!daemon_started){
          printf("Direct daemon start failed, trying to start LinuxTrack GUI...\n");
          const char* gui_paths[] = {
            "/usr/bin/ltr_gui",
            "/usr/local/bin/ltr_gui", 
            "/opt/bin/ltr_gui",
            "ltr_gui"  // Try PATH
          };
          
          for(int i = 0; i < sizeof(gui_paths)/sizeof(gui_paths[0]); i++){
            printf("Trying to start GUI from: %s\n", gui_paths[i]);
            char gui_cmd[512];
            snprintf(gui_cmd, sizeof(gui_cmd), "%s > /dev/null 2>&1 &", gui_paths[i]);
            
            if(system(gui_cmd) == 0){
              printf("GUI start command executed successfully\n");
              Sleep(5000);  // 5 second delay for GUI to start daemon
              
              // Check if socket was created
              if(system(check_socket_cmd) == 0){
                printf("LinuxTrack socket created - GUI successfully started daemon\n");
                daemon_started = 1;
                break;
              } else {
                printf("GUI started but socket not created - daemon may not be running\n");
              }
            } else {
              printf("Failed to start GUI from: %s\n", gui_paths[i]);
            }
          }
        }
        
        if(daemon_started){
          // Try initialization again after starting the daemon
          printf("Retrying LinuxTrack initialization after daemon start...\n");
          init_result = linuxtrack_init("Default");
          if(init_result < LINUXTRACK_OK){
            printf("LinuxTrack initialization still failed after daemon start (%d): %s\n", 
                   init_result, linuxtrack_explain(init_result));
            return 1;
          } else {
            printf("LinuxTrack initialization successful after daemon start\n");
          }
        } else {
          printf("Could not start LinuxTrack daemon - TrackIR functionality will not work\n");
          printf("Please ensure LinuxTrack is properly installed and ltr_server1 is available\n");
          printf("You can start the daemon manually by running: ltr_server1\n");
          printf("Or start the LinuxTrack GUI which will start the daemon automatically: ltr_gui\n");
          return 1;
        }
      }
    }
  }
  char *toLock = file_path("NPClient.dll");
  sharedLock(toLock);
  free(toLock);
  runFile("TrackIR.exe");
  linuxtrack_suspend();
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
  linuxtrack_wakeup();
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
  linuxtrack_suspend();
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

