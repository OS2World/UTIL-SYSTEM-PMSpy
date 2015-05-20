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
/*� PMSPYOW.C                                                                �*/
/*�                                                                          �*/
/*� Dialog procedure to display and select an OBJECT window for SPYing       �*/
/*����������������������������������������������������������������������������*/

#include "pmspy.h"                      /* Resource symbolic identifiers*/

#define  SYSTEM_OBJECT_WINDOW_ID '#'    /* first char of SYSTEM object class name */

typedef struct
{
  HWND       hwnd;
  char       szClass[40];
  PID        pidCreator;
  TID        tidCreator;
  CLASSINFO  classInfo;
  HMQ        hmq;
  BOOL       fIsOBJECT;

}    OW_INFO,
  * POW_INFO;

/*��������������������������������������������������������������������������Ŀ*/
/*� Routine to add one window to the LISTBOX                                 �*/
/*����������������������������������������������������������������������������*/
static
BOOL                                     /* TRUE = included, FALSE = excluded */
AddOneWindowToList( HWND     hwndLB,     /* window of LISTBOX                 */
                    HWND     hwndWindow, /* window to add                     */
                    POW_DATA pData)      /* Dialog's data                     */
{
  BOOL     fIncluded = BOOL_TRUE;

  USHORT   list_index;

  OW_INFO  objData;

  POW_INFO pOW;

  char     szData  [128],
           szHWND  [20],
           szHMQ   [20],
           szParent[40];

  PSZ      pszClass;

  memset(&objData, 0, sizeof(objData) );

  objData.hwnd      = hwndWindow;
  objData.fIsOBJECT = pData->fIsOBJECT;

  WinQueryClassName(hwndWindow, sizeof(objData.szClass), objData.szClass);

  WinQueryClassInfo(WinQueryAnchorBlock(hwndLB),
                    objData.szClass,
                    &objData.classInfo);

  WinQueryWindowProcess(hwndWindow, &objData.pidCreator, &objData.tidCreator);

  objData.hmq = (HMQ)WinQueryWindowULong(hwndWindow, QWL_HMQ);

  if ( objData.hmq == pData->hmqSPY )         /* this SPY agent's window?     */
  {
    fIncluded = BOOL_FALSE;                   /* Yes: exclude...              */
  }
  else
  {
    pszClass = TranslateClassName( objData.szClass );  /* translate SYSTEM */

    sprintf(szHWND  , "%8.8lX",   hwndWindow);
    sprintf(szHMQ   , "%8.8lX",   objData.hmq);

    sprintf(szParent, "%s.%u.%u", pData->pszType,
                                  objData.pidCreator,
                                  objData.tidCreator );

    if ( pszClass != objData.szClass )  /* Class name translated? */
      sprintf( szData,                  /* yes... */
               "Hmq: %s Hwnd: %s Linage: %-12.12s Class: %s (%s)",
               szHMQ,
               szHWND,
               szParent,
               pszClass,
               objData.szClass
             );
    else                                /* no... */
      sprintf( szData,
               "Hmq: %s Hwnd: %s Linage: %-12.12s Class: %s",
               szHMQ,
               szHWND,
               szParent,
               objData.szClass
             );

    pOW = (POW_INFO) AllocateMemory( sizeof( OW_INFO) );

    *pOW = objData;

    list_index = SHORT1FROMMR(WinSendMsg(hwndLB,
                                         LM_INSERTITEM,    /* Insert LB item    */
                                         MPFROMSHORT(LIT_SORTASCENDING),/* sort */
                                         MPFROMP(szData)) );        /*   Text   */

    WinSendMsg(hwndLB,                    /*   Window Handle              */
               LM_SETITEMHANDLE,          /*   Message                    */
               MPFROMSHORT(list_index),   /*   Index of list item         */
               MPFROMP(pOW) );            /*   Handle of list item        */
  }

  return( fIncluded );
}

/*��������������������������������������������������������������������������Ŀ*/
/*� Routine to recursively add all child windows to the LISTBOX              �*/
/*����������������������������������������������������������������������������*/
static
void
AddWindowChildrenToList(HWND     hwndLB,       /* window of LISTBOX           */
                        HWND     hwndWindow,   /* parent window               */
                        POW_DATA pData)        /* Dialog's data               */
{
  HWND     hwndChild;

  HENUM    hEnum = WinBeginEnumWindows( hwndWindow );

  /* process all child windows of this window */

  for( /* Initialize */ hwndChild  = WinGetNextWindow(hEnum);
       /* While      */ hwndChild != NULLH;
       /* Iterate    */ hwndChild  = WinGetNextWindow(hEnum) )
  {
     AddOneWindowToList( hwndLB,               /* window of LISTBOX */
                         hwndChild,            /* window to add     */
                         pData);               /* Dialog's data     */

     AddWindowChildrenToList( hwndLB,          /* window of LISTBOX    */
                              hwndChild,       /* add child's children */
                              pData);          /* Dialog's data        */

     WinLockWindowUpdate(hwndChild, BOOL_FALSE); /* unLOCK the window */
  }

  WinEndEnumWindows( hEnum );
}

/*��������������������������������������������������������������������������Ŀ*/
/*� Routine to delete all existing OBJECT windows in the listbox             �*/
/*�                                                                          �*/
/*� - for performance and a visually 'nice' appear, we'll delete             �*/
/*�   list items at once (after dealing with each's HANDLE data)             �*/
/*����������������������������������������������������������������������������*/
static VOID DeleteList(HWND hwndLB)   /* handle of LISTBOX      */

{
           PSZ    pData;              /* data for current list item */

  register USHORT uItem;              /* deletion loop counter */

  register USHORT uTotalItems = SHORT1FROMMR(WinSendMsg(hwndLB,
                                                        LM_QUERYITEMCOUNT,
                                                        NULL,
                                                        NULL) );

  /*������������������������������������������������������������������������Ŀ*/
  /*� Delete HANDLE data for every item                                      �*/
  /*��������������������������������������������������������������������������*/
  for( /* Initialization */  uItem = 0;                /* start @ first Item */
       /* While          */  uItem < uTotalItems;      /* while not all processed */
       /* Iterate        */  uItem++)                  /* to the next Item */
  {
    /*����������������������������������������������������������������������Ŀ*/
    /*� Obtain item's "handlized" data and free allocated memory             �*/
    /*������������������������������������������������������������������������*/
    pData = (PSZ) WinSendMsg( hwndLB,
                              LM_QUERYITEMHANDLE, /* give me HANDLE */
                              MPFROMSHORT(uItem), /* current Item # */
                              NULL);              /* Reserved */
    if (pData != NULL)
      FreeMemory( pData );
  }

  /*������������������������������������������������������������������������Ŀ*/
  /*� Delete ALL item(s) in list at once (quick and more visually pleasing   �*/
  /*� than do one at a time in the above loop)                               �*/
  /*��������������������������������������������������������������������������*/
  WinSendMsg( hwndLB,
              LM_DELETEALL,     /* Delete ALL Item */
              NULL,             /* Reserved */
              NULL);            /* Reserved */
}

/*��������������������������������������������������������������������������Ŀ*/
/*� Routine to generate the list of windows                                  �*/
/*����������������������������������������������������������������������������*/
static USHORT GenerateList( HWND     hwndDlg,      /* handle of DIALOG        */
                            USHORT   idList,       /* ID of listbox in DIALOG */
                            USHORT   idCount,      /* ID of "count" in DIALOG */
                            POW_DATA pData)        /* Dialog's data           */
{
  register USHORT uNumberOfWindows;
  CHAR            szData[128];

  HWND     hwndLB = WinWindowFromID(hwndDlg,idList);  /* calculate HWND once! */

  /*������������������������������������������������������������������������Ŀ*/
  /*� First of all, "lock" the ListBox to minimize bad visual                �*/
  /*� effect from the deletion and insertion of the VIEWs...                 �*/
  /*��������������������������������������������������������������������������*/
  WinLockWindowUpdate(HWND_DESKTOP,          /* Desktop */
                      hwndLB);               /* Lockee */

  /* start by erasing any existing windows in the listbox */

  DeleteList( hwndLB );      /* LISTBOX window */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Add DESKTOP windows if specified                                       �*/
  /*��������������������������������������������������������������������������*/
  if (pData->fDESKTOP)
  {
    pData->pszType   = "DSK";
    pData->fIsOBJECT = BOOL_FALSE;

    if ( AddOneWindowToList( hwndLB,          /* window of LISTBOX */
                             HWND_DESKTOP,    /* window of OBJECT  */
                             pData) )         /* dialog's data     */
    {
      AddWindowChildrenToList( hwndLB,        /* window of LISTBOX */
                               HWND_DESKTOP,  /* parent window     */
                               pData);        /* dialog's data     */
    }
  }

  /*������������������������������������������������������������������������Ŀ*/
  /*� Add OBJECT  windows if specified                                       �*/
  /*��������������������������������������������������������������������������*/
  if (pData->fOBJECT)
  {
    pData->pszType   = "OBJ";
    pData->fIsOBJECT = BOOL_TRUE;

    if ( AddOneWindowToList( hwndLB,          /* window of LISTBOX */
                             HWND_OBJECT,     /* window of OBJECT  */
                             pData) )         /* dialog's data     */
    {
      AddWindowChildrenToList( hwndLB,        /* window of LISTBOX */
                               HWND_OBJECT,   /* parent window     */
                               pData);        /* dialog's data     */
    }
  }

  /*������������������������������������������������������������������������Ŀ*/
  /*� Select first item in the ListBox                                       �*/
  /*��������������������������������������������������������������������������*/
  WinSendMsg(hwndLB,                 /* Select 1st item      */
             LM_SELECTITEM,          /* msg: select item     */
             MPFROMSHORT( 0 ),       /* mp1: FIRST item      */
             MPFROMSHORT( TRUE ));   /* mp2: SELECT item     */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Finally, "unlock" the ListBox so it is only redrawn once               �*/
  /*��������������������������������������������������������������������������*/
  WinLockWindowUpdate(HWND_DESKTOP,  /* Desktop */
                      NULLH);        /* Re-enable ListBox */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Determine current number of windows in LISTBOX                         �*/
  /*��������������������������������������������������������������������������*/
  uNumberOfWindows = SHORT1FROMMR( WinSendMsg(hwndLB,
                                              LM_QUERYITEMCOUNT,
                                              NULL, NULL) );

  /*������������������������������������������������������������������������Ŀ*/
  /*� Display the current window "count" in the dialog                       �*/
  /*��������������������������������������������������������������������������*/
  {
    static   CHAR   szCountFmt[128] = "";            /* dialog "count" format   */

    /*************************************************************
    * if necesarry, load Format string from dialog template
    *************************************************************/
    if (szCountFmt[0] == 0)
    {
      WinQueryDlgItemText(hwndDlg, idCount, sizeof(szCountFmt), szCountFmt);
    }

    /*************************************************************
    * Format to include number of windows
    *************************************************************/
    sprintf(szData,                      /* target formating area  */
            szCountFmt,                  /* DLG format string      */
            uNumberOfWindows);           /* Insert: # windows      */

    /*************************************************************
    * Update dialog's visible text
    *************************************************************/
    WinSetDlgItemText(hwndDlg, idCount, szData);
  }

  return( uNumberOfWindows );
}

/*��������������������������������������������������������������������������Ŀ*/
/*� Internal Function: DismissDialog                                         �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� Function:                                                                �*/
/*�                                                                          �*/
/*�   This module: 1) sets the flag indicating if entered data should        �*/
/*�                   be used by invoker of dialog                           �*/
/*�                2) POSTs message to invoker of dialog                     �*/
/*�                3) Delete all LISTBOX items                               �*/
/*�                4) Dismisses the dialog                                   �*/
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
                          POW_DATA          pDlgData,
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
  /*� Delete all list items                                                  �*/
  /*��������������������������������������������������������������������������*/
  DeleteList(  WinWindowFromID( hwndDlg, ID_OW_LB_WINDOWS) );

  /*������������������������������������������������������������������������Ŀ*/
  /*� Dismiss the dialog                                                     �*/
  /*��������������������������������������������������������������������������*/
  WinDismissDlg( hwndDlg, fDlgUse);        /* Close the Dialog box         */
}

  /*������������������������������������������������������������������������Ŀ*/
  /*� Dialog Procedure for displaying list of OBJECT windows                 �*/
  /*��������������������������������������������������������������������������*/

 MRESULT ObjectDlgProc(          HWND    hwnd,
                        register ULONG   message,
                                 MPARAM  lParam1,
                                 MPARAM  lParam2 )

{

  /*������������������������������������������������������������������������Ŀ*/
  /*� Local Variables                                                        �*/
  /*��������������������������������������������������������������������������*/

  MRESULT   dpResult  = NULL;
  POW_INFO  pOW;

  /*������������������������������������������������������������������������Ŀ*/
  /*� Obtain pointer to this dialog's instance data                          �*/
  /*��������������������������������������������������������������������������*/

  POW_DATA pData = (POW_DATA) WinQueryWindowPtr(hwnd, QWL_USER);

    /*����������������������������������������������������������������������Ŀ*/
    /*� Process the message                                                  �*/
    /*������������������������������������������������������������������������*/

    switch (message)                       /* Switch off of the message ID */
      {
        /*������������������������������������������������������������������Ŀ*/
        /*� One of the PUSHBUTTON has been used...                           �*/
        /*��������������������������������������������������������������������*/
        case WM_COMMAND:

          switch( LOUSHORT( lParam1 ) )
          {
              /*������������������������������������������������������������Ŀ*/
              /*� process the selected window                                �*/
              /*��������������������������������������������������������������*/
              case ID_OW_PB_SPY_HWND:
              case ID_OW_PB_SPY_HMQ:
              {
                USHORT        curItem;                 /* current list item */

                if ( (curItem = SHORT1FROMMR(
                                WinSendDlgItemMsg(      /* Query 1st selection          */
                                  hwnd,                 /*   Dialog handle              */
                                  ID_OW_LB_WINDOWS,     /*   Listbox ID                 */
                                  LM_QUERYSELECTION,    /*   Query ID of selected       */
                                  MPFROMSHORT(LIT_FIRST),
                                  NULL)) ) != (USHORT)LIT_NONE )
                {
                  pOW = PVOIDFROMMR( WinSendDlgItemMsg(hwnd,
                                                       ID_OW_LB_WINDOWS,
                                                       LM_QUERYITEMHANDLE,
                                                       MPFROMSHORT(curItem),
                                                       NULL) );

                  /*��������������������������������������������������������Ŀ*/
                  /*� Switch to SPYing this window...                        �*/
                  /*����������������������������������������������������������*/
                  pData->hwndOW        = pOW->hwnd;
                  pData->hmqOW         = pOW->hmq;

                  pData->fHwndIsOBJECT = pOW->fIsOBJECT;

                  pData->fSpyHWND = SHORT1FROMMP(lParam1) == ID_OW_PB_SPY_HWND
                                    ? BOOL_TRUE
                                    : BOOL_FALSE;

                  DismissDialog(hwnd, pData, TRUE);  /* end dialog (do USE data) */
                }
                else
                  WinAlarm(HWND_DESKTOP, WA_ERROR);
              }
              break;

              /*������������������������������������������������������������Ŀ*/
              /*� refresh the list with all windows                          �*/
              /*��������������������������������������������������������������*/
              case ID_OW_PB_REFRESH:

                   pData->fDESKTOP =                   /* query current state */
                             SHORT1FROMMR(WinSendDlgItemMsg(hwnd,
                                                            ID_OW_CB_DESKTOP,
                                                            BM_QUERYCHECK,
                                                            NULL,
                                                            NULL));

                   pData->fOBJECT  =                   /* query current state */
                             SHORT1FROMMR(WinSendDlgItemMsg(hwnd,
                                                            ID_OW_CB_OBJECT,
                                                            BM_QUERYCHECK,
                                                            NULL,
                                                            NULL));

                   GenerateList(hwnd,
                                ID_OW_LB_WINDOWS,
                                ID_OW_ST_WINDOWS,
                                pData);            /* Dialog's data           */
              break;

              /*������������������������������������������������������������Ŀ*/
              /*� User cancelled exclusion/inclusion                         �*/
              /*��������������������������������������������������������������*/
              case DID_CANCEL:

                   DismissDialog(hwnd, pData, FALSE);  /* DO NOT use USE data */

              break;
            }

          break;                                       /* Break WM_COMMAND    */

        /*������������������������������������������������������������������Ŀ*/
        /*� Notification from one of the dialog controls....                 �*/
        /*��������������������������������������������������������������������*/
        case WM_CONTROL:

          /******************************************************************
          * One of our checkboxes?
          ******************************************************************/
          if ( ( SHORT1FROMMP(lParam1) == ID_OW_CB_DESKTOP ) ||
               ( SHORT1FROMMP(lParam1) == ID_OW_CB_OBJECT  ) )
          {
            /****************************************************************
            * Decide if the "Refresh" pull-down should be:
            *
            * - enabled  if one or both checked
            * - disabled if neither     checked
            ****************************************************************/
            WinEnableWindow(WinWindowFromID(hwnd, ID_OW_PB_REFRESH),
                            SHORT1FROMMR(WinSendDlgItemMsg(hwnd,
                                                           ID_OW_CB_DESKTOP,
                                                           BM_QUERYCHECK,
                                                           NULL,
                                                           NULL)) ||

                             SHORT1FROMMR(WinSendDlgItemMsg(hwnd,
                                                            ID_OW_CB_OBJECT,
                                                            BM_QUERYCHECK,
                                                            NULL,
                                                            NULL))
                             ? BOOL_TRUE
                             : BOOL_FALSE);
          }
          break;                                        /* Break WM_CONTROL   */

        /*������������������������������������������������������������������Ŀ*/
        /*� WM_CLOSE:                                                        �*/
        /*� - Call WinDismissDlg to close the dialog box                     �*/
        /*��������������������������������������������������������������������*/
        case WM_CLOSE:

             DismissDialog(hwnd, pData, FALSE); /* DO NOT use USE data... */

             dpResult = MRFROMSHORT( TRUE );

        break;

         /*�����������������������������������������������������������������Ŀ*/
         /*� WM_INITDLG:                                                     �*/
         /*�                                                                 �*/
         /*� - Load list with current set of OBJECT windows                  �*/
         /*�                                                                 �*/
         /*� - MP2 is dialog instance's data                                 �*/
         /*�������������������������������������������������������������������*/
         case WM_INITDLG:
              /*������������������������������������������������������������Ŀ*/
              /*� Save Dialog's instance data pointer (passed in MP2)        �*/
              /*��������������������������������������������������������������*/
              WinSetWindowPtr(hwnd, QWL_USER, pData = (POW_DATA)lParam2);

              pData->fUseData = BOOL_FALSE;       /* start a pessemist...     */

              /*������������������������������������������������������������Ŀ*/
              /*� Setup the Dialog for processing...                         �*/
              /*��������������������������������������������������������������*/
              CenterDialog(hwnd);                              /* Center DLG  */

              /*������������������������������������������������������������Ŀ*/
              /*� Set default "Refresh" control checkboxes                   �*/
              /*��������������������������������������������������������������*/
              WinSendDlgItemMsg(hwnd,
                                ID_OW_CB_DESKTOP,
                                BM_SETCHECK,
                                MPFROMSHORT(pData->fDESKTOP),
                                NULL);

              WinSendDlgItemMsg(hwnd,
                                ID_OW_CB_OBJECT,
                                BM_SETCHECK,
                                MPFROMSHORT(pData->fOBJECT),
                                NULL);

              /*������������������������������������������������������������Ŀ*/
              /*� Load listbox with current set of windows                   �*/
              /*��������������������������������������������������������������*/
              SetListboxFont(WinWindowFromID(hwnd, ID_OW_LB_WINDOWS),/* set LB font */
                             (PSZ) pData->pfnCurrent);

              GenerateList(hwnd,
                           ID_OW_LB_WINDOWS,
                           ID_OW_ST_WINDOWS,
                           pData);            /* Dialog's data           */
          break;

         /*�����������������������������������������������������������������Ŀ*/
         /*� We don't need to handle any other messages...                   �*/
         /*�                                                                 �*/
         /*� If this isn't an IPF message, let PM do it's default "thing"    �*/
         /*�������������������������������������������������������������������*/
         default:
              if ( !HandleIPF(hwnd, message, lParam1, lParam2, &dpResult) )
                dpResult = WinDefDlgProc(hwnd, message, lParam1, lParam2 );

      }                                                 /* End switch                   */

   /*�����������������������������������������������������������������������Ŀ*/
   /*� Exit                                                                  �*/
   /*�������������������������������������������������������������������������*/
   return( dpResult );
}
