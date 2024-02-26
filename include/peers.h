#ifndef PEERS_H
#define PEERS_H

#include <Arduino.h>
#include "../../jeepify.h"

bool   PeriphChanged(struct_Peer *Peer, int Start, int Stop);
bool   isPDC (struct_Peer *Peer);
bool   isBat(struct_Peer *Peer);
bool   isSwitch(struct_Periph *Periph);
bool   isSensor(struct_Periph *Periph);
bool   isSensorAmp (struct_Periph *Periph);
bool   isSensorVolt(struct_Periph *Periph);
struct_Periph *FindPeriphById (struct_Peer *Peer, uint16_t Id);
struct_Periph *FindFirstPeriph(struct_Peer *Peer, int Type, bool OnlyActual);
struct_Periph *FindNextPeriph (struct_Periph *Periph, int Type, bool OnlyActual);
struct_Periph *FindPrevPeriph (struct_Periph *Periph, int Type, bool OnlyActual);
struct_Peer   *FindPeerByName (char *Name);
struct_Peer   *FindPeerById   (uint16_t Id);
struct_Peer   *FindPeerByMAC  (const uint8_t *MAC);
struct_Peer   *FindEmptyPeer  ();
struct_Peer   *FindFirstPeer  (int Type);
struct_Peer   *FindNextPeer   (struct_Peer *Peer, int Type);
struct_Peer   *FindPrevPeer   (struct_Peer *Peer, int Type);
int            FindHighestPeerId();

extern struct_MultiScreen Screen[MULTI_SCREENS];
extern int ActiveMultiScreen; 
#endif