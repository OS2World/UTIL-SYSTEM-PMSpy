/*��������������������������������������������������������������������������Ŀ*/
/*�                                                                          �*/
/*� PROGRAM NAME: PMSPY                                                      �*/
/*� -------------                                                            �*/
/*�  A PM program that is used to look at or 'spy' on the message queue of   �*/
/*�  other PM applications windows.                                          �*/
/*�                                                                          �*/
/*� COPYRIGHT:                                                               �*/
/*� ----------                                                               �*/
/*�  Copyright (C) International Business Machines Corp., 1992               �*/
/*�                                                                          �*/
/*� DISCLAIMER OF WARRANTIES:                                                �*/
/*� -------------------------                                                �*/
/*�  The following [enclosed] code is sample code created by IBM Corporation.�*/
/*�  This sample code is not part of any standard IBM product and is provided�*/
/*�  to you solely for the purpose of assisting you in the development of    �*/
/*�  your applications.  The code is provided "AS IS", without warranty of   �*/
/*�  any kind.  IBM shall not be liable for any damages arising out of your  �*/
/*�  use of the sample code, even if they have been advised of the           �*/
/*�  possibility of such damages.                                            �*/
/*�                                                                          �*/
/*� For details on what this program does etc., please see the PMSPY.C file. �*/
/*�                                                                          �*/
/*����������������������������������������������������������������������������*/

/*��������������������������������������������������������������������������Ŀ*/
/*� PMSPYNOT.C                                                               �*/
/*�                                                                          �*/
/*� Notes dialog procedure                                                   �*/
/*����������������������������������������������������������������������������*/

#include "pmspy.h"

/*��������������������������������������������������������������������������Ŀ*/
/*� Internal Function: DismissDialog                                         �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� Function:                                                                �*/
/*�                                                                          �*/
/*�   This module: 1) sets the flag indicating if entered data should        �*/
/*�                   be used by invoker of dialog                           �*/
/*�                2) POSTs message to invoker of dialog                     �*/
/*�                3) Dismisses the dialog                                   �*/
/*�                                                                          �*/
/*� Input:                                                                   �*/
/*�       DlgHwnd.......dialog handle to dismiss                             �*/
/*�       DlgData.......pointer to dialog's PNOTES_DATA structure            �*/
/*�       DlgUse........indicator if entered NOTES data should be used       �*/
/*�                                                                          �*/
/*� Output:                                                                  �*/
/*�                                                                          �*/
/*�   - N O N E                                                              �*/
/*����������������������������������������������������������������������������*/

static void DismissDialog(HWND              hwndDlg,
                          PNOTES_DATA       pDlgData,
                          register BOOL     fDlgUse)

{
  /*������������������������������������������������������������������������Ŀ*/
  /*� Set the flag for caller's interogation                                 �*/
  /*��������������������������������������������������������������������������*/

  pDlgData->fUseData = fDlgUse;

  /*������������������������������������������������������������������������Ŀ*/
  /*� Post REPLY message to caller                                           �*/
  /*��������������������������������������������������������������������������*/

  WinPostMsg(pDlgData->hwndNotify,
             pDlgData->uNotifyMsg,
             MPFROMP(pDlgData),
             MPFROMP(NULL) );

  /*������������������������������������������������������������������������Ŀ*/
  /*� Dismiss the dialog                                                     �*/
  /*��������������������������������������������������������������������������*/

  WinDismissDlg(hwndDlg, fDlgUse);        /* Close the Dialog box         */
}


MRESULT NotesDlgProc(HWND    hwndClientDlg, /* Dialog handle */
                     ULONG   message,       /* Message ID */
                     MPARAM  lParam1,       /* Parameter 1 */
                     MPARAM  lParam2)       /* Parameter 2 */
{
  /*������������������������������������������������������������������������Ŀ*/
  /*� Local Variables                                                        �*/
  /*��������������������������������������������������������������������������*/

  MRESULT   dpResult = NULL;         /* Return code */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Obtain pointer to this dialog's instance data                          �*/
  /*��������������������������������������������������������������������������*/

  PNOTES_DATA pData = (PNOTES_DATA)
                       WinQueryWindowPtr(hwndClientDlg, QWL_USER);

  /*������������������������������������������������������������������������Ŀ*/
  /*� Process this message                                                   �*/
  /*��������������������������������������������������������������������������*/

  switch (message)                      /* Switch off of the message ID       */
  {
    /*����������������������������������������������������������������������Ŀ*/
    /*� One of the pushbuttons was pressed....                               �*/
    /*������������������������������������������������������������������������*/

    case WM_COMMAND:

         switch( LOUSHORT( lParam1 ) )
         {
          /*����������������������������������������������������������������Ŀ*/
          /*� DID_OK:  (User clicked on OK button)                           �*/
          /*�                                                                �*/
          /*� - Get text from edit field                                     �*/
          /*� - Validate user's entry                                        �*/
          /*������������������������������������������������������������������*/

          case DID_OK:                              /* OK was selected          */

               /* Disable input into this button */

               WinEnableWindow( WinWindowFromID( hwndClientDlg, DID_OK),
                                BOOL_FALSE);

               /* Get text from input fields   */

               WinQueryDlgItemText(hwndClientDlg,              /* Dialog handle. */
                                   DLG_NOTES_EF_TEXT,          /* Dlg field ID. */
                                   sizeof(pData->szText) - 1,  /* Max # chars */
                                   pData->szText);             /* Return text here */

               if ( pData->szText[0] != 0 )     /* just a NULL string? */
               {
                  DismissDialog( hwndClientDlg, /* which Dialog */
                                 pData,         /* Dialog data */
                                 TRUE);         /* DO use the notes */
               }
               else
               {
                  WinAlarm( WinQueryAnchorBlock(hwndClientDlg), WA_ERROR);

                  WinEnableWindow( WinWindowFromID(hwndClientDlg, DID_OK),
                                   BOOL_TRUE);
               }
          break;

          /*����������������������������������������������������������������Ŀ*/
          /*� DID_CANCEL:                                                    �*/
          /*� - Dismiss dialog                                               �*/
          /*������������������������������������������������������������������*/
          case DID_CANCEL:                          /* CANCEL was selected    */

               DismissDialog( hwndClientDlg, /* which Dialog */
                              pData,         /* Dialog data */
                              FALSE);        /* DO NOT use the name... */
          break;
         }
         break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� WM_CLOSE:                                                            �*/
    /*� - Call WinDismissDlg to close the dialog box                         �*/
    /*������������������������������������������������������������������������*/
    case WM_CLOSE:                                  /* Close Dialog Box.      */

         DismissDialog( hwndClientDlg, /* which Dialog */
                        pData,         /* Dialog data */
                        FALSE);        /* DO NOT use the name... */
    break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� WM_INITDLG:                                                          �*/
    /*������������������������������������������������������������������������*/

    case WM_INITDLG:                                /* Initialize Dialog Box. */

      /*��������������������������������������������������������������������Ŀ*/
      /*� Save Dialog's instance data pointer (passed in MP2)                �*/
      /*����������������������������������������������������������������������*/

      WinSetWindowPtr(hwndClientDlg,
                      QWL_USER,
                      pData = (PNOTES_DATA)lParam2);

      /*��������������������������������������������������������������������Ŀ*/
      /*� Set TEXT entry field width and default values                      �*/
      /*����������������������������������������������������������������������*/

      WinSendDlgItemMsg(hwndClientDlg,      /* which Dlg */
                        DLG_NOTES_EF_TEXT,  /* which Field */
                        EM_SETTEXTLIMIT,    /* cmd: set field width */
                        MPFROMSHORT(sizeof(pData->szText)-1), /* mp1: width */
                        NULL);                                /* mp2: ignored */

      WinSetDlgItemText(hwndClientDlg,      /* which Dlg */
                        DLG_NOTES_EF_TEXT,  /* which Field */
                        pData->szText);     /* Default Field text */

      CenterDialog(hwndClientDlg);          /* Center the Dialog */
    break;

  /*������������������������������������������������������������������������Ŀ*/
  /*� We don't need to handle any other messages...                          �*/
  /*�                                                                        �*/
  /*� If this isn't an IPF message, let PM do it's default "thing"           �*/
  /*��������������������������������������������������������������������������*/

  default:
       if ( !HandleIPF(hwndClientDlg, message, lParam1, lParam2, &dpResult) )
         dpResult = WinDefDlgProc(hwndClientDlg, message, lParam1, lParam2 );

  }                                                 /* End switch                   */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Exit                                                                   �*/
  /*��������������������������������������������������������������������������*/

  return( dpResult );
}
