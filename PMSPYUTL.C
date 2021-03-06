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
/*� PMSPYUTL.C                                                               �*/
/*�                                                                          �*/
/*� Utility routines                                                         �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Includes                                                                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

#include "pmspy.h"                      /* Resource symbolic identifiers*/

#include "pmspyMM.h"                    /* MMPM/2 header file           */

#include <ctype.h>

typedef struct {                        /* Memory Allocation control structure */

                 ULONG  nAllocs,
                        sAllocs,
                        nFrees;
               } MEM_DATA, *PMEM_DATA;

static MEM_DATA memStats = { 0, 0, 0};

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    AllocateMemory                                               �*/
/*�                                                                          �*/
/*� Function:   Allocate dynamic memory (also, sets to all NULLs)            �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PVOID AllocateMemory(size_t SizeWanted )

{
  PVOID p;

  /*** Attempt to allocate the requested memory ***/

  if ( (p = calloc( SizeWanted, 1)) == NULL)
  {
    MsgDisplay(HWND_DESKTOP,
               Strings[IDS_TITLE],
               Strings[IDS_MSG_ALLOCATE],
               0,
               MB_CUAWARNING | MB_CANCEL,
               SizeWanted,
               memStats.nAllocs,
               memStats.sAllocs,
               memStats.nFrees);
  }
  else
  {
    /*** Update statistics ***/
    memStats.nAllocs++;
    memStats.sAllocs += SizeWanted;
  }

  return( p );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    FreeMemory                                                   �*/
/*�                                                                          �*/
/*� Function:   Free memory obtained from AllocateMemory                     �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

VOID FreeMemory(register PVOID p )

{
   if (p == NULL)
   {
    MsgDisplay(HWND_DESKTOP,
               Strings[IDS_TITLE],
               Strings[IDS_MSG_FREE],
               0,
               MB_CUAWARNING | MB_CANCEL,
               p,
               memStats.nAllocs,
               memStats.sAllocs,
               memStats.nFrees);
   }
   else
   {
     memStats.nFrees++;
     free( p );
   } /* endif */
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    QueryMemory                                                  �*/
/*�                                                                          �*/
/*� Function:   Query memory statistics                                      �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

VOID QueryMemory(register PMEM_DATA p )

{
  *p = memStats;                /* return COPY to caller */
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    MsgDisplay                                                   �*/
/*�                                                                          �*/
/*� Function:   Display MessageBox with variable substitution                �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
static BOOL bEnableHELP = BOOL_FALSE;          /* add "help" button?          */

ULONG MsgDisplay( HWND   hWnd,
                  PSZ    strCap,
                  PSZ    strFormat,
                  USHORT mb_id,
                  ULONG  style,
                  ...)                         /* strFormat insert data       */
{
  CHAR          MessageText[512];              /* Message box output          */

  va_list       arg_ptr;                       /* Variable argument list ptr  */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Ensure "Help" button present if IPF initialized OK                     �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  if ( bEnableHELP )
    style |= MB_HELP;

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*�  Format the specified message (insert message variables, etc)          �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  va_start(arg_ptr, style);                     /* Start variable list access */

  vsprintf(MessageText,                         /*  Buffer to put it in       */
           strFormat,                           /*  Format string             */
           arg_ptr);

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*�  Display the formatted message & return user's response                �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  return( WinMessageBox(HWND_DESKTOP,           /*   Desktop handle           */
                        hWnd,                   /*   Client window handle     */
                        MessageText,            /*   message text             */
                        strCap,                 /*   Caption of Box           */
                        mb_id,                  /*   ID of message box (Help) */
                        style | MB_MOVEABLE) ); /*   Style of message box     */
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    MsgDisplay                                                   �*/
/*�                                                                          �*/
/*� Function:   Display MessageBox with variable substitution                �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID MsgDisplayShouldEnableHELP( VOID )

{
  bEnableHELP = BOOL_TRUE;

}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: ConvertToNumber                                                 �*/
/*� ------------------------------------------------------------------------ �*/
/*�                                                                          �*/
/*� Purpose:   Convert an Ascii text string to its binary value using        �*/
/*�            C language format conventions                                 �*/
/*�                                                                          �*/
/*�            a) decimal          dddd                                      �*/
/*�            b) octal           0dddd                                      �*/
/*�            c) hexidecimal    0xdddd                                      �*/
/*�                                                                          �*/
/*�            Complete Format: [<white space>]<number>                      �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� Prototype: BOOL ConvertToNumber(PSZ   pAscii,                            �*/
/*�                                 CTN  *pNumber,                           �*/
/*�                                 CTN   lMin,                              �*/
/*�                                 CTN   lMax);                             �*/
/*�                                                                          �*/
/*� Entry:     pAscii.....pointer to NULL terminated string to attempt to    �*/
/*�                       convert to binary                                  �*/
/*�            pNumeric...pointer to variable to receive converted value     �*/
/*�            lMim.......minimum acceptable range value (inclusive)         �*/
/*�            lMax.......maximum acceptable range value (inclusive)         �*/
/*�                                                                          �*/
/*� Exit:      TRUE.......conversion was successful                          �*/
/*�                       ==> *pNumber contains converted value              �*/
/*�                                                                          �*/
/*�            FALSE......conversion was not successful                      �*/
/*�                       ==> *pNumber is indeterminate                      �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

BOOL ConvertToNumber(PSZ     pAscii,
                     PUSHORT pNumber,
                     USHORT  lMin,
                     USHORT  lMax)

{
  register BOOL  fOK = BOOL_FALSE;            /* start as a pessimist! */
           PSZ   pEnd;                        /* locates last char converted */

           ULONG ulConversion = strtoul(pAscii,
                                        &pEnd,
                                        0);     /* figure base out */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Determine if conversion was successful                                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

  if (*pEnd == 0)
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Conversion was successful, is number within allowed range?           �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    *pNumber = LOUSHORT(ulConversion); /* convert from 32 to 16 bits */

    if ( (*pNumber >= lMin) && (*pNumber <= lMax) )
      fOK = BOOL_TRUE;
  }

  return( fOK );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: OpenPDTR                                                        �*/
/*� -------------------------------------------------------------------------�*/
/*�                                                                          �*/
/*� Purpose:   Open a Programmer Defined Text Resource (PDTR) for later      �*/
/*�                                                                  READing �*/
/*� Prototype: PPDTR_DATA openPDTR(PPDTR_DATA pData)                         �*/
/*�                                                                          �*/
/*� Entry:     pData......pointer to PDTR control structure                  �*/
/*�                                                                          �*/
/*� Exit:      !NULL......Open was successful                                �*/
/*�                       ==> ready to Read/Eof/Close                        �*/
/*�                                                                          �*/
/*�             NULL......Open failed                                        �*/
/*�                       ==> probable causes: - invalid hModule, typePDTR,  �*/
/*�                                              or idPDTR value specified   �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PPDTR_DATA OpenPDTR(register PPDTR_DATA pData)

{
  register BOOL bOK = BOOL_FALSE;
  PVOID    pVoid;

  if (DosGetResource(pData->hModule,          /* Who to get from    */
                     pData->typePDTR,         /* What Resource Type */
                     pData->idPDTR,           /* What Resource ID   */
                     &pVoid) == 0)            /* Where PTR placed   */
  {
    pData->pTOF    =
    pData->pRead   =
    pData->offPDTR = pVoid;

    bOK = BOOL_TRUE;
  }

  return(bOK ? pData : NULL);
}

#define PDTR_EOF 0x1A                  /* End-Of-File byte */
#define PDTR_EOL 0xD                   /* End-Of-Line byte */

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: EofPDTR                                                         �*/
/*� --------------------------------------------------------------------     �*/
/*�                                                                          �*/
/*� Purpose:   Determine if Open Programmer Defined Text Resource (PDTR)     �*/
/*�            is at End-Of-File                                             �*/
/*�                                                                          �*/
/*� Prototype: BOOL EofPDTR(PPDTR_DATA pData)                                �*/
/*�                                                                          �*/
/*� Entry:     pData......pointer to PDTR control structure                  �*/
/*�                                                                          �*/
/*� Exit:      TRUE.......File is at EOF                                     �*/
/*�                                                                          �*/
/*�            FALSE......File is not at EOF                                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

BOOL EofPDTR(register PPDTR_DATA pData)

{
  return( *pData->pRead == PDTR_EOF ? BOOL_TRUE : BOOL_FALSE );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: GetsPDTR                                                        �*/
/*� -------------------------------------------------------------------------�*/
/*�                                                                          �*/
/*� Purpose:   Get String from an Open Programmer Defined Text Resource(PDTR)�*/
/*�                                                                          �*/
/*� Prototype: PSZ GetsPDTR(PPDTR_DATA pData, PSZ pString, USHORT lString)   �*/
/*�                                                                          �*/
/*� Entry:     pData......pointer to PDTR control structure                  �*/
/*�            pString....pointer to area to put string                      �*/
/*�            lString....length of pString area (must be ONE or more!)      �*/
/*�                                                                          �*/
/*� Exit:      NULL.......File is at EOF                                     �*/
/*�           !NULL.......entry pString value                                �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� PDTR format: <text....><CR><LF>                                          �*/
/*�              <text....><CR><LF>                                          �*/
/*�              <text....><CR><LF>                                          �*/
/*�              <EOF>                                                       �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

PSZ GetsPDTR(         PPDTR_DATA pData,
                      PSZ        pString,
             register USHORT     lString)

{
  register PSZ pWork;                           /* work in pString area */

  /* if at EOF, quit right away */

  if ( *pData->pRead == PDTR_EOF )
    return( NULL );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Copy line from SEGMENT until: 1) EOL reached                           �*/
  /*�                               2) pString area filled                   �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  for(/* Init */ pWork = pString;               /* where to put string */

      /* Term */ (*pData->pRead != PDTR_EOL) && /* quit @ EOL */
                 (lString       != 1);          /* or no room left */

      /* Iter */ pWork++,                       /* move to next string place */
                 pData->pRead++,                /* move READ pointer */
                 lString--                      /* adjust remaining size */
     )
    *pWork = *pData->pRead;

  pData->pRead += 2;   /* skip CR/LF */

  *pWork = 0;       /* make NULL terminated.... */

  return( pString );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: ClosePDTR                                                       �*/
/*� ----------------------------------------------------------------         �*/
/*�                                                                          �*/
/*� Purpose:   Close an Open Programmer Defined Text Resource (PDTR)         �*/
/*�                                                                          �*/
/*� Prototype: BOOL ClosePDTR(PPDTR_DATA pData)                              �*/
/*�                                                                          �*/
/*� Entry:     pData......pointer to PDTR control structure                  �*/
/*�                                                                          �*/
/*� Exit:      0..........PDTR close successful                              �*/
/*�                                                                          �*/
/*�            EOF........PDTR close unsuccessful                            �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

int ClosePDTR(register PPDTR_DATA pData)

{
  return (DosFreeResource (pData->offPDTR) == 0 ? 0 : EOF);
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: FindWordInList                                                  �*/
/*� ----------------------------------------------------------------------   �*/
/*�                                                                          �*/
/*� Purpose:   Find a NULL terminated "word" in a "word list"                �*/
/*�                                                                          �*/
/*� Prototype: USHORT FindWordInList(PSZ pWord, PWORD_LIST pWordList);       �*/
/*�                                                                          �*/
/*� Entry:     pWord......pointer to WORD to look for in pWordList           �*/
/*�            pWordList..pointer to WORD list                               �*/
/*�                                                                          �*/
/*� Exit:      'value' for "word" (or value from End-Of-List marker saying   �*/
/*�            that the "word" was not found                                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

USHORT FindWordInList(PSZ pWord, PWORD_LIST pWordList)

{
  for( /* Init  */   /**** N O N E ****/;

       /* While */   (pWordList->ppWord != NULL) &&
                     (stricmp(pWord, *pWordList->ppWord) != 0);

       /* Iter  */   pWordList++ );

  return(pWordList->vWord );

}


/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: UpdateSpyTitle                                                  �*/
/*� --------------------------------------------------------                 �*/
/*�                                                                          �*/
/*� Purpose:   Update the title of the PMSPY frame                           �*/
/*�                                                                          �*/
/*� Prototype: VOID UpdateSpyTitle( HWND         hwndFrame,                  �*/
/*�                                 SPY_TITLES   whichTitle,                 �*/
/*�                                 ...);                                    �*/
/*�                                                                          �*/
/*� Entry:     hwndFrame....frame window                                     �*/
/*�            whichTitle...desired title mode                               �*/
/*�                                                                          �*/
/*� Exit:      NONE, except title in frame updated                           �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

VOID UpdateSpyTitle(HWND         hwndFrame,
                    SPY_TITLES   whichTitle,
                    ...)
{
  CHAR          szText[256];

  va_list       arg_ptr;                /* Variable argument list ptr */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Locate start of variable parm(s) passed to us                          �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  va_start(arg_ptr, whichTitle);        /* Start variable list access */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Generate the new PMSPY title                                           �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  switch( whichTitle )
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Display information about the "window" the POINTER is currently over �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case tSpyWho:
         vsprintf(szText, Strings[IDS_FMT_PTR_QUEUE], arg_ptr);
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Change title to display "window" being SPYed                         �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case tWindow:
         vsprintf(szText, Strings[IDS_FMT_SPY_WINDOW], arg_ptr);
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Change title to display "object" being SPYed                         �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case tObject:
         vsprintf(szText, Strings[IDS_FMT_SPY_OBJECT], arg_ptr);
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Change title to display application "queue" being SPYed              �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case tQueue:
         vsprintf(szText, Strings[IDS_FMT_SPY_QUEUE], arg_ptr);
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Change title to display that SYSTEM "queue" being SPYed              �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    case tQueueSystem:
         vsprintf(szText, Strings[IDS_FMT_SPY_QUEUE_SYSTEM], arg_ptr);
    break;

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Change title back to PMSPY standard                                  �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    default:
         strcpy(szText, swcntrl.szSwtitle);
  }

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Update the PMSPY title                                                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  WinSetWindowText(hwndFrame, szText);

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� LOG the new PMSPY title, if we need to                                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  if ( bCurrentlyLogging )
    fprintf(pLogFile, "%s\n", szText);
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine: TranslateClassName                                              �*/
/*� ------------------------------------------------------------------------ �*/
/*�                                                                          �*/
/*� Purpose:  Translate WinQueryClassName to usable WC_* string              �*/
/*�                                                                          �*/
/*� Prototype: PSZ TranslateClassName(PSZ pszClassName)                      �*/
/*�                                                                          �*/
/*� Entry:     pszClassName....name from WinQueryClassName                   �*/
/*�                                                                          �*/
/*� Returns:   if a standard WC_* class, then pointer to usable string       �*/
/*�                                      else pointer to original class name �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

PSZ TranslateClassName(PSZ pszClassName)

{

           PSZ    pszTranslate;        /* final, translated class name */
  register USHORT i;

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Translate table of WinClassName value to WC_* strings                    �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

static struct
{
  PSZ    pszClassName,            /* binary WC_xxxx value                     */
         pszWC;                   /* human usable text equivalent             */

  CHAR   szClassName[8];          /* "#nnn" version of binary WC_xxxx value   */

} Class2WC[] =

{
  {WC_BUTTON            ,"WC_BUTTON"             },

#ifdef WC_CIRCULARSLIDER
  {WC_CIRCULARSLIDER    ,"WC_CIRCULARSLIDER"     },      /* MMPM/2 control */
#endif

  {WC_COMBOBOX          ,"WC_COMBOBOX"           },
  {WC_CONTAINER         ,"WC_CONTAINER"          },
  {WC_ENTRYFIELD        ,"WC_ENTRYFIELD"         },
  {WC_FRAME             ,"WC_FRAME"              },

#ifdef WC_GRAPHICBUTTON
  {WC_GRAPHICBUTTON     ,"WC_GRAPHICBUTTON"      },      /* MMPM/2 control */
#endif

  {WC_LISTBOX           ,"WC_LISTBOX"            },
  {WC_MENU              ,"WC_MENU"               },
  {WC_MLE               ,"WC_MLE"                },
  {WC_NOTEBOOK          ,"WC_NOTEBOOK"           },
  {WC_SCROLLBAR         ,"WC_SCROLLBAR"          },
  {WC_SLIDER            ,"WC_SLIDER"             },
  {WC_SPINBUTTON        ,"WC_SPINBUTTON"         },
  {WC_STATIC            ,"WC_STATIC"             },
  {WC_TITLEBAR          ,"WC_TITLEBAR"           },
  {WC_VALUESET          ,"WC_VALUESET"           },

  {WC_APPSTAT           ,"WC_APPSTAT"            },
  {WC_DBE_KKPOPUP       ,"WC_DBE_KKPOPUP"        },
  {WC_KBDSTAT           ,"WC_KBDSTAT"            },
  {WC_PECIC             ,"WC_PECIC"              },

  {NULL                 ,NULL                    }   /* End-Of-Table marker   */
};

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Need to perform one-time generation of "#nnn" strings?                 �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  if ( Class2WC[0].szClassName[0] == 0 )
  {
    for(/* Initialize */  i = 0;
        /* While      */  Class2WC[i].pszClassName != NULL;
        /* Iterate    */  i++)
    {
       /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
       /*� Generate class name in "#nnn" format                              �*/
       /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
       sprintf(Class2WC[i].szClassName,
               "#%d",
               LOUSHORT(Class2WC[i].pszClassName) );
    }
  }

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Translate all WC_* class names to something human understandable       �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  for(/* Initialize */ pszTranslate =  NULL,
                       i            = 0;
      /* While      */ pszTranslate == NULL;
      /* Iterate    */ i++)
  {
     /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     /*� At end of translate table?  if so, use class name ASIS              �*/
     /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     if (Class2WC[i].pszClassName == NULL)
       pszTranslate = pszClassName;
     /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     /*� Is this a WC_* class?  if so, use translation                       �*/
     /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
     else if (strcmp(pszClassName, Class2WC[i].szClassName) == 0)
       pszTranslate = Class2WC[i].pszWC;
  }

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Return the translation result                                          �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  return( pszTranslate );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Module Level Design                                                      �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� Function:                                                                �*/
/*�     To center a dialog in the middle of the DESKTOP area                 �*/
/*�                                                                          �*/
/*� Input:                                                                   �*/
/*�     - Dialog's Window Handle                                             �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� Output:                                                                  �*/
/*�     - Returns:                                                           �*/
/*�       a) TRUE........ no error detected (dialog nicely centered!)        �*/
/*�       b) FALSE....... error detected (logged to RAS error log)           �*/
/*�                                                                          �*/
/*� Notes:                                                                   �*/
/*�     -                                                                    �*/
/*�                                                                          �*/
/*� Logic:                                                                   �*/
/*�     - get size of DESKTOP area (results are 'window' coordinates)        �*/
/*�     - convert DESKTOP area to 'screen' coordinates                       �*/
/*�     - get size of Dialog (results are 'window' coordinates)              �*/
/*�     - convert Dialog area to 'screen' coordinates                        �*/
/*�     - use WinSetPos to position the Dialog centered on the DESKTOP       �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

BOOL CenterDialog(HWND hwndDialog)     /* Dialog window to center */

{
  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Local Variables                                                        �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  BOOL     fOK      = BOOL_FALSE;    /* start as a pessimist! */

  RECTL    rectlDesktop,             /* Rectl of DeskTop */
           rectlDialog;              /* Rectl of dialog */

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Note: using DO_ONCE_BLOCK for easy error recovery                      �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  BEGIN_DO_ONCE_BLOCK

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Gather information on the DESKTOP                                    �*/
    /*�                                                                      �*/
    /*� - get DESKTOP rectangle (in window coordinates)                      �*/
    /*� - convert to screen coordinates                                      �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    if ( !WinQueryWindowRect(HWND_DESKTOP, &rectlDesktop) ||

         !WinMapWindowPoints(hwndDialog,              /* From: Window coordinates */
                             HWND_DESKTOP,            /* To:   Desktop coordinates */
                             (PPOINTL)&rectlDesktop,  /* address(points) */
                             2)  ||                   /* number of points */

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Gather information on the dialog                                     �*/
    /*�                                                                      �*/
    /*� - get DESKTOP rectangle                                              �*/
    /*� - convert to window coordinates                                      �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
         !WinQueryWindowRect(hwndDialog, &rectlDialog) ||

         !WinMapWindowPoints(hwndDialog,              /* From: Window coordinates */
                             HWND_DESKTOP,            /* To:   Desktop coordinates */
                             (PPOINTL)&rectlDialog,   /* address(points) */
                             2) )
    {
      EXIT_DO_ONCE_BLOCK;
    }

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Position Dialog in center of Desk Top                                �*/
    /*�                                                                      �*/
    /*� Centered X position = (Width (DeskTop) - Width (Dialog) ) / 2        �*/
    /*� Centered Y position = (Height(DeskTop) - Height(Dialog) ) / 2        �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    if ( WinSetWindowPos(hwndDialog,
                         HWND_TOP,                       /* Z_ORDER */
                         (SHORT)(                        /* X position */
                                 ((rectlDesktop.xRight - /*  Desktop height */
                                   rectlDesktop.xLeft) -
                                  (rectlDialog .xRight - /*  Dialog  height */
                                   rectlDialog .xLeft))/2
                                ),

                         (SHORT)(                        /* Y position */
                                 ((rectlDesktop.yTop   - /* Desktop width */
                                   rectlDesktop.yBottom) -
                                  (rectlDialog .yTop   - /* Dialog  width */
                                   rectlDialog .yBottom))/2
                                ),

                         (SHORT)NULL,
                         (SHORT)NULL,
                         SWP_MOVE | SWP_ZORDER) )        /* Flags */

       fOK = BOOL_TRUE;         /* we're a success! */
    else
    {
      EXIT_DO_ONCE_BLOCK;
    }

  END_DO_ONCE_BLOCK;

 /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
 /*� Exit                                                                    �*/
 /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
 return( fOK );
}

/*---------------------------------------------------------------------------
* Set new listbox FONT
*
* Input:   hwndLB.....HWND of LISTBOX
*          pszFont....new FONT
*
* OutPut:  NONE
+---------------------------------------------------------------------------*/
VOID SetListboxFont(HWND hwndLB,
                    PSZ  pszFont)
{
   /*****************************************************************
   * Local data
   ******************************************************************/
   HPS               hPS;
   FONTMETRICS       Metrics;

   /*****************************************************************
   * Set new LISTBOX font
   ******************************************************************/
   WinSetPresParam(hwndLB,
                   PP_FONTNAMESIZE,
                   (ULONG) strlen(pszFont) + 1,     /* must include NULL! */
                                  pszFont);

   /*****************************************************************
   * Get "metrics" of the new LISTBOX font
   ******************************************************************/
   hPS = WinGetPS(hwndLB);

   GpiQueryFontMetrics(hPS, (LONG)sizeof(FONTMETRICS), &Metrics );

   WinReleasePS(hPS);

   /*****************************************************************
   * Set new LISTBOX height so no inter-row "gap"
   ******************************************************************/
   WinSendMsg(hwndLB,
              LM_SETITEMHEIGHT,
              MPFROMSHORT( LOUSHORT(Metrics.lMaxBaselineExt) ),
              MPFROMP(NULL) );
}
