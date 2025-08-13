#ifndef NPIFC__H
#define NPIFC__H


#include <stdbool.h>
#include "../client/NPClient_dll.h"

#ifdef __cplusplus
extern "C" {
#endif
  bool npifc_init(HWND wnd, int id);
  void npifc_close();
  bool npifc_get_signature_and_version(tir_signature_t *sig, unsigned short *version);

 int npifc_getdata(tir_data_t *data);

typedef int __stdcall (*NP_RegisterWindowHandle_t)(HWND hwnd);
typedef int __stdcall (*NP_UnregisterWindowHandle_t)(void);
typedef int __stdcall (*NP_RegisterProgramProfileID_t)(unsigned short id);
typedef int __stdcall (*NP_QueryVersion_t)(unsigned short *version);
typedef int __stdcall (*NP_RequestData_t)(unsigned short req);
typedef int __stdcall (*NP_GetSignature_t)(tir_signature_t *sig);
typedef int __stdcall (*NP_GetData_t)(tir_data_t *data);
typedef int __stdcall (*NP_GetParameter_t)(void);
typedef int __stdcall (*NP_SetParameter_t)(void);
typedef int __stdcall (*NP_StartCursor_t)(void);
typedef int __stdcall (*NP_StopCursor_t)(void);
typedef int __stdcall (*NP_ReCenter_t)(void);
typedef int __stdcall (*NP_StartDataTransmission_t)(void);
typedef int __stdcall (*NP_StopDataTransmission_t)(void);

extern NP_RegisterWindowHandle_t NP_RegisterWindowHandle;
extern NP_UnregisterWindowHandle_t NP_UnregisterWindowHandle;
extern NP_RegisterProgramProfileID_t NP_RegisterProgramProfileID;
extern NP_QueryVersion_t NP_QueryVersion;
extern NP_RequestData_t NP_RequestData;
extern NP_GetSignature_t NP_GetSignature;
extern NP_GetData_t NP_GetData;
extern NP_GetParameter_t NP_GetParameter;
extern NP_SetParameter_t NP_SetParameter;
extern NP_StartCursor_t NP_StartCursor;
extern NP_StopCursor_t NP_StopCursor;
extern NP_ReCenter_t NP_ReCenter;
extern NP_StartDataTransmission_t NP_StartDataTransmission;
extern NP_StopDataTransmission_t NP_StopDataTransmission;

#ifdef __cplusplus
}
#endif

#endif

