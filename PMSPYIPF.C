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
/*� PMSPYIPF.C                                                               �*/
/*�                                                                          �*/
/*� IPF interface code                                                       �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

#define INCL_WINMESSAGEMGR

#include "pmspy.h"
#include "pmspyipf.h"


/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Shared, local data used by IPF routines                                  �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

static HWND hFrame = NULLH,
            hIPF   = NULLH;


/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� InitializeIPF                                                            �*/
/*�                                                                          �*/
/*� Returns: NULL........unable to create IPF instance                       �*/
/*�          Otherwise...IPF help handle                                     �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

HWND InitializeIPF( HWND   hwndFrame,
                    USHORT uActionBarID,
                    PSZ    pszHelpLibs,
                    PSZ    pszRefTitle)
{
  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Define Automatic variables                                             �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  HELPINIT HM_Struct;                    /* HM Initialization Structure */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Reset to "No IPF now..."                                               �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  hIPF = hFrame = NULLH;

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� set up HelpManager intialization structure                             �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  HM_Struct.cb                        = sizeof(HM_Struct);
  HM_Struct.ulReturnCode              = 0L;
  HM_Struct.pszTutorialName           = NULL;
  HM_Struct.phtHelpTable              = MPFROM2SHORT(uActionBarID,0xFFFF);
  HM_Struct.hmodAccelActionBarModule  = 0;
  HM_Struct.idAccelTable              = 0;
  HM_Struct.idActionBar               = 0;
  HM_Struct.pszHelpWindowTitle        = pszRefTitle;
  HM_Struct.fShowPanelId              = CMIC_HIDE_PANEL_ID;  /* CMIC_SHOW_PANEL_ID */
  HM_Struct.pszHelpLibraryName        = pszHelpLibs;
  HM_Struct.hmodHelpTableModule       = hmodNonNLS;

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Create an instance of the HelpManager                                  �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  if ( (hIPF = WinCreateHelpInstance(WinQueryAnchorBlock(hwndFrame),
                                     &HM_Struct)) == NULLH)
  {
    MsgDisplay( hwndFrame,
                swcntrl.szSwtitle,
                Strings[IDS_MSG_IPF_INIT_1],
                IDS_MSG_IPF_INIT_1,
                MB_CANCEL | MB_CUACRITICAL,

                hwndFrame,
                uActionBarID,
                HM_Struct.ulReturnCode,
                pszHelpLibs
              );
  }
  else
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Associate IPF instance with specified FRAME window                   �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    if ( WinAssociateHelpInstance(hIPF, hwndFrame) == BOOL_FALSE )
    {
      MsgDisplay( hwndFrame,
                  swcntrl.szSwtitle,
                  Strings[IDS_MSG_IPF_INIT_2],
                  IDS_MSG_IPF_INIT_2,
                  MB_CANCEL | MB_CUACRITICAL,

                  hIPF,
                  WinGetLastError(WinQueryAnchorBlock(hwndFrame))
                );

      WinDestroyHelpInstance( hIPF );

      hIPF = NULLH;
    }
    else
    {
      hFrame = hwndFrame;
    }
  }

  return ( hIPF );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� HandleIPF - handle any required IPF messages/actions                     �*/
/*�                                                                          �*/
/*� Returns: TRUE........the message HAS     been processed; application     �*/
/*�                      should just return "set" MRESULT value              �*/
/*�                                                                          �*/
/*�          FALSE.......the message HAS NOT been processed; application     �*/
/*�                      should do something with it ("default" for window or�*/
/*�                      dialog)                                             �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
BOOL HandleIPF(HWND     hWnd,         /* current window */
               MSG      Msg,          /* current message */
               MPARAM   mp1,          /* current message parm #1 */
               MPARAM   mp2,          /* current message parm #2 */
               PMRESULT pmrValue)     /* where to "set" result */

{
  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Define automatic variables                                             �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  USHORT usPanelID  = 0;         /* panel ID to display (default: NONE) */

  BOOL   bHandled   = BOOL_TRUE; /* final "result" (default: we DID handle) */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Initially, set to "no special result"                                  �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  *pmrValue = NULL;

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Processing is based on the "msg"...                                    �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  switch ( Msg )
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Some sort of IPF error...                                            �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case HM_ERROR:
    case HM_EXT_HELP_UNDEFINED:

         MsgDisplay( hFrame,
                     swcntrl.szSwtitle,
                     Strings[IDS_MSG_IPF_ERROR_1],
                     IDS_MSG_IPF_ERROR_1,
                     MB_CANCEL | MB_CUACRITICAL,

                     Msg,
                     mp1,
                     mp2,
                     WinGetLastError(WinQueryAnchorBlock(hFrame))
                   );
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Return Panel ID to use for "Keys Help"                               �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case HM_QUERY_KEYS_HELP:

         *pmrValue = MRFROMSHORT(PID_KEYSHELP);
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Check for pulldown selections we're interested in                    �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_COMMAND:

         switch (LOUSHORT(mp1))
         {
           /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
           /*� Display the "Help for Help" panel                             �*/
           /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
           case ID_H_HELP:

                usPanelID = PID_HELPHELP;
           break;

           /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
           /*� Sorry, we just don't care!                                    �*/
           /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

           default:
                bHandled = BOOL_FALSE;          /* we DID NOT handle... */
         }

         /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
         /*� Now check to see if we should display a Panel...                �*/
         /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
         if ( usPanelID != 0 )
           WinSendMsg(hIPF,
                      HM_DISPLAY_HELP,
                      MPFROMSHORT(usPanelID),
                      MPFROMSHORT(HM_RESOURCEID) );    /* i've given a Panel ID */
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� If the "top level" frame window is being destroyed, stop IPF...      �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case WM_DESTROY:
         if (hWnd == hFrame)
         {
           WinDestroyHelpInstance(hIPF);
           hIPF = hFrame = NULLH;
         }

         bHandled = BOOL_FALSE;            /* we DID NOT handle... */

    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Otherwise, not a message we need to consider for IPF...              �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    default:
         bHandled = BOOL_FALSE;            /* we DID NOT handle... */
  }

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Return if this is an HM_xxx message we've handled...                   �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  return( bHandled );
}
