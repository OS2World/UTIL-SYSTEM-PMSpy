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
/*� PMSPYGRP.C                                                               �*/
/*�                                                                          �*/
/*� "Group Message Filter" Dialog                                            �*/
/*����������������������������������������������������������������������������*/

#include "pmspy.h"

/*��������������������������������������������������������������������������Ŀ*/
/*� FormatGRP                                                                �*/
/*�                                                                          �*/
/*� - common subroutine to format a list item for a specific GRP             �*/
/*����������������������������������������������������������������������������*/
static
PSZ
FormatGRP(PGRP_ITEM   pGrp,
          PSZ         pszFormatArea)
{
  sprintf(pszFormatArea,
          pGrp->Include ? Strings[IDS_FMT_GRP_INCLUDED]
                        : Strings[IDS_FMT_GRP_EXCLUDED],
          pGrp->pDesc       /* Group's mnemonic */
         );

  return(pszFormatArea);
}

/*��������������������������������������������������������������������������Ŀ*/
/*� Group Message Filter Dialog Procedure                                    �*/
/*����������������������������������������������������������������������������*/
 MRESULT GroupDlgProc(          HWND    hwnd,
                       register ULONG   message,
                                MPARAM  lParam1,
                                MPARAM  lParam2 )
  {
     /*���������������������������������������������������������������������Ŀ*/
     /*� Local Variables                                                     �*/
     /*�����������������������������������������������������������������������*/
     MRESULT            dpResult = NULL;

     CHAR               ListText[200];

              PGRP_ITEM pGrp;

     register USHORT    usItem,
                        pbSelected;

     static PSPY_DATA   pSpyData = NULL;

     HWND               hwndLB;                /* HWND of group listbox */

  /*������������������������������������������������������������������������Ŀ*/
  /*� Process the message                                                    �*/
  /*��������������������������������������������������������������������������*/
  switch (message)
  {
    /*����������������������������������������������������������������������Ŀ*/
    /*� A dialog pushbutton was selected                                     �*/
    /*������������������������������������������������������������������������*/
    case WM_COMMAND:

      switch( pbSelected = SHORT1FROMMP(lParam1) )
      {
          /*����������������������������������������������������������������Ŀ*/
          /*� Handle all attribute pushbuttons                               �*/
          /*������������������������������������������������������������������*/
          default:
          {
            register MSG_COLOR color = GetColor(hwnd, pSpyData);

            USHORT             nSelected;  /* Number of items selected */

            /*����������������������������������������������������������Ŀ*/
            /*� Obtain HWND of group listbox once for performance        �*/
            /*������������������������������������������������������������*/
            hwndLB = WinWindowFromID(hwnd, ID_GRP_LB_GROUPS);

            /************************************************************
            * "Lock" the ListBox to minimize performance impact
            ************************************************************/
            WinLockWindowUpdate(HWND_DESKTOP,     /* Desktop */
                                hwndLB);          /* Lockee  */

            /************************************************************
            * Process the "selected" ListBox items
            ************************************************************/
            for(nSelected = 0,
                usItem    = SHORT1FROMMR(WinSendMsg(hwndLB,
                                                    LM_QUERYSELECTION,
                                                    MPFROMSHORT(LIT_FIRST),
                                                    NULL) );

                usItem != (USHORT)LIT_NONE;

                nSelected++,
                usItem    = SHORT1FROMMR(WinSendMsg(hwndLB,
                                                    LM_QUERYSELECTION,
                                                    MPFROMSHORT(usItem),
                                                    NULL) )
               )
            {
              pGrp = PVOIDFROMMR( WinSendMsg(hwndLB,
                                             LM_QUERYITEMHANDLE,
                                             MPFROMSHORT(usItem),
                                             NULL) );

              /*������������������������������������������������������������Ŀ*/
              /*� Perform selected pushbutton's action                       �*/
              /*��������������������������������������������������������������*/
              switch( pbSelected )
              {
                /*����������������������������������������������������������Ŀ*/
                /*� Delete this group                                        �*/
                /*������������������������������������������������������������*/
                case ID_GRP_PB_DELETE:

                     DeleteGroup(pSpyData, pGrp->GroupID);

                     WinSendMsg(hwndLB,
                                LM_DELETEITEM,
                                MPFROMSHORT(usItem),
                                MPFROMP(NULL) );

                     usItem = (USHORT)( (usItem == 0)
                                          ? LIT_FIRST
                                          : (usItem - 1) );
                break;

                /*����������������������������������������������������������Ŀ*/
                /*� Exclude this group                                       �*/
                /*������������������������������������������������������������*/
                case ID_GRP_PB_EXCLUDE:

                     IncludeGroup(pSpyData, pGrp, BOOL_FALSE, color);

                     WinSendMsg(hwndLB,
                                LM_SETITEMTEXT,
                                MPFROMSHORT(usItem),
                                MPFROMP(FormatGRP(pGrp, ListText)) );
                break;

                /*����������������������������������������������������������Ŀ*/
                /*� Include this group                                       �*/
                /*������������������������������������������������������������*/
                case ID_GRP_PB_INCLUDE:

                     IncludeGroup(pSpyData, pGrp, BOOL_TRUE, color);

                     WinSendMsg(hwndLB,
                                LM_SETITEMTEXT,
                                MPFROMSHORT(usItem),
                                MPFROMP(FormatGRP(pGrp, ListText)) );
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
            if (nSelected == 0)                  /* Any selections? */
              WinAlarm(HWND_DESKTOP, WA_ERROR);
          }
          break;

          /*����������������������������������������������������������������Ŀ*/
          /*� User's done with the dialog...                                 �*/
          /*������������������������������������������������������������������*/
          case DID_CANCEL:

               WinDismissDlg( hwnd, 0 );

          break;
      }
    break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� WM_CLOSE:                                                            �*/
    /*� - Call WinDismissDlg to close the dialog box                         �*/
    /*������������������������������������������������������������������������*/
    case WM_CLOSE:

         WinDismissDlg( hwnd, 0);

    break;

    /*����������������������������������������������������������������������Ŀ*/
    /*� WM_INITDLG:                                                          �*/
    /*� - Init pointer to data pointed to by WinDlgBox                       �*/
    /*� - Init global handle of this dlg                                     �*/
    /*������������������������������������������������������������������������*/
    case WM_INITDLG:

      /*��������������������������������������������������������������������Ŀ*/
      /*� Locate "SPY" data to utilize                                       �*/
      /*����������������������������������������������������������������������*/
      pSpyData = PVOIDFROMMP(lParam2);

      /*����������������������������������������������������������Ŀ*/
      /*� Obtain HWND of group listbox once for performance        �*/
      /*������������������������������������������������������������*/
      hwndLB = WinWindowFromID(hwnd, ID_GRP_LB_GROUPS);

      /************************************************************
      * "Lock" the ListBox to minimize performance impact
      ************************************************************/
      WinLockWindowUpdate(HWND_DESKTOP,     /* Desktop */
                          hwndLB);          /* Lockee  */

      /*���������������������������������������������������������������������Ŀ*/
      /*� Set listbox FONT to current non-proportional value                  �*/
      /*�����������������������������������������������������������������������*/
      SetListboxFont(hwndLB, (PSZ) pSpyData->pfnLB);

      /*��������������������������������������������������������������������Ŀ*/
      /*� Add all Groups to list box                                         �*/
      /*����������������������������������������������������������������������*/
      for(/* Initialize */ pGrp = ProcessFirstGroup(pSpyData); /* @ 1st GROUP */
          /* Terminate  */ pGrp != NULL;                       /* @ end of list */
          /* Iterate    */ pGrp = ProcessNextGroup(pSpyData)   /* try next GROUP */
         )
      {
        usItem = SHORT1FROMMR(WinSendMsg(hwndLB,
                                         LM_INSERTITEM,
                                         MPFROMSHORT(LIT_SORTASCENDING),
                                         MPFROMP(FormatGRP(pGrp, ListText))) );

        WinSendMsg(hwndLB,
                   LM_SETITEMHANDLE,
                   MPFROMSHORT(usItem),
                   MPFROMP(pGrp));
      }

      /************************************************************
      * Finally, "unlock" the ListBox so it is only redrawn once
      ************************************************************/
      WinLockWindowUpdate(HWND_DESKTOP, NULLHANDLE);

      /************************************************************
      * Other dialog set-up
      ************************************************************/
      SetColors( hwnd, pSpyData );      /* load colors       */

      CenterDialog(hwnd);               /* Center the dialog */

    break;                                          /* WM_INITDLG */

    /*����������������������������������������������������������������������Ŀ*/
    /*� This is a MSG we don't care about...                                 �*/
    /*������������������������������������������������������������������������*/
    default:
         if ( !HandleIPF(hwnd, message, lParam1, lParam2, &dpResult) )
           dpResult = WinDefDlgProc(hwnd, message, lParam1, lParam2);
  }

  /*������������������������������������������������������������������������Ŀ*/
  /*� Exit                                                                   �*/
  /*��������������������������������������������������������������������������*/
  return( dpResult );
}
