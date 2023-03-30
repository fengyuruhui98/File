//shmetro24G.h
#ifndef _SHMETRO24G_H_
#define _SHMETRO24G_H_
//start of file

extern UBYTE g24GFlag;
extern UBYTE EntryPosID[4],ExitPosID[4];
extern UDWORD TotalPurse,LastTransValue;
extern UWORD TotalCnt;
extern UBYTE LocationID;
extern UBYTE CurrentLocation;
extern UBYTE Record24G[65];
extern UBYTE OtherTransFlag;
extern UBYTE gTransTypeFlag;
extern UBYTE gPosCityID[2];
extern UBYTE gTransValueMax[4];
extern UBYTE gLastEntryTime[7];


int trans_entry_24G(void);
int trans_exit_24G(UBYTE transtype);
int trans_lock_24G(void);
int trans_update_24G(UBYTE *updatecache);
int trans_getproof_24G(UBYTE *sim_sn);

//end of file
#endif