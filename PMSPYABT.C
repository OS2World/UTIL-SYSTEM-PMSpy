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
/*� PMSPYABT.C                                                               �*/
/*�                                                                          �*/
/*� Window Procedure to display the PMSPY Logo                               �*/
/*����������������������������������������������������������������������������*/

/*��������������������������������������������������������������������������Ŀ*/
/*� Includes                                                                 �*/
/*����������������������������������������������������������������������������*/

#include "pmspy.h"                      /* Resource symbolic identifiers  */

MRESULT SpyLogoDlgProc(register HWND   hwnd,
                       register ULONG  msg,
                                MPARAM mp1,
                                MPARAM mp2)

{
  MRESULT dpResult = NULL;

  switch(msg)
  {
    case WM_CLOSE:

         WinDismissDlg(hwnd,0);

         break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� WM_INITDLG:                                                          �*/
    /*�                                                                      �*/
    /*� - Position window in center of Desk Top                              �*/
    /*������������������������������������������������������������������������*/

    case WM_INITDLG:                                /* Display Caption & edits      */
         {
                    HWND   abouthwnd;               /* child window handle(for about dialog) */

                    ULONG  style;                   /* window style in reserved window word's memory */

                    HPOINTER hicon;                 /* handle of the program icon */

           /*���������������������������������������������������������������Ŀ*/
           /*� We will now add the PMSPY icon to the about dialog box area by�*/
           /*� issuing the following PM commands:                            �*/
           /*�����������������������������������������������������������������*/

           abouthwnd=WinWindowFromID(               /* Get the window icon    */
                                hwnd,               /*   Parent window Handle */
                                IDD_ABOUTICON);     /*   Child ID             */

           style=WinQueryWindowULong(               /* Get the window style   */
                                     abouthwnd,     /*   About Window Handle  */
                                     QWL_STYLE);    /*   ULong to get is window style  */

           WinSetWindowULong(                       /* Set the window style   */
                             abouthwnd,             /*   About Window Handle  */
                             QWL_STYLE,             /*   ULong to set         */
                             (style & (ULONG)(~SS_TEXT)) | SS_ICON);

           hicon=WinLoadPointer(                     /* Get the window icon   */
                                HWND_DESKTOP,        /*   DeskTop Window hand */
                                hmodNLS,             /*   Resource Module hand*/
                                ID_MAINWND);         /*   Res ID or ptr to be loaded */

           WinSendDlgItemMsg(                /* Send msg to dialog item, ABOUTICON  */
                             hwnd,           /*   Parent Window Handle(dialog)*/
                             IDD_ABOUTICON,  /*   dialog item ID              */
                             SM_SETHANDLE,   /*   message ID                  */
                             (MPARAM)hicon,  /*   parm1 is ptr from res file  */
                             NULL);          /*   msg parm2 is NULL           */

           CenterDialog(hwnd);               /* Center the Dialog             */

         }
         break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� WM_COMMAND:                                                          �*/
    /*�                                                                      �*/
    /*� - switch on user command value                                       �*/
    /*������������������������������������������������������������������������*/

    case WM_COMMAND:

         switch ( SHORT1FROMMP(mp1) )
         {
           /*���������������������������������������������������������������Ŀ*/
           /*� OK pressed:                                                   �*/
           /*�����������������������������������������������������������������*/

           case DID_OK:

                WinDismissDlg(hwnd, TRUE);

                break;

           default:
                dpResult = WinDefDlgProc( hwnd, msg, mp1, mp2 );
         }

         break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� We don't need to handle any other messages...                        �*/
    /*�                                                                      �*/
    /*� If this isn't an IPF message, let PM do it's default "thing"         �*/
    /*������������������������������������������������������������������������*/

    default:
         if ( !HandleIPF(hwnd, msg, mp1, mp2, &dpResult) )
           dpResult = WinDefDlgProc( hwnd, msg, mp1, mp2 );

  }

  return( dpResult );
}
