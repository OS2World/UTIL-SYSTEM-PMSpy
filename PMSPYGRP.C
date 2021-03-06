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
/*� PMSPYGRP.C                                                               �*/
/*�                                                                          �*/
/*� "Group Message Filter" Dialog                                            �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

#include "pmspy.h"

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� FormatGRP                                                                �*/
/*�                                                                          �*/
/*� - common subroutine to format a list item for a specific GRP             �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Group Message Filter Dialog Procedure                                    �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
 MRESULT GroupDlgProc(          HWND    hwnd,
                       register ULONG   message,
                                MPARAM  lParam1,
                                MPARAM  lParam2 )
  {
     /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     /*� Local Variables                                                     �*/
     /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     MRESULT            dpResult = NULL;

     CHAR               ListText[200];

              PGRP_ITEM pGrp;

     register USHORT    usItem,
                        pbSelected;

     static PSPY_DATA   pSpyData = NULL;

     HWND               hwndLB;                /* HWND of group listbox */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Process the message                                                    �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  switch (message)
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� A dialog pushbutton was selected                                     �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_COMMAND:

      switch( pbSelected = SHORT1FROMMP(lParam1) )
      {
          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� Handle all attribute pushbuttons                               �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          default:
          {
            register MSG_COLOR color = GetColor(hwnd, pSpyData);

            USHORT             nSelected;  /* Number of items selected */

            /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
            /*� Obtain HWND of group listbox once for performance        �*/
            /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

              /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
              /*� Perform selected pushbutton's action                       �*/
              /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
              switch( pbSelected )
              {
                /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
                /*� Delete this group                                        �*/
                /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

                /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
                /*� Exclude this group                                       �*/
                /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
                case ID_GRP_PB_EXCLUDE:

                     IncludeGroup(pSpyData, pGrp, BOOL_FALSE, color);

                     WinSendMsg(hwndLB,
                                LM_SETITEMTEXT,
                                MPFROMSHORT(usItem),
                                MPFROMP(FormatGRP(pGrp, ListText)) );
                break;

                /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
                /*� Include this group                                       �*/
                /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

          /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
          /*� User's done with the dialog...                                 �*/
          /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
          case DID_CANCEL:

               WinDismissDlg( hwnd, 0 );

          break;
      }
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� WM_CLOSE:                                                            �*/
    /*� - Call WinDismissDlg to close the dialog box                         �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_CLOSE:

         WinDismissDlg( hwnd, 0);

    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� WM_INITDLG:                                                          �*/
    /*� - Init pointer to data pointed to by WinDlgBox                       �*/
    /*� - Init global handle of this dlg                                     �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_INITDLG:

      /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
      /*� Locate "SPY" data to utilize                                       �*/
      /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
      pSpyData = PVOIDFROMMP(lParam2);

      /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
      /*� Obtain HWND of group listbox once for performance        �*/
      /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
      hwndLB = WinWindowFromID(hwnd, ID_GRP_LB_GROUPS);

      /************************************************************
      * "Lock" the ListBox to minimize performance impact
      ************************************************************/
      WinLockWindowUpdate(HWND_DESKTOP,     /* Desktop */
                          hwndLB);          /* Lockee  */

      /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
      /*� Set listbox FONT to current non-proportional value                  �*/
      /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
      SetListboxFont(hwndLB, (PSZ) pSpyData->pfnLB);

      /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
      /*� Add all Groups to list box                                         �*/
      /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
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

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� This is a MSG we don't care about...                                 �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    default:
         if ( !HandleIPF(hwnd, message, lParam1, lParam2, &dpResult) )
           dpResult = WinDefDlgProc(hwnd, message, lParam1, lParam2);
  }

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Exit                                                                   �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  return( dpResult );
}
