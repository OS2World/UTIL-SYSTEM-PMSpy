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
/*� PMSPYFIL.C                                                               �*/
/*�                                                                          �*/
/*� Message Filter Dialog Procedure                                          �*/
/*����������������������������������������������������������������������������*/

#include "pmspy.h"

/*��������������������������������������������������������������������������Ŀ*/
/*� SetColors                                                                �*/
/*�                                                                          �*/
/*� - common subroutine to 'set up' the Color listbox in various dialogs     �*/
/*����������������������������������������������������������������������������*/
VOID SetColors( HWND       hwnd,
                PSPY_DATA  pSpyData )

{
 register USHORT i;

 /* Add "Using existing color" as FIRST list item  */

 WinSendDlgItemMsg(hwnd,                           /* Window Handle  */
                   ID_LB_COLORS,                   /* Listbox ID     */
                   LM_INSERTITEM,                  /* Insert item    */
                   MPFROMSHORT(LIT_END),           /* At the End     */
                   MPFROMP(Strings[IDS_FMT_EXISTING_COLOR]) );

 /* Add all colors to the list in 'asis' order (starting at items 1 */

 for( /* Initialize */  i = 0;              /* start @ first */
      /* Terminate  */  i < Color_Total;    /* stop at End-Of-Table */
      /* Iterate    */  i++                 /* try the next */
    )
 {
   WinSendDlgItemMsg
   ( hwnd,                           /* Window Handle  */
     ID_LB_COLORS,                   /* Listbox ID     */
     LM_INSERTITEM,                  /* Insert item    */
     MPFROMSHORT(LIT_END),           /* At the End     */
     MPFROMP(ExternalColorTranslation[pSpyData->LogicalColorTranslation[i].iExtColor].pszClrName));
 }

 /* Select FIRST list item  */

 WinSendDlgItemMsg(hwnd,                           /* Window Handle  */
                   ID_LB_COLORS,                   /* Listbox ID     */
                   LM_SELECTITEM,                  /* Select item    */
                   MPFROMSHORT(0),                 /* Item = First   */
                   MPFROMSHORT(TRUE));             /* Select = Yes   */
}

/*��������������������������������������������������������������������������Ŀ*/
/*� GetColor                                                                 �*/
/*�                                                                          �*/
/*� - common subroutine to 'get the value' of the Color                      �*/
/*�   selected by the user from the color LISTBOX                            �*/
/*����������������������������������������������������������������������������*/
MSG_COLOR GetColor( HWND       hwnd,
                    PSPY_DATA  pSpyData )

{
  /*������������������������������������������������������������������������Ŀ*/
  /*� Get currently selected "color" from the list                           �*/
  /*��������������������������������������������������������������������������*/
  USHORT curItem = SHORT1FROMMR(WinSendDlgItemMsg(hwnd,
                                                  ID_LB_COLORS,
                                                  LM_QUERYSELECTION,
                                                  MPFROMSHORT(LIT_FIRST),
                                                  NULL) );

  /*������������������������������������������������������������������������Ŀ*/
  /*� The first "color" in the list is the "Leave ASIS" item                 �*/
  /*��������������������������������������������������������������������������*/
  MSG_COLOR msgColor = (MSG_COLOR)
                ( (curItem == 0)
                  ? Color_Asis
                  : pSpyData->LogicalColorTranslation[curItem - 1].pbValue );

  return( msgColor );
}

/*��������������������������������������������������������������������������Ŀ*/
/*� FormatMSG                                                                �*/
/*�                                                                          �*/
/*� - common subroutine to format a list item for a specific MSG             �*/
/*����������������������������������������������������������������������������*/
static
PSZ
FormatMSG(PMSG_ITEM   pMsg,
          PSZ         pszFormatArea)
{
  sprintf(pszFormatArea,
          Strings[IDS_FMT_FILTER_ITEM],

          pMsg->pDesc,                     /* wording of Msg */
          pMsg->Msg,                       /* value   of Msg */

          pMsg->Include        ? Strings[IDS_MSG_INCLUDE]
                               : Strings[IDS_MSG_EXCLUDE],

          pMsg->TriggerThaw    ? Strings[IDS_MSG_THAW]
                               : "",

          pMsg->TriggerFreeze  ? Strings[IDS_MSG_FREEZE]
                               : ""
         );

  return(pszFormatArea);
}

/*��������������������������������������������������������������������������Ŀ*/
/*� Dialog procedure                                                         �*/
/*����������������������������������������������������������������������������*/
 MRESULT FilterDlgProc(          HWND    hwnd,
                        register ULONG   message,
                                 MPARAM  lParam1,
                                 MPARAM  lParam2 )

  {
     /*���������������������������������������������������������������������Ŀ*/
     /*� Local Variables                                                     �*/
     /*�����������������������������������������������������������������������*/
     MRESULT            dpResult = NULL;

     register USHORT    pbSelected;
     CHAR               sFilterItem[128];
     PMSG_ITEM          pMsg;

     static PSPY_DATA   pSpyData = NULL;

     HWND               hwndLB;                /* HWND of message listbox */

    /*����������������������������������������������������������������������Ŀ*/
    /*� Process the message                                                  �*/
    /*������������������������������������������������������������������������*/
    switch (message)                          /* Switch off of the message ID */
    {
        /*������������������������������������������������������������������Ŀ*/
        /*� One of our pushbuttons was pressed...                            �*/
        /*��������������������������������������������������������������������*/
        case WM_COMMAND:

          switch( pbSelected = LOUSHORT( lParam1 ) )
          {
              /*������������������������������������������������������������Ŀ*/
              /*� Process selected items                                     �*/
              /*��������������������������������������������������������������*/
              default:
              {
                USHORT        curItem,            /* current list item        */
                              nSelected;          /* Number of items selected */

                MSG_COLOR     color = GetColor( hwnd, pSpyData);

                /*����������������������������������������������������������Ŀ*/
                /*� Obtain HWND of message listbox once for performance      �*/
                /*������������������������������������������������������������*/
                hwndLB = WinWindowFromID(hwnd, IDD_FILTER);

                /************************************************************
                * "Lock" the ListBox to minimize performance impact
                ************************************************************/
                WinLockWindowUpdate(HWND_DESKTOP,     /* Desktop */
                                    hwndLB);          /* Lockee  */

                /************************************************************
                * Process the "selected" ListBox items
                ************************************************************/
                for(nSelected = 0,                    /* Initialize */
                    curItem   = SHORT1FROMMR(WinSendMsg(hwndLB,
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT(LIT_FIRST),
                                                        NULL) );

                    curItem != (USHORT)LIT_NONE;      /* While      */

                    nSelected++,                     /* Iterate     */
                    curItem   = SHORT1FROMMR(WinSendMsg(hwndLB,
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT(curItem),
                                                        NULL) )
                   )
                {
                  /*��������������������������������������������������������Ŀ*/
                  /*� Locate specific MSG item for this selected item        �*/
                  /*����������������������������������������������������������*/
                  pMsg = PVOIDFROMMR( WinSendMsg(hwndLB,
                                                 LM_QUERYITEMHANDLE,
                                                 MPFROMSHORT(curItem),
                                                 NULL) );

                  /*��������������������������������������������������������Ŀ*/
                  /*� Perform action on this selected item                   �*/
                  /*����������������������������������������������������������*/
                  switch( pbSelected )
                  {
                    /*********************************************************
                    * Delete this MSG's definition
                    **********************************************************/
                    case ID_PB_F_DELETE:

                         DeleteMsg(pSpyData, pMsg->Msg);

                         WinSendMsg(hwndLB,
                                    LM_DELETEITEM,
                                    MPFROMSHORT(curItem),
                                    MPFROMP(NULL));

                         curItem = (USHORT)( (curItem == 0)
                                             ? LIT_FIRST
                                             : (curItem - 1) );
                    break;

                    /*********************************************************
                    * Mark that this MSG can NOT be "spied" / act as "trigger"
                    **********************************************************/
                    case ID_PB_F_EXCLUDE:

                         IncludeMsg(pSpyData, pMsg->Msg, BOOL_FALSE, color);

                         WinSendMsg(hwndLB,
                                    LM_SETITEMTEXT,
                                    MPFROMSHORT(curItem),
                                    MPFROMP(FormatMSG(pMsg, sFilterItem)));
                    break;

                    /*********************************************************
                    * Mark that this MSG can be "spied" / act as "trigger"
                    **********************************************************/
                    case ID_PB_F_INCLUDE:

                         IncludeMsg(pSpyData, pMsg->Msg, BOOL_TRUE, color);

                         WinSendMsg(hwndLB,
                                    LM_SETITEMTEXT,
                                    MPFROMSHORT(curItem),
                                    MPFROMP(FormatMSG(pMsg, sFilterItem)));
                    break;

                    /*********************************************************
                    * Set FREEZE "trigger"
                    **********************************************************/
                    case ID_PB_F_FREEZE:

                         pMsg->TriggerFreeze  = BOOL_TRUE;
                         pMsg->TriggerThaw    = BOOL_FALSE;

                         WinSendMsg(hwndLB,
                                           LM_SETITEMTEXT,
                                           MPFROMSHORT(curItem),
                                           MPFROMP(FormatMSG(pMsg, sFilterItem)));

                         if (color != Color_Asis)
                           pMsg->ClrFG = color;
                    break;

                    /*********************************************************
                    * Set THAW "trigger"
                    **********************************************************/
                    case ID_PB_F_THAW:

                         pMsg->TriggerThaw    = BOOL_TRUE;
                         pMsg->TriggerFreeze  = BOOL_FALSE;

                         WinSendMsg(hwndLB,
                                    LM_SETITEMTEXT,
                                    MPFROMSHORT(curItem),
                                    MPFROMP(FormatMSG(pMsg, sFilterItem)));

                         if (color != Color_Asis)
                           pMsg->ClrFG = color;
                    break;

                    /*********************************************************
                    * Remove FREEZE and THAW "triggers"
                    **********************************************************/
                    case ID_PB_F_NO_TRIGGER:

                         pMsg->TriggerFreeze  =
                         pMsg->TriggerThaw    = BOOL_FALSE;

                         WinSendMsg(hwndLB,
                                    LM_SETITEMTEXT,
                                    MPFROMSHORT(curItem),
                                    MPFROMP(FormatMSG(pMsg, sFilterItem)));

                         if (color != Color_Asis)
                           pMsg->ClrFG = color;
                    break;

                    /*********************************************************
                    * Reset MSG unique data
                    **********************************************************/
                    case ID_PB_F_RESET:

                         pMsg->aulTimes[MSG_TIMES_SINCE] = 0;

                         if (color != Color_Asis)
                           pMsg->ClrFG = color;
                    break;
                  }
                }

                /************************************************************
                * Finally, "unlock" the ListBox so it is only redrawn once
                ************************************************************/
                WinLockWindowUpdate(HWND_DESKTOP, NULLHANDLE);

                /************************************************************
                * Make "noise" if no items were selected...
                ************************************************************/
                if (nSelected == 0)
                  WinAlarm(HWND_DESKTOP, WA_ERROR);
              }
              break;

              /*������������������������������������������������������������Ŀ*/
              /*� User cancelled dialog                                      �*/
              /*��������������������������������������������������������������*/
              case DID_CANCEL:

                   WinDismissDlg(hwnd, 0);              /* Close dialog */
              break;
          }
        break;                                          /* Break WM_COMMAND             */

        /*������������������������������������������������������������������Ŀ*/
        /*� WM_CLOSE:                                                        �*/
        /*� - Call WinDismissDlg to close the dialog box                     �*/
        /*��������������������������������������������������������������������*/

        case WM_CLOSE:                                  /* Close Dialog Box.    */

             WinDismissDlg( hwnd, 0);

        break;

        /*������������������������������������������������������������������Ŀ*/
        /*� WM_INITDLG:                                                      �*/
        /*� - Init pointer to data pointed to by WinDlgBox                   �*/
        /*� - Init global handle of this dlg                                 �*/
        /*�                                                                  �*/
        /*� - MP2 = PSPY_DATA                                                �*/
        /*��������������������������������������������������������������������*/
        case WM_INITDLG:
        {
               register USHORT  list_index;

               /*�����������������������������������������������������������Ŀ*/
               /*� Locate "SPY" data to utilize                              �*/
               /*�������������������������������������������������������������*/
               pSpyData = PVOIDFROMMP(lParam2);

               /*�����������������������������������������������������������Ŀ*/
               /*� Obtain HWND of message listbox once for performance       �*/
               /*�������������������������������������������������������������*/
               hwndLB = WinWindowFromID(hwnd, IDD_FILTER);

              /*������������������������������������������������������������Ŀ*/
              /*� Set listbox FONT to current non-proportional value         �*/
              /*��������������������������������������������������������������*/
              SetListboxFont(hwndLB, (PSZ) pSpyData->pfnLB);

              /************************************************************
              * "Lock" the ListBox to minimize performance impact
              ************************************************************/
              WinLockWindowUpdate(HWND_DESKTOP,     /* Desktop */
                                  hwndLB);          /* Lockee  */

              /*�����������������������������������������������������������Ŀ*/
              /*� Add all Messages to list box                              �*/
              /*�������������������������������������������������������������*/
              for( /* Initialize */  pMsg = ProcessFirstMsg(pSpyData); /* start @ first MSG */
                   /* Terminate  */  pMsg != NULL;                     /* stop at End-Of-Table */
                   /* Iterate    */  pMsg = ProcessNextMsg(pSpyData)   /* try the next MSG */
                 )
              {
                list_index = (USHORT)
                    WinSendMsg(hwndLB,
                               LM_INSERTITEM,
                               MPFROMSHORT(LIT_SORTASCENDING),
                               MPFROMP(FormatMSG(pMsg, sFilterItem)) );

                WinSendMsg(hwndLB,                    /* Window Handle              */
                           LM_SETITEMHANDLE,          /* Message                    */
                           MPFROMSHORT(list_index),   /* Index of list item         */
                           MPFROMP(pMsg));            /* Handle of list item        */
              }

              /************************************************************
              * Finally, "unlock" the ListBox so it is only redrawn once
              ************************************************************/
              WinLockWindowUpdate(HWND_DESKTOP, NULLHANDLE);

              /************************************************************
              * Other dialog set-up
              ************************************************************/
              SetColors( hwnd, pSpyData );      /* load colors       */

              CenterDialog(hwnd);               /* Center the Dialog */
        }
        break;

        /*������������������������������������������������������������������Ŀ*/
        /*� We don't need to handle any other messages...                    �*/
        /*�                                                                  �*/
        /*� If this isn't an IPF message, let PM do it's default "thing"     �*/
        /*��������������������������������������������������������������������*/
        default:
             if ( !HandleIPF(hwnd, message, lParam1, lParam2, &dpResult) )
               dpResult = WinDefDlgProc(hwnd,       /* Dialog Handle                */
                                        message,    /* Message                      */
                                        lParam1,    /* First parameter for message. */
                                        lParam2);   /* Second parameter for message.*/

      } /* End MSG switch */

   /*�����������������������������������������������������������������������Ŀ*/
   /*� Exit                                                                  �*/
   /*�������������������������������������������������������������������������*/
   return( dpResult );
}
