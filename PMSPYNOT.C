/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
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
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� PMSPYNOT.C                                                               �*/
/*�                                                                          �*/
/*� Notes dialog procedure                                                   �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

#include "pmspy.h"

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
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
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

static void DismissDialog(HWND              hwndDlg,
                          PNOTES_DATA       pDlgData,
                          register BOOL     fDlgUse)

{
  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Set the flag for caller's interogation                                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  pDlgData->fUseData = fDlgUse;

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Post REPLY message to caller                                           �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  WinPostMsg(pDlgData->hwndNotify,
             pDlgData->uNotifyMsg,
             MPFROMP(pDlgData),
             MPFROMP(NULL) );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Dismiss the dialog                                                     �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  WinDismissDlg(hwndDlg, fDlgUse);        /* Close the Dialog box         */
}


MRESULT NotesDlgProc(HWND    hwndClientDlg, /* Dialog handle */
                     ULONG   message,       /* Message ID */
                     MPARAM  lParam1,       /* Parameter 1 */
                     MPARAM  lParam2)       /* Parameter 2 */
{
  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Local Variables                                                        �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  MRESULT   dpResult = NULL;         /* Return code */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Obtain pointer to this dialog's instance data                          �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  PNOTES_DATA pData = (PNOTES_DATA)
                       WinQueryWindowPtr(hwndClientDlg, QWL_USER);

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Process this message                                                   �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  switch (message)                      /* Switch off of the message ID       */
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� One of the pushbuttons was pressed....                               �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    case WM_COMMAND:

         switch( LOUSHORT( lParam1 ) )
         {
          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� DID_OK:  (User clicked on OK button)                           �*/
          /*�                                                                �*/
          /*� - Get text from edit field                                     �*/
          /*� - Validate user's entry                                        �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

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

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� DID_CANCEL:                                                    �*/
          /*� - Dismiss dialog                                               �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          case DID_CANCEL:                          /* CANCEL was selected    */

               DismissDialog( hwndClientDlg, /* which Dialog */
                              pData,         /* Dialog data */
                              FALSE);        /* DO NOT use the name... */
          break;
         }
         break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� WM_CLOSE:                                                            �*/
    /*� - Call WinDismissDlg to close the dialog box                         �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_CLOSE:                                  /* Close Dialog Box.      */

         DismissDialog( hwndClientDlg, /* which Dialog */
                        pData,         /* Dialog data */
                        FALSE);        /* DO NOT use the name... */
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� WM_INITDLG:                                                          �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    case WM_INITDLG:                                /* Initialize Dialog Box. */

      /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
      /*� Save Dialog's instance data pointer (passed in MP2)                �*/
      /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

      WinSetWindowPtr(hwndClientDlg,
                      QWL_USER,
                      pData = (PNOTES_DATA)lParam2);

      /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
      /*� Set TEXT entry field width and default values                      �*/
      /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

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

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� We don't need to handle any other messages...                          �*/
  /*�                                                                        �*/
  /*� If this isn't an IPF message, let PM do it's default "thing"           �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  default:
       if ( !HandleIPF(hwndClientDlg, message, lParam1, lParam2, &dpResult) )
         dpResult = WinDefDlgProc(hwndClientDlg, message, lParam1, lParam2 );

  }                                                 /* End switch                   */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Exit                                                                   �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  return( dpResult );
}
