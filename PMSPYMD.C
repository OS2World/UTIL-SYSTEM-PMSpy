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
/*� PMSPYMD.C                                                                �*/
/*�                                                                          �*/
/*� Dialog procedure to display MSG's field decomposition                    �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
#include "pmspy.h"                      /* Resource symbolic identifiers      */
#include "pmspyMP.h"                    /* MP externals                       */

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� LoadMpFormats                                                            �*/
/*�                                                                          �*/
/*� - common subroutine to 'set up' the MP formats drop-down list            �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
static
VOID
LoadMpFormats(HWND      hwndDlg,
              USHORT    usDropDownList,
              MP_FORMAT defMP)
{
  USHORT          usThisItem;                  /* where this MP added to list */
  MP_FORMAT       mpThis;                      /* current MP Format           */

  /**************************************************************************
  * Determine HWND of drop-down list once
  **************************************************************************/
  HWND     hwndDD = WinWindowFromID(hwndDlg, usDropDownList);

  /**************************************************************************
  * Add all MP format items to the specified drop-down list
  **************************************************************************/
 for( mpThis = MP_First;       /* Initialize: start @ first         */
      mpThis < MP_Total;       /* While:      still more MP formats */
      mpThis++                 /* Iterate:    try the next          */
    )
 {
   /************************************************************************
   * Add this MP format item remembering "where" it went
   ************************************************************************/
   usThisItem = SHORT1FROMMR(WinSendMsg(hwndDD,
                                        LM_INSERTITEM,
                                        MPFROMSHORT(LIT_END),
                                        MPFROMP(MpDisplayText(mpThis))) );

   /************************************************************************
   * Set item's "handle" to MP format value
   ************************************************************************/
   WinSendMsg(hwndDD,                    /* Window Handle              */
              LM_SETITEMHANDLE,          /* Message                    */
              MPFROMSHORT(usThisItem),   /* Index    of list item      */
              MPFROMSHORT(mpThis) );     /* "Handle" of list item      */

   /************************************************************************
   * If this is the designated "default" MP format items, select it
   ************************************************************************/
   if (mpThis == defMP)
     WinSendMsg(hwndDD,
                LM_SELECTITEM,                  /* Select item    */
                MPFROMSHORT(usThisItem),        /* Item   = This  */
                MPFROMSHORT(TRUE));             /* Select = Yes   */
 }
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� MSG Decompositon Dialog Procedure                                      �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
MRESULT MsgDlgProc(HWND    hwnd,
                   MSG     message,
                   MPARAM  lParam1,
                   MPARAM  lParam2 )
{
  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Local Variables                                                        �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  MRESULT   dpResult  = NULL;

  CHAR      szFmt [256],                          /* formatting specification */
            szText[256];                          /* formatting area          */

  USHORT    usThisItem;                           /* list item                */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Locate dialog's instance data                                          �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  PMD_DATA pData = WinQueryWindowPtr(hwnd, QWL_USER);

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Process the message                                                    �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  switch (message)
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� "Done" PUSHBUTTON has been used or dialog close requested          �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_COMMAND:
    case WM_CLOSE:

         WinDismissDlg(hwnd, 0);

    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Notification from one of the dialog controls....                   �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_CONTROL:
          /******************************************************************
          * One of our drop-down list items have new selection?
          ******************************************************************/
          if (
                 (SHORT2FROMMP(lParam1) == CBN_EFCHANGE) &&
               ( (SHORT1FROMMP(lParam1) == DLG_MD_DD_MP1) ||
                 (SHORT1FROMMP(lParam1) == DLG_MD_DD_MP2) )
             )
          {
            MP_FORMAT mpThis;                     /* current MP Format        */

            /****************************************************************
            * Determine "new" drop-down list item
            ****************************************************************/
            usThisItem = SHORT1FROMMR( WinSendMsg(HWNDFROMMP(lParam2),
                                                  LM_QUERYSELECTION,                  /* Select item    */
                                                  MPFROMSHORT(LIT_FIRST),
                                                  NULL) );

            /****************************************************************
            * Query item's "handle" to MP table item
            *****************************************************************/
            mpThis = (MP_FORMAT)
                     SHORT1FROMMR( WinSendMsg(HWNDFROMMP(lParam2),
                                              LM_QUERYITEMHANDLE,
                                              MPFROMSHORT(usThisItem),
                                              NULL) );

            /****************************************************************
            * Format MP1/MP2 using selected value
            *****************************************************************/
            if ( SHORT1FROMMP(lParam1) == DLG_MD_DD_MP1 )
            {
              WinSetWindowText( WinWindowFromID(hwnd, DLG_MD_ST_MP1),
                                MpFormat(pData->qmsgItem.mp1,
                                         pData->pMsg->fMP1 = mpThis,
                                         szText) );
            }
            else
            {
              WinSetWindowText( WinWindowFromID(hwnd, DLG_MD_ST_MP2),
                                MpFormat(pData->qmsgItem.mp2,
                                         pData->pMsg->fMP2 = mpThis,
                                         szText) );
            }
          }
    break;

     /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     /*� WM_INITDLG:                                                       �*/
     /*�                                                                   �*/
     /*� - MP2 is dialog instance's data                                   �*/
     /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     case WM_INITDLG:
          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Save Dialog's instance data pointer (passed in MP2)          �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          WinSetWindowPtr(hwnd, QWL_USER, pData = PVOIDFROMMP(lParam2));

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Setup the Dialog for processing...                           �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          CenterDialog(hwnd);                                /* Center DLG  */

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Set up Dialog's title to include MSG name                    �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          WinQueryWindowText(hwnd, sizeof(szFmt), szFmt);

          sprintf(szText, szFmt, pData->pMsg->pDesc);

          WinSetWindowText(hwnd, szText);

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Display MSG's value                                          �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          sprintf(szText,
                  "0x%4.4X (%d)",
                  pData->pMsg->Msg,
                  pData->pMsg->Msg);

          WinSetWindowText( WinWindowFromID(hwnd, DLG_MD_ST_MSG), szText);

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Display MSG "seen" values                                    �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          WinQueryWindowText( WinWindowFromID(hwnd, DLG_MD_ST_SEEN),
                              sizeof(szFmt), szFmt);

          sprintf( szText,
                   szFmt,
                   pData->pMsg->aulTimes[MSG_TIMES_EVER],
                   pData->pMsg->aulTimes[MSG_TIMES_SINCE]
                 );

          WinSetWindowText( WinWindowFromID(hwnd, DLG_MD_ST_SEEN), szText);

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Set MP format values                                         �*/
          /*�                                                              �*/
          /*� Note: WM_CONTROL messages are generated by the following     �*/
          /*�       which cause formatting of MP1 / MP2 data (see above)   �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          LoadMpFormats(hwnd, DLG_MD_DD_MP1, pData->pMsg->fMP1);
          LoadMpFormats(hwnd, DLG_MD_DD_MP2, pData->pMsg->fMP2);

      break;

     /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     /*� We don't need to handle any other messages...                     �*/
     /*�                                                                   �*/
     /*� If this isn't an IPF message, let PM do it's default "thing"      �*/
     /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     default:
          if ( !HandleIPF(hwnd, message, lParam1, lParam2, &dpResult) )
            dpResult = WinDefDlgProc(hwnd, message, lParam1, lParam2 );
   }

   /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
   /*� Exit                                                                  �*/
   /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
   return( dpResult );
}
