#ifndef PEERS_H
#define PEERS_H

#include "../../jeepify.h"

bool   PeriphChanged(struct_Peer *Peer, int Start, int Stop=0);
bool   isPDC (struct_Peer *Peer);
bool   isBat(struct_Peer *Peer);
bool   isSwitch(struct_Periph *Periph);
bool   isSensor(struct_Periph *Periph);
bool   isSensorAmp (struct_Periph *Periph);
bool   isSensorVolt(struct_Periph *Periph);
struct_Periph *FindPeriphById (struct_Peer *Peer, uint16_t Id);
struct_Periph *FindFirstPeriph(struct_Peer *Peer, int Type, bool OnlyActual=false);
struct_Periph *FindNextPeriph (struct_Periph *Periph, int Type=SENS_TYPE_EQUAL, bool OnlyActual=false);
struct_Periph *FindPrevPeriph (struct_Periph *Periph, int Type=SENS_TYPE_EQUAL, bool OnlyActual=false);
struct_Peer   *FindPeerByName (char *Name);
struct_Peer   *FindPeerById   (uint16_t Id);
struct_Peer   *FindPeerByMAC  (const uint8_t *MAC);
struct_Peer   *FindEmptyPeer  ();
struct_Peer   *FindFirstPeer  (int Type=MODULE_ALL);
struct_Peer   *FindNextPeer   (struct_Peer *Peer, int Type=MODULE_ALL);
struct_Peer   *FindPrevPeer   (struct_Peer *Peer, int Type=MODULE_ALL);
int            FindHighestPeerId();
struct_Periph *SelectPeriph();

#endif