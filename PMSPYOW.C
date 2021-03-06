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
/*� PMSPYOW.C                                                                �*/
/*�                                                                          �*/
/*� Dialog procedure to display and select an OBJECT window for SPYing       �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

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

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine to add one window to the LISTBOX                                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine to recursively add all child windows to the LISTBOX              �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine to delete all existing OBJECT windows in the listbox             �*/
/*�                                                                          �*/
/*� - for performance and a visually 'nice' appear, we'll delete             �*/
/*�   list items at once (after dealing with each's HANDLE data)             �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
static VOID DeleteList(HWND hwndLB)   /* handle of LISTBOX      */

{
           PSZ    pData;              /* data for current list item */

  register USHORT uItem;              /* deletion loop counter */

  register USHORT uTotalItems = SHORT1FROMMR(WinSendMsg(hwndLB,
                                                        LM_QUERYITEMCOUNT,
                                                        NULL,
                                                        NULL) );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Delete HANDLE data for every item                                      �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  for( /* Initialization */  uItem = 0;                /* start @ first Item */
       /* While          */  uItem < uTotalItems;      /* while not all processed */
       /* Iterate        */  uItem++)                  /* to the next Item */
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Obtain item's "handlized" data and free allocated memory             �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    pData = (PSZ) WinSendMsg( hwndLB,
                              LM_QUERYITEMHANDLE, /* give me HANDLE */
                              MPFROMSHORT(uItem), /* current Item # */
                              NULL);              /* Reserved */
    if (pData != NULL)
      FreeMemory( pData );
  }

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Delete ALL item(s) in list at once (quick and more visually pleasing   �*/
  /*� than do one at a time in the above loop)                               �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  WinSendMsg( hwndLB,
              LM_DELETEALL,     /* Delete ALL Item */
              NULL,             /* Reserved */
              NULL);            /* Reserved */
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine to generate the list of windows                                  �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
static USHORT GenerateList( HWND     hwndDlg,      /* handle of DIALOG        */
                            USHORT   idList,       /* ID of listbox in DIALOG */
                            USHORT   idCount,      /* ID of "count" in DIALOG */
                            POW_DATA pData)        /* Dialog's data           */
{
  register USHORT uNumberOfWindows;
  CHAR            szData[128];

  HWND     hwndLB = WinWindowFromID(hwndDlg,idList);  /* calculate HWND once! */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� First of all, "lock" the ListBox to minimize bad visual                �*/
  /*� effect from the deletion and insertion of the VIEWs...                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  WinLockWindowUpdate(HWND_DESKTOP,          /* Desktop */
                      hwndLB);               /* Lockee */

  /* start by erasing any existing windows in the listbox */

  DeleteList( hwndLB );      /* LISTBOX window */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Add DESKTOP windows if specified                                       �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Add OBJECT  windows if specified                                       �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Select first item in the ListBox                                       �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  WinSendMsg(hwndLB,                 /* Select 1st item      */
             LM_SELECTITEM,          /* msg: select item     */
             MPFROMSHORT( 0 ),       /* mp1: FIRST item      */
             MPFROMSHORT( TRUE ));   /* mp2: SELECT item     */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Finally, "unlock" the ListBox so it is only redrawn once               �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  WinLockWindowUpdate(HWND_DESKTOP,  /* Desktop */
                      NULLH);        /* Re-enable ListBox */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Determine current number of windows in LISTBOX                         �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  uNumberOfWindows = SHORT1FROMMR( WinSendMsg(hwndLB,
                                              LM_QUERYITEMCOUNT,
                                              NULL, NULL) );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Display the current window "count" in the dialog                       �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
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
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

static void DismissDialog(HWND              hwndDlg,
                          POW_DATA          pDlgData,
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
  /*� Delete all list items                                                  �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  DeleteList(  WinWindowFromID( hwndDlg, ID_OW_LB_WINDOWS) );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Dismiss the dialog                                                     �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  WinDismissDlg( hwndDlg, fDlgUse);        /* Close the Dialog box         */
}

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Dialog Procedure for displaying list of OBJECT windows                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

 MRESULT ObjectDlgProc(          HWND    hwnd,
                        register ULONG   message,
                                 MPARAM  lParam1,
                                 MPARAM  lParam2 )

{

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Local Variables                                                        �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  MRESULT   dpResult  = NULL;
  POW_INFO  pOW;

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Obtain pointer to this dialog's instance data                          �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  POW_DATA pData = (POW_DATA) WinQueryWindowPtr(hwnd, QWL_USER);

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Process the message                                                  �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    switch (message)                       /* Switch off of the message ID */
      {
        /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
        /*� One of the PUSHBUTTON has been used...                           �*/
        /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
        case WM_COMMAND:

          switch( LOUSHORT( lParam1 ) )
          {
              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� process the selected window                                �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

                  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
                  /*� Switch to SPYing this window...                        �*/
                  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� refresh the list with all windows                          �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� User cancelled exclusion/inclusion                         �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
              case DID_CANCEL:

                   DismissDialog(hwnd, pData, FALSE);  /* DO NOT use USE data */

              break;
            }

          break;                                       /* Break WM_COMMAND    */

        /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
        /*� Notification from one of the dialog controls....                 �*/
        /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

        /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
        /*� WM_CLOSE:                                                        �*/
        /*� - Call WinDismissDlg to close the dialog box                     �*/
        /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
        case WM_CLOSE:

             DismissDialog(hwnd, pData, FALSE); /* DO NOT use USE data... */

             dpResult = MRFROMSHORT( TRUE );

        break;

         /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
         /*� WM_INITDLG:                                                     �*/
         /*�                                                                 �*/
         /*� - Load list with current set of OBJECT windows                  �*/
         /*�                                                                 �*/
         /*� - MP2 is dialog instance's data                                 �*/
         /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
         case WM_INITDLG:
              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� Save Dialog's instance data pointer (passed in MP2)        �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
              WinSetWindowPtr(hwnd, QWL_USER, pData = (POW_DATA)lParam2);

              pData->fUseData = BOOL_FALSE;       /* start a pessemist...     */

              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� Setup the Dialog for processing...                         �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
              CenterDialog(hwnd);                              /* Center DLG  */

              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� Set default "Refresh" control checkboxes                   �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� Load listbox with current set of windows                   �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
              SetListboxFont(WinWindowFromID(hwnd, ID_OW_LB_WINDOWS),/* set LB font */
                             (PSZ) pData->pfnCurrent);

              GenerateList(hwnd,
                           ID_OW_LB_WINDOWS,
                           ID_OW_ST_WINDOWS,
                           pData);            /* Dialog's data           */
          break;

         /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
         /*� We don't need to handle any other messages...                   �*/
         /*�                                                                 �*/
         /*� If this isn't an IPF message, let PM do it's default "thing"    �*/
         /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
         default:
              if ( !HandleIPF(hwnd, message, lParam1, lParam2, &dpResult) )
                dpResult = WinDefDlgProc(hwnd, message, lParam1, lParam2 );

      }                                                 /* End switch                   */

   /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
   /*� Exit                                                                  �*/
   /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
   return( dpResult );
}
