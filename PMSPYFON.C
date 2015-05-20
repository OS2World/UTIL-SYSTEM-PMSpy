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
/*� PMSPYFON.C                                                               �*/
/*�                                                                          �*/
/*� Font Selection Dialog Procedure                                          �*/
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
/*�       DlgData.......pointer to dialog's PFONT_DATA structure             �*/
/*�       DlgUse........indicator if entered FONT data should be used        �*/
/*�                                                                          �*/
/*� Output:                                                                  �*/
/*�                                                                          �*/
/*�   - N O N E                                                              �*/
/*�                                                                          �*/
/*����������������������������������������������������������������������������*/

static
void DismissDialog(HWND              hwndDlg,
                   PFONT_DATA        pDlgData,
                   register BOOL     fDlgUse)

{
  /*������������������������������������������������������������������������Ŀ*/
  /*� Set the flag for caller's interrogation                                �*/
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


 MRESULT  FontDlgProc(          HWND    hWnd,
                       register ULONG   usMsg,
                                MPARAM  mp1,
                                MPARAM  mp2 )

{
  /*������������������������������������������������������������������������Ŀ*/
  /*� Local Variables                                                        �*/
  /*��������������������������������������������������������������������������*/

  MRESULT    dpResult = NULL;          /* result of current MSG processing */
  USHORT     usItem;

  /*������������������������������������������������������������������������Ŀ*/
  /*� Obtain pointer to this dialog's instance data                          �*/
  /*��������������������������������������������������������������������������*/

  PFONT_DATA pData = (PFONT_DATA) WinQueryWindowPtr(hWnd, QWL_USER);

  /*������������������������������������������������������������������������Ŀ*/
  /*� Process the message                                                    �*/
  /*��������������������������������������������������������������������������*/

  switch ( usMsg )
    {
      /*��������������������������������������������������������������������Ŀ*/
      /*� One of the pushbuttons has been used....                           �*/
      /*����������������������������������������������������������������������*/

      case WM_COMMAND:

        switch( LOUSHORT( mp1 ) )
        {
           /*���������������������������������������������������������������Ŀ*/
           /*� Process selected FONT                                         �*/
           /*�����������������������������������������������������������������*/

           case DID_OK:

                /*����������������������������������������������������������Ŀ*/
                /*� Determine selected FONT                                  �*/
                /*������������������������������������������������������������*/

                usItem = (USHORT)
                         WinSendDlgItemMsg(hWnd,
                                           DLG_FONT_LB,
                                           LM_QUERYSELECTION,
                                           MPFROMSHORT(LIT_FIRST),
                                           MPFROMP(NULL));

                pData->pfnSelected = (PFONTNAME)
                                     WinSendDlgItemMsg(hWnd,
                                                       DLG_FONT_LB,
                                                       LM_QUERYITEMHANDLE,
                                                       MPFROMSHORT(usItem),
                                                       MPFROMP(NULL));
                DismissDialog(hWnd, pData, TRUE);

           break;

           /*���������������������������������������������������������������Ŀ*/
           /*� User cancelled exclusion/inclusion                            �*/
           /*�����������������������������������������������������������������*/

           case DID_CANCEL:

                DismissDialog(hWnd, pData, FALSE);

           break;
        }

        break;                                        /* Break WM_COMMAND           */

      /*��������������������������������������������������������������������Ŀ*/
      /*� WM_CLOSE:                                                          �*/
      /*� - Dismiss the dialog, but DO NOT use results...                    �*/
      /*����������������������������������������������������������������������*/

      case WM_CLOSE:
           DismissDialog(hWnd, pData, FALSE);
      break;

      /*��������������������������������������������������������������������Ŀ*/
      /*� WM_INITDLG:                                                        �*/
      /*�                                                                    �*/
      /*� - save dialog instance data pointer                                �*/
      /*�                                                                    �*/
      /*� - MP2 = PFONT_DATA                                                 �*/
      /*�                                                                    �*/
      /*����������������������������������������������������������������������*/

      case WM_INITDLG:
           {
             PFONTNAME  pFont;

             /*�������������������������������������������������������������Ŀ*/
             /*� Save Dialog's instance data pointer (passed in MP2)         �*/
             /*���������������������������������������������������������������*/

             WinSetWindowPtr(hWnd, QWL_USER, pData = (PFONT_DATA)mp2);

             /*�������������������������������������������������������������Ŀ*/
             /*� Add all FONTS to list box                                   �*/
             /*���������������������������������������������������������������*/

             for( /* Initialize */  pFont = pData->pfnList; /* @ first FONT */
                  /* Terminate  */  *pFont[0] != 0;         /* stop at EOT */
                  /* Iterate    */  pFont++                 /* try next FONT */
                )
             {
               usItem = (USHORT)
                         WinSendDlgItemMsg(hWnd,
                                           DLG_FONT_LB,
                                           LM_INSERTITEM,
                                           MPFROMSHORT(LIT_SORTASCENDING),
                                           MPFROMP(pFont) ); /* FONT Name */
               WinSendDlgItemMsg(hWnd,
                                 DLG_FONT_LB,
                                 LM_SETITEMHANDLE,
                                 MPFROMSHORT(usItem),
                                 MPFROMP(pFont) );
             }

            /*��������������������������������������������������������������Ŀ*/
            /*� See if "current" FONT exists...automatically select and      �*/
            /*� make it the "top" item                                       �*/
            /*����������������������������������������������������������������*/

            if ( (pData->pfnCurrent != NULL ) &&
                 ( (usItem = SHORT1FROMMR(
                             WinSendDlgItemMsg(hWnd,
                                               DLG_FONT_LB,
                                               LM_SEARCHSTRING,
                                               MPFROM2SHORT(LSS_CASESENSITIVE |
                                                            LSS_PREFIX,
                                                            LIT_FIRST),
                                               MPFROMP(pData->pfnCurrent))) )
                   != (USHORT)LIT_NONE )
               )
            {
              WinSendDlgItemMsg(hWnd,                    /* select "current" */
                                DLG_FONT_LB,
                                LM_SELECTITEM,
                                MPFROMSHORT(usItem),
                                MPFROMSHORT(TRUE));

              WinSendDlgItemMsg(hWnd,                    /* make "top" item */
                                DLG_FONT_LB,
                                LM_SETTOPINDEX,
                                MPFROMSHORT(usItem),
                                MPFROMP(NULL));
            }

             CenterDialog(hWnd);                        /* Center the Dialog */

           }

        break;

      /*��������������������������������������������������������������������Ŀ*/
      /*� We don't need to handle any other messages...                      �*/
      /*�                                                                    �*/
      /*� If this isn't an IPF message, let PM do it's default "thing"       �*/
      /*����������������������������������������������������������������������*/

      default:
           if ( !HandleIPF(hWnd, usMsg, mp1, mp2, &dpResult) )
             dpResult = WinDefDlgProc(hWnd,       /* Dialog Handle                */
                                      usMsg,      /* Message                      */
                                      mp1,        /* First parameter for message. */
                                      mp2);       /* Second parameter for message.*/

   } /* End MSG switch */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Exit                                                                   �*/
  /*��������������������������������������������������������������������������*/

  return( dpResult );
}
