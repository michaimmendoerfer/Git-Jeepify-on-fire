#ifndef PREF_MANAGER_H
#define PREF_MANAGER_H

#include "../../jeepify.h"

void   SavePeers();
void   GetPeers();
void   RegisterPeers();
void   ClearPeers();
void   ClearInit();
void   ReportAll();
int    GetPeerCount();
void   ReportScreen(int s);

extern Preferences preferences;
#endif