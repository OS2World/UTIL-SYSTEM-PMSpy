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
/*� PMSPYMSG.C                                                               �*/
/*�                                                                          �*/
/*� MSG and GROUP Data structure support routines                            �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� MSG Data Structure Requirements:                                         �*/
/*� --------------------------------                                         �*/
/*� 1. Fast, Random access by MSG number                                     �*/
/*� 2. Sequential access required (BUT not performance critical)             �*/
/*� 3. Dynamic Additions and Deletions                                       �*/
/*�                                                                          �*/
/*� MSG Data Structure Implementation:                                       �*/
/*� ----------------------------------                                       �*/
/*�                                                                          �*/
/*� Hash Table      Linked List                                              �*/
/*� (MSG_SLOT)      (MSG_DATA)                                               �*/
/*�                 旼컴컴컴컴컴�    旼컴컴컴컴컴�                           �*/
/*� 旼컴컴컫컴커 旼>�         *컵컴�>�         * �                           �*/
/*� �      � *컵켸  �           �    �           �                           �*/
/*� 쳐컴컴컵컴캑    넬컴컴컴컴커�    넬컴컴컴컴커�                           �*/
/*� �      �   �    납         납    납         납                           �*/
/*� 쳐컴컴컵컴캑    납MSG_ITEM 납    납MSG_ITEM 납                           �*/
/*� �      �   �    납         납    납         납                           �*/
/*� 쳐컴컴컵컴캑    냅컴컴컴컴켸�    냅컴컴컴컴켸�                           �*/
/*� �      �   �    읕컴컴컴컴컴�    읕컴컴컴컴컴�                           �*/
/*� 쳐컴컴컵컴캑                                                             �*/
/*� �      �   �                                                             �*/
/*� 읕컴컴컨컴켸                                                             �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� * MSG number is hashed to index into MSG_SLOT table                      �*/
/*�                                                                          �*/
/*� * MSG_DATA items are stored sorted in ascending order to reduce          �*/
/*�   sequential search time                                                 �*/
/*�                                                                          �*/
/*�                                                                          �*/
/*� GROUP Data Structure Requirements:                                       �*/
/*� ----------------------------------                                       �*/
/*� 1. Sequential access required (BUT not performance critical)             �*/
/*� 2. Dynamic Additions and Deletions                                       �*/
/*�                                                                          �*/
/*� GROUP Data Structure Implementation:                                     �*/
/*� ------------------------------------                                     �*/
/*�                                                                          �*/
/*� List Head       Linked List                                              �*/
/*�                 (GRP_DATA)                                               �*/
/*�                 旼컴컴컴컴컴�    旼컴컴컴컴컴�                           �*/
/*� 旼컴컴컫컴커 旼>�         *컵컴�>�         * �                           �*/
/*� 쿓ead  � *컵켸  �           �  �>�           �                           �*/
/*� 쳐컴컴컵컴캑    넬컴컴컴컴커�  � 넬컴컴컴컴커�                           �*/
/*� 쿟ail  � * �    납         납  � 납         납                           �*/
/*� 읕컴컴컨컫켸    납GRP_ITEM 납  � 납GRP_ITEM 납                           �*/
/*�          �      납         납  � 납         납                           �*/
/*�          �      냅컴컴컴컴켸�  � 냅컴컴컴컴켸�                           �*/
/*�          �      읕컴컴컴컴컴�  � 읕컴컴컴컴컴�                           �*/
/*�          �                     �                                         �*/
/*�          읕컴컴컴컴컴컴컴컴컴컴�                                         �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Includes                                                                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

#include "pmspy.h"                      /* Resource symbolic identifiers*/

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    InitializeMsg                                                �*/
/*�                                                                          �*/
/*� Function:   Initialize passed MSG table for processing                   �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID InitializeMsg(register PSPY_DATA pSpy )

{
  static MSG_SLOT defSlot = { NULL,             /* none in list yet */

                              0,                /* no messages, yet... */

                              0xFFFF,           /* no LOW  message */
                              0                 /* no HIGH message */
                            };

  register USHORT i;

  for( /* Init */ i =                           /* Start @ 1st slot */
                  pSpy->TotalMsgs = 0;

       /* Term */ i < NO_MSG_SLOTS;             /* Stop after all slots processed */

       /* Iter */ i++)                          /* On to the next slot... */

    pSpy->Msgs[i] = defSlot;
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    HashMsg                                                      �*/
/*�                                                                          �*/
/*� Function:   "hash" specified MSG to determine MSG_SLOT entry             �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
#if defined(HashMsgAsSubroutine)

  USHORT HashMsg(register MSG msg )

  {
    return( msg % NO_MSG_SLOTS );
  }

#else

  #define HashMsg(msg) ( msg % NO_MSG_SLOTS )

#endif

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    AddMsg                                                       �*/
/*�                                                                          �*/
/*� Function:   Add specified MSG to database                                �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID AddMsg(PSPY_DATA pSpy,
            PMSG_ITEM pMsg )

{
  PMSG_DATA pLast,
            pCur;

  /* Locate "home" slot for this message */

  PMSG_SLOT pSlot = &(pSpy->Msgs[ HashMsg(pMsg->Msg) ]);

  /* Allocate new slot data instance */

  PMSG_DATA pData = AllocateMemory( sizeof( MSG_DATA) );

  pData->pNext = NULL;
  pData->item  = *pMsg;

  /* Processing is based on number of messages already in list */

  switch(pSlot->uNoMsgs)
  {
    case 0:  /* this is the first message in this here slot... */

             pSlot->pFirst = pData;
             break;

    default: /* there are one or more items in list
             **
             ** decide where this one goes
             */

             for(pCur   = pSlot->pFirst,          /* start @ 1st message */
                 pLast  = (PMSG_DATA)pSlot;       /* fudge as MSG_DATA */

                 /* Terminates within loop below */ ;

                 pLast = pCur,
                 pCur  = pCur->pNext)
             {
               if ( pMsg->Msg < pCur->item.Msg )  /* insert before this message? */
               {
                 pLast->pNext = pData;            /* yes... */
                 pData->pNext = pCur;
                 break;
               }
               else if (pCur->pNext == NULL)      /* no...last in list? */
               {
                 pCur->pNext = pData;
                 break;
               }
             }
  }

  /* Update SLOT information */

  pSlot->uNoMsgs++;

  if ( pMsg->Msg < pSlot->MsgLow )
    pSlot->MsgLow = pMsg->Msg;

  if ( pMsg->Msg > pSlot->MsgHigh )
    pSlot->MsgHigh = pMsg->Msg;

  pSpy->TotalMsgs++;
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    iFindMsg (internal use only)                                 �*/
/*�                                                                          �*/
/*� Function:   locate the MSG_DATA entry for a specific MSG                 �*/
/*�                                                                          �*/
/*� Returns:     NULL...if message WAS NOT found                             �*/
/*�             !NULL...if message WAS     found (locates MSG_DATA info)     �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
static PMSG_DATA iFindMsg(         PSPY_DATA  pSpy,   /* Control data structure */
                          register MSG        Msg,    /* MSG to find */
                                   PMSG_SLOT *pSlot,  /* IFF found, 'slot' message "lives" in */
                                   PMSG_DATA *pPrev)  /* IFF found, MSG_DATA before this MSG */
{
  PMSG_DATA pCur;

  /* Locate "home" slot for this message */

  *pSlot = &(pSpy->Msgs[ HashMsg(Msg) ]);

  /* Don't even scan if MSG is not within this slot's current values */

  if ( (Msg >= (*pSlot)->MsgLow) && (Msg <= (*pSlot)->MsgHigh) )
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Scan sequentially for this message                                   �*/
    /*�                                                                      �*/
    /*� Note: since MSG's are kept in sorted order, we will stop             �*/
    /*�       when this MSG is found to not be possible                      �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
    for(pCur   = (*pSlot)->pFirst,     /* start @ 1st  message */
        *pPrev = NULL;

        (pCur  != NULL) &&             /* quit  @ last message */
        (Msg   >= pCur->item.Msg);     /* or when MSG no longer possible */

        *pPrev = pCur,
        pCur   = pCur->pNext)          /* try next message */
    {
      if ( Msg == pCur->item.Msg )
        return( pCur );
    }
  }

  return( NULL );                               /* MSG was not found */
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    FindMsg                                                      �*/
/*�                                                                          �*/
/*� Function:   locate the MSG_ITEM entry to be used by a specific MSG       �*/
/*�                                                                          �*/
/*� Returns:     NULL...if message WAS NOT found                             �*/
/*�             !NULL...if message WAS     found (locates MSG_ITEM info)     �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PMSG_ITEM FindMsg(         PSPY_DATA pSpy,
                  register MSG       Msg )

{
  PMSG_SLOT pSlot;
  PMSG_DATA pMsg, pPrev;

  pMsg = iFindMsg(pSpy, Msg, &pSlot, &pPrev );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� MSG in our database?                                                   �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  return( pMsg == NULL ? NULL
                       : &(pMsg->item) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    FindMsgName                                                  �*/
/*�                                                                          �*/
/*� Function:   locate the MSG_ITEM entry to be used by a specific MSG name  �*/
/*�                                                                          �*/
/*� Returns:     NULL...if message WAS NOT found                             �*/
/*�             !NULL...if message WAS     found (locates MSG_ITEM info)     �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PMSG_ITEM FindMsgName(PSPY_DATA pSpy,
                      PSZ       pName)

{
  register PMSG_ITEM pMsg;

  for( /* Init */ pMsg  = ProcessFirstMsg(pSpy);   /* Start @ 1st MSG */

       /* Term */ (pMsg != NULL) &&                /* Until all MSGs processed */
                  (strcmp(pMsg->pDesc,pName)!=0); /* or name found */

       /* Iter */ pMsg = ProcessNextMsg(pSpy) );    /* Try next MSG */

  return( pMsg );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    DeleteMsg                                                    �*/
/*�                                                                          �*/
/*� Function:   Delete the specified MSG                                     �*/
/*�                                                                          �*/
/*� Returns:    FALSE...message WAS NOT deleted                              �*/
/*�             TRUE....message WAS     deleted                              �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
BOOL DeleteMsg(         PSPY_DATA pSpy,
               register MSG       Msg )

{
  PMSG_SLOT pSlot;
  PMSG_DATA pMsg, pPrev;

  if ( (pMsg = iFindMsg(pSpy, Msg, &pSlot, &pPrev)) == NULL)
    return( BOOL_FALSE );

  /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
  /*� Delete this MSG from this SLOT                                         �*/
  /*�                                                                        �*/
  /*� 1. Delete from this SLOT's linked list                                 �*/
  /*� 2. Update SLOT statitistics                                            �*/
  /*� 3. Update MSG  statistics                                              �*/
  /*� 4. Free Memory (MSG_DATA, Message Description)                         �*/
  /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
  if (pPrev == NULL)                            /* First in SLOT's list? */
    pSlot->pFirst = pMsg->pNext;
  else
    pPrev->pNext  = pMsg->pNext;

  pSlot->uNoMsgs--;

  FreeMemory(pMsg->item.pDesc);
  FreeMemory(pMsg);

  pSpy->TotalMsgs--;

  return( BOOL_TRUE );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    DeleteAllMsgs                                                �*/
/*�                                                                          �*/
/*� Function:   Delete ALL defined MSGs                                      �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID DeleteAllMsgs( PSPY_DATA pSpy )

{
  register PMSG_ITEM pMsg;

  for( /* Init */ pMsg  = ProcessFirstMsg(pSpy);  /* Start @ 1st MSG */

       /* Term */ pMsg != NULL;                   /* Until all MSGs processed */

       /* Iter */ pMsg = ProcessFirstMsg(pSpy) )  /* Restart @ (new) 1st MSG */
  {
    DeleteMsg(pSpy, pMsg->Msg);
  }
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    IncludeMsg                                                   �*/
/*�                                                                          �*/
/*� Function:   mark a specific MSG as Included/Not Included                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID IncludeMsg(         PSPY_DATA pSpy,
                register MSG       Msg,
                         BOOL      Include,
                         MSG_COLOR Color)

{
  register PMSG_ITEM pMsg = FindMsg(pSpy, Msg );

  if (pMsg != NULL)
  {
    pMsg->Include = Include;

    if (Color != Color_Asis)
      pMsg->ClrFG = Color;
  }
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    IncludeGroup                                                 �*/
/*�                                                                          �*/
/*� Function:   process all MSGs in a 'group'; mark as Included/Not Included �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID IncludeGroup  (          PSPY_DATA  pSpy,
                              PGRP_ITEM  pGrp,
                    register  BOOL       Include,
                    register  MSG_COLOR  Color
                   )
{
  PMSG_ITEM  pMsg;

  /* Process each MSG defined in this group */

  for( /* Initialize */ pMsg  = ProcessFirstMsgInGroup(pSpy,pGrp->GroupID);
       /* Terminate  */ pMsg != NULL;     /* stop when all processed */
       /* Iterate    */ pMsg  = ProcessNextMsgInGroup(pSpy)
     )
  {
    IncludeMsg(pSpy, pMsg->Msg, Include, Color);
  }

  /* Maintain GROUP's status */

  pGrp->Include = Include;

  if (Color != Color_Asis)              /* only change if instructed so... */
    pGrp->ClrFG = Color;
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ProcessFirstMsg                                              �*/
/*�                                                                          �*/
/*� Function:   setup processing to sequentially return all MSG's defined    �*/
/*�                                                                          �*/
/*� Returns:     NULL......there are NO messages                             �*/
/*�             !NULL......pointer to first MSG_ITEM                         �*/
/*�                                                                          �*/
/*� Note:       - the MSG's WILL NOT be returned in sequential numeric order!�*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PMSG_ITEM ProcessFirstMsg(PSPY_DATA pSpy)
{
  /* Initialize Processing Data */

  pSpy->ProcessAllMsgs_Slot  = (USHORT) -1;  /* start 1 before 1st SLOT */
  pSpy->ProcessAllMsgs_pData = NULL;         /* say 'need to find next MSG' */

  return( ProcessNextMsg(pSpy) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ProcessNextMsg                                               �*/
/*�                                                                          �*/
/*� Function:   Continue to sequentially return all MSG's defined            �*/
/*�                                                                          �*/
/*� Returns:     NULL......there are NO messages                             �*/
/*�             !NULL......pointer to next MSG_ITEM                          �*/
/*�                                                                          �*/
/*� Note:       - the MSG's WILL NOT be returned in sequential numeric order!�*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PMSG_ITEM ProcessNextMsg(PSPY_DATA pSpy)
{
  register PMSG_DATA pData;

  /* Locate next MSG... */

  while( pSpy->ProcessAllMsgs_pData == NULL )
  {
    /* try the next SLOT */

    if ( ++pSpy->ProcessAllMsgs_Slot >= NO_MSG_SLOTS )
      return( NULL );

    pSpy->ProcessAllMsgs_pData = pSpy->Msgs[pSpy->ProcessAllMsgs_Slot].pFirst;
  }

  /* We've found the next MSG.  Prepare for our next invocation */

  pData                      = pSpy->ProcessAllMsgs_pData;
  pSpy->ProcessAllMsgs_pData = pSpy->ProcessAllMsgs_pData->pNext;

  return( &(pData->item) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    InitializeGroup                                              �*/
/*�                                                                          �*/
/*� Function:   Initialize passed GROUP table for processing                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID InitializeGroup(register PSPY_DATA pSpy )
{
    pSpy->NextGroupID = 0;

    pSpy->GroupHead   =
    pSpy->GroupTail   = NULL;
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    IncludeAllMsgs                                               �*/
/*�                                                                          �*/
/*� Function:   - mark all MSGs   as Included/Not Included                   �*/
/*�             - mark all GROUPs as Included/Not Included                   �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID IncludeAllMsgs(         PSPY_DATA pSpy,
                    register BOOL      Include,
                             MSG_COLOR Color)
{
  register PMSG_ITEM pMsg;
  register PGRP_ITEM pGrp;

  for( /* Initialize */  pMsg = ProcessFirstMsg(pSpy); /* start @ first MSG */
       /* Terminate  */  pMsg != NULL;                 /* stop when all processed */
       /* Iterate    */  pMsg = ProcessNextMsg(pSpy)   /* try the next MSG */
     )
  {
    IncludeMsg(pSpy, pMsg->Msg, Include, Color);
  }

  /* Update the GROUP status, also                                            */

  for(/* Initialize */ pGrp  = ProcessFirstGroup(pSpy); /* start @ 1st */
      /* Terminate  */ pGrp != NULL;                    /* @ end of list */
      /* Iterate    */ pGrp  = ProcessNextGroup(pSpy)   /* move to next */
     )
  {
    pGrp->Include = Include;
  }
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    AddGroup                                                     �*/
/*�                                                                          �*/
/*� Function:   Add specified GROUP to database                              �*/
/*�                                                                          �*/
/*� Returns:    GroupID of new GROUP                                         �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
GROUP AddGroup(PSPY_DATA pSpy,
               PGRP_ITEM pGrp )
{
  PGRP_DATA pData = AllocateMemory( sizeof(GRP_DATA) );

  pData->pNext        = NULL;

  pData->item         = *pGrp;
  pData->item.GroupID = pSpy->NextGroupID++;

  if (pSpy->GroupHead == NULL)
  {
    pSpy->GroupHead =
    pSpy->GroupTail = pData;
  }
  else
  {
    pSpy->GroupTail->pNext = pData;
    pSpy->GroupTail        = pData;
  }

  return(pData->item.GroupID);
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    iFindGroup (Internal)                                        �*/
/*�                                                                          �*/
/*� Function:   Find specified GROUP in database                             �*/
/*�                                                                          �*/
/*� Returns:     NULL....GroupID not found                                   �*/
/*�             !NULL....GroupID     found (this is PGRP_DATA)               �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
static PGRP_DATA iFindGroup(         PSPY_DATA pSpy,
                            register GROUP     GroupID)
{
  register PGRP_DATA pData;

  /* Sequentially scan the list for this GROUP ID */

  for(/* Init */ pData = pSpy->GroupHead;            /* Start @ 1st GROUP */
      /* Term */ (pData != NULL) &&                  /* Until last GROUP seen */
                 (pData->item.GroupID != GroupID);   /* or GROUP found */
      /* Init */ pData = pData->pNext);              /* try the next one */


  return(pData );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    FindGroup                                                    �*/
/*�                                                                          �*/
/*� Function:   Find specified GROUP in database                             �*/
/*�                                                                          �*/
/*� Returns:     NULL....GroupID not found                                   �*/
/*� Returns:    !NULL....GroupID     found (this is PGRP_ITEM)               �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PGRP_ITEM FindGroup(         PSPY_DATA pSpy,
                    register GROUP     GroupID)
{
  register PGRP_DATA pData = iFindGroup(pSpy, GroupID);

  return(pData == NULL ? NULL : &(pData->item) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    FindGroupName                                                �*/
/*�                                                                          �*/
/*� Function:   Find specified GROUP in database (by name)                   �*/
/*�                                                                          �*/
/*� Returns:     NULL....Group not found                                     �*/
/*� Returns:    !NULL....Group     found (this is PGRP_ITEM)                 �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PGRP_ITEM FindGroupName(PSPY_DATA pSpy,
                        PSZ       pName)
{
  register PGRP_DATA pData;

  /* Sequentially scan the list for this GROUP NAME */

  for(/* Init */ pData = pSpy->GroupHead;                 /* Start @ 1st GROUP */
      /* Term */ (pData != NULL) &&                       /* Until last GROUP seen */
                 (strcmp(pData->item.pDesc,pName) != 0);  /* or GROUP found */
      /* Init */ pData = pData->pNext);                   /* try the next one */


  return(pData == NULL ? NULL : &(pData->item) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ProcessFirstMsgInGroup                                       �*/
/*�                                                                          �*/
/*� Function:   setup processing to sequentially return all MSG's defined    �*/
/*�             in this GROUP                                                �*/
/*�                                                                          �*/
/*� Returns:     NULL......there are NO messages                             �*/
/*�             !NULL......pointer to first MSG_ITEM                         �*/
/*�                                                                          �*/
/*� Note:       - the MSG's WILL NOT be returned in sequential numeric order!�*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PMSG_ITEM ProcessFirstMsgInGroup(         PSPY_DATA pSpy,
                                 register GROUP     GroupID)
{
  /* Initialize Processing Data */

  pSpy->ProcessAllMsgsInGroup_ID    = GroupID;
  pSpy->ProcessAllMsgsInGroup_First = BOOL_TRUE;

  return( ProcessNextMsgInGroup(pSpy) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ProcessNextMsgInGroup                                        �*/
/*�                                                                          �*/
/*� Function:   Continue to sequentially return all MSG's defined in this    �*/
/*�                                                                     GROUP�*/
/*� Returns:     NULL......there are NO messages                             �*/
/*�             !NULL......pointer to next MSG_ITEM                          �*/
/*�                                                                          �*/
/*� Note:       - the MSG's WILL NOT be returned in sequential numeric order!�*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PMSG_ITEM ProcessNextMsgInGroup(PSPY_DATA pSpy)
{
  register PMSG_ITEM pMsg;

  for(/* Init */ pMsg = pSpy->ProcessAllMsgsInGroup_First
                        ? ProcessFirstMsg(pSpy)
                        : ProcessNextMsg(pSpy),

                 pSpy->ProcessAllMsgsInGroup_First = BOOL_FALSE;

      /* Term */ (pMsg          != NULL ) &&
                 (pMsg->GroupID != pSpy->ProcessAllMsgsInGroup_ID);
      /* Init */ pMsg = ProcessNextMsg(pSpy)
     );

  return( pMsg );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ProcessFirstGroup                                            �*/
/*�                                                                          �*/
/*� Function:   setup processing to sequentially return all GROUP's defined  �*/
/*�                                                                          �*/
/*� Returns:     NULL......there are NO groups                               �*/
/*�             !NULL......pointer to first GRP_ITEM                         �*/
/*�                                                                          �*/
/*� Note:       - the GROUP's WILL be returned in sequential numeric order!  �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PGRP_ITEM ProcessFirstGroup( register PSPY_DATA pSpy )
{
  /* Initialize Processing Data */

  pSpy->ProcessAllGroups_pGroup  = NULL;
  pSpy->ProcessAllGroups_First   = BOOL_TRUE;

  return( ProcessNextGroup(pSpy) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ProcessNextGroup                                             �*/
/*�                                                                          �*/
/*� Function:   Continue to sequentially return all GROUP's defined          �*/
/*�                                                                          �*/
/*� Returns:     NULL......there are NO groups                               �*/
/*�             !NULL......pointer to next PGRP_ITEM                         �*/
/*�                                                                          �*/
/*� Note:       - the GROUP's WILL be returned in sequential numeric order!  �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
PGRP_ITEM ProcessNextGroup(register PSPY_DATA pSpy)
{
  register PGRP_DATA pGrp;

  if ( pSpy->ProcessAllGroups_First )
  {
    pGrp = pSpy->GroupHead;
    pSpy->ProcessAllGroups_First  = BOOL_FALSE;
  }
  else
  {
    pGrp = pSpy->ProcessAllGroups_pGroup == NULL
           ? NULL
           : pSpy->ProcessAllGroups_pGroup->pNext;
  }

  pSpy->ProcessAllGroups_pGroup = pGrp;             /* save current location */

  return( pGrp == NULL ? NULL : &(pGrp->item) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    DeleteGroup                                                  �*/
/*�                                                                          �*/
/*� Function:   Delete specified GROUP and all MSGs defined in it            �*/
/*�                                                                          �*/
/*� Returns:    FALSE......group not found/deleted                           �*/
/*�             TRUE.......group deleted                                     �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
BOOL DeleteGroup(         PSPY_DATA pSpy,
                 register GROUP     GroupID)
{
  register PGRP_DATA pGrp, pCurGrp, pPrevGrp;

           PMSG_ITEM pMsg;

  if ( (pGrp = iFindGroup(pSpy, GroupID)) != NULL )
  {
    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Phase 1: Delete Group from List                                      �*/
    /*�                                                                      �*/
    /*� Cases: a) Group is 'head'..................first GROUP               �*/
    /*�        b) Group is 'head' and 'tail'.......only  GROUP               �*/
    /*�        c) Group is 'tail'..................last  GROUP               �*/
    /*�        d) Group is not 'head' or 'tail'....not first or last         �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    if (pGrp == pSpy->GroupHead)                /* Head? */
    {
      /* Make second item the first */

      pSpy->GroupHead = pGrp->pNext;

      if (pGrp == pSpy->GroupTail)              /* Also Tail? */
        pSpy->GroupTail = NULL;                 /* yes --> was only in list */
    }
    else
    {
      /* Know not first...scan for this GROUP in linked list */

      for(/* Init */ pCurGrp  = pSpy->GroupHead,
                     pPrevGrp = NULL;

          /* Term */ pCurGrp != pGrp;

          /* Iter */ pPrevGrp = pCurGrp,
                     pCurGrp  = pCurGrp->pNext);

      pPrevGrp->pNext = pGrp->pNext;            /* point PREVIOUS to NEXT */

      if (pGrp == pSpy->GroupTail)              /* also Tail? */
        pSpy->GroupTail = pPrevGrp;
    }

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Phase 2: Delete all MSGs defined for this Group                      �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    for( /* Init */ pMsg  = ProcessFirstMsgInGroup(pSpy, GroupID);

         /* Term */ pMsg != NULL;                 /* Stop after all MSGs processed */

         /* Iter */ pMsg = ProcessNextMsgInGroup(pSpy) )
    {
      DeleteMsg(pSpy, pMsg->Msg );
    }

    /*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
    /*� Phase 3: Free Memory allocated for this Group                        �*/
    /*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/

    FreeMemory(pGrp->item.pDesc);               /* GROUP description */
    FreeMemory(pGrp);                           /* GROUP data */

  }

  return( pGrp == NULL
          ? BOOL_FALSE
          : BOOL_TRUE );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    DeleteAllGroups                                              �*/
/*�                                                                          �*/
/*� Function:   Delete all defined GROUPs                                    �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID DeleteAllGroups(register PSPY_DATA pSpy)
{
  register PGRP_ITEM pGrp;

  for( /* Init */ pGrp  = ProcessFirstGroup(pSpy);    /* start at first */

       /* Term */ pGrp != NULL;                       /* until no GROUPS left */

       /* Iter */ pGrp  = ProcessFirstGroup(pSpy) )   /* restart at new first */
  {
    DeleteGroup(pSpy, pGrp->GroupID );
  }
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    InitializeSpyData                                            �*/
/*�                                                                          �*/
/*� Function:   Initialize a SPY_DATA structure to 'default' values          �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID InitializeSpyData( register PSPY_DATA pSpy)
{
  static XLOGCOLOR defLogicalColorTranslation = {
                                                   { 0, Color1 },
                                                   { 1, Color2 },
                                                   { 2, Color3 },
                                                   { 3, Color4 },
                                                   { 4, Color5 },
                                                   { 5, Color6 },
                                                   { 6, Color7 },
                                                   { 7, Color8 },
                                                };

  memset(pSpy, 0, sizeof(*pSpy) );              /* set to all ZERO's */

  InitializeMsg(pSpy);                          /* MSG   data */
  InitializeGroup(pSpy);                        /* GROUP data */

  memcpy(pSpy->LogicalColorTranslation,         /* COLOR data */
         defLogicalColorTranslation,
         sizeof(XLOGCOLOR) );
}

/*旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커*/
/*� Routine:    ResetSpyData                                                 �*/
/*�                                                                          �*/
/*� Function:   Reset a SPY_DATA structure to 'default' values               �*/
/*읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸*/
VOID ResetSpyData( register PSPY_DATA pSpy)
{
   /* Delete all existing Groups and Messages */

   DeleteAllMsgs(pSpy);
   DeleteAllGroups(pSpy);

   /* Reinitialize... */

   InitializeSpyData(pSpy);
}
