#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "resource.h"
#include "rest.h"
#include "npifc.h"

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
            SetDlgItemInt(hwndDlg, IDC_APPID, 2307, true);
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


