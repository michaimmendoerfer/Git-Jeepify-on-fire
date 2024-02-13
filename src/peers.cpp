#include "peers.h"

struct_Peer   P[MAX_PEERS];
struct_Peer   *ActivePeer, *ActivePDC, *ActiveBat, *ActiveSelection;
struct_Periph *ActiveSens, *ActiveSwitch, *ActivePeriph;

struct_Periph *FindFirstPeriph(struct_Peer *Peer, int Type, bool OnlyActual){
  if (!Peer) Peer = FindFirstPeer();
   if (Peer) {
    for (int PNr=0; PNr<MAX_PEERS; PNr++) {
      for (int SNr=0; SNr<MAX_PERIPHERALS; SNr++) {
        switch (Type) {
          case SENS_TYPE_ALL:    if (Peer->Periph[SNr].Type > 0)       { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          case SENS_TYPE_AMP:    if (isSensorAmp(&Peer->Periph[SNr]))  { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          case SENS_TYPE_VOLT:   if (isSensorVolt(&Peer->Periph[SNr])) { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          case SENS_TYPE_SENS:   if (isSensor(&Peer->Periph[SNr]))     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          case SENS_TYPE_SWITCH: if (isSwitch(&Peer->Periph[SNr]))     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
        }
      }
      if (OnlyActual) return NULL;
      struct_Peer *TempPeer = FindNextPeer(Peer);
      if (TempPeer == Peer) return NULL; else Peer = TempPeer;
    }
  }
  return NULL;
}

struct_Periph *FindNextPeriph (struct_Periph *Periph, int Type, bool OnlyActual){
  struct_Peer *Peer = FindPeerById(Periph->PeerId);    
  int SNr = Periph->Id;

  if ((Periph) and (Peer)) {
    if (Type == SENS_TYPE_EQUAL) Type = Periph->Type;

    for (int SNr=Periph->Id; SNr < MAX_PERIPHERALS; SNr++) {
      switch (Type) {
          case SENS_TYPE_ALL:
            if (Peer->Periph[SNr].Type > 0)                  { ActivePeer = Peer; return &Peer->Periph[SNr]; break;}
            break;
          case SENS_TYPE_SENS:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
          case SENS_TYPE_SWITCH:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_SWITCH)  { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
          case SENS_TYPE_AMP:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
          case SENS_TYPE_VOLT:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
      }
    }

    for (int PNr=0; PNr<MAX_PEERS; PNr++) {
      if (!OnlyActual) {
        struct_Peer *TempPeer = FindNextPeer(Peer);
        if (TempPeer == Peer) return Periph; else Peer = TempPeer;
      }
        
      for (SNr=0; SNr < MAX_PERIPHERALS; SNr++) {
        switch (Type) {
          case SENS_TYPE_ALL:
            if (Peer->Periph[SNr].Type > 0)                  { ActivePeer = Peer; return &Peer->Periph[SNr]; break;}
            break;
          case SENS_TYPE_SENS:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
          case SENS_TYPE_SWITCH:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_SWITCH)  { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
          case SENS_TYPE_AMP:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
          case SENS_TYPE_VOLT:  
            if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
            break;
        }
      }
    }
  }
  return NULL;
}

struct_Periph *FindPrevPeriph (struct_Periph *Periph, int Type, bool OnlyActual){
  struct_Peer *Peer = FindPeerById(Periph->PeerId);    

  if ((Periph) and (Peer)) {
    if (Type == SENS_TYPE_EQUAL) Type = Periph->Type;

    for (int SNr=Periph->Id-2; SNr >= 0; SNr--) { // -1-1 weil Id bei 1 anfängt
      
      Serial.print("Peer = "); Serial.println(Peer->Name);
      Serial.print("Periph = "); Serial.println(Periph->Name);
      Serial.print("SNr = "); Serial.println(SNr);
      Serial.print("Orig SNr = "); Serial.println(Periph->Id);
      
      switch (Type) {
        case SENS_TYPE_ALL:
          if (Peer->Periph[SNr].Type > 0)                  { ActivePeer = Peer; return &Peer->Periph[SNr]; break;}
          break;
        case SENS_TYPE_SENS:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
        case SENS_TYPE_SWITCH:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_SWITCH)  { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
        case SENS_TYPE_AMP:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
        case SENS_TYPE_VOLT:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
      }
    }
  }

  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (!OnlyActual) {
        struct_Peer *TempPeer = FindNextPeer(Peer);
        if (TempPeer == Peer) return Periph; else Peer = TempPeer;
      }

    for (int SNr=MAX_PERIPHERALS-1; SNr>0; SNr--) {
      switch (Type) {
        case SENS_TYPE_ALL:
          if (Peer->Periph[SNr].Type > 0)                  { ActivePeer = Peer; return &Peer->Periph[SNr]; break;}
          break;
        case SENS_TYPE_SENS:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
        case SENS_TYPE_SWITCH:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_SWITCH)  { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
        case SENS_TYPE_AMP:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_AMP)     { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
        case SENS_TYPE_VOLT:  
          if (Peer->Periph[SNr].Type == SENS_TYPE_VOLT)    { ActivePeer = Peer; return &Peer->Periph[SNr]; break; }
          break;
      }
    }
  }
  return NULL;
}

struct_Periph *FindPeriphById(struct_Peer *Peer, uint16_t Id) {
  if (Peer) {
    for (int SNr=0; SNr<MAX_PERIPHERALS; SNr++) {
      if (Peer->Periph[SNr].Id == Id) return &Peer->Periph[SNr];
    }
  }
  return NULL;
}

struct_Periph *SelectPeriph() {
  if (!ActivePeer) {
    ActivePeer = FindFirstPeer();
  }
  if (ActivePeer) {
    if (!ActivePeriph) {
      ActivePeriph = FindFirstPeriph(ActivePeer, SENS_TYPE_ALL, false);
    }
    if (ActivePeriph) {
      if ((TSScreenRefresh - millis() > SCREEN_INTERVAL) or (Mode != OldMode)) {
        ScreenChanged = true;
        TFTBuffer.pushImage(0,0, 240, 240, JeepifyBackground);
        TFTBuffer.loadFont(AA_FONT_LARGE);
        TFTBuffer.setTextDatum(MC_DATUM);
        TFTBuffer.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
        TFTBuffer.drawString(ActivePeriph->Name, 120, 120);
        TFTBuffer.unloadFont();
        
        TFTBuffer.loadFont(AA_FONT_SMALL);
        TFTBuffer.drawString(P[ActivePeriph->PeerId].Name, 120,  80);
        
        switch (ActivePeriph->Type) {
          case SENS_TYPE_AMP:    TFTBuffer.drawString("Amp-Sensor",  120, 160); break;
          case SENS_TYPE_VOLT:   TFTBuffer.drawString("Volt-Sensor", 120, 160); break;
          case SENS_TYPE_SWITCH: TFTBuffer.drawString("Switch",      120, 160); break;
        }
        TSScreenRefresh = millis();      
        return (ActivePeriph);
      }
    }
  }
  return NULL;
}

struct_Peer   *FindPeerByName(char *Name) {
    for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    //Serial.print(P[PNr].Name); Serial.print(" =? "); Serial.println(Name);
    if (strcmp(P[PNr].Name, Name) == 0) return &P[PNr];
  }
  //Serial.println("durchgelaufen");
  return NULL;
}

struct_Peer   *FindPeerById  (uint16_t Id) {
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Id == Id) return &P[PNr];
  }
  return NULL;
}

struct_Peer   *FindPeerByMAC (const uint8_t *MAC) {
  Serial.print("gesuchte MAC: "); PrintMAC(MAC);

  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if(memcmp(P[PNr].BroadcastAddress, MAC, 6) == 0) {
      Serial.print(" ist: "); Serial.println(P[PNr].Name);
      return &P[PNr];
    }
  }
  Serial.println(" nicht gefunden");
  return NULL;
}

struct_Peer   *FindEmptyPeer () {
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Type == 0) return &P[PNr];
  }
  return NULL;
}

struct_Peer   *FindFirstPeer (int Type) {
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if ( P[PNr].Type == Type)                   return &P[PNr];
    if ((P[PNr].Type) and (Type == MODULE_ALL)) return &P[PNr];
  }
  return NULL;
}

struct_Peer   *FindNextPeer  (struct_Peer *Peer, int Type) {
  if (Peer) {
    int PNr = Peer->PNumber;

    for (int i=0; i<MAX_PEERS; i++) {
      PNr++; if (PNr == MAX_PEERS) { PNr = 0; }
      if  (P[PNr].Type == Type)                   return &P[PNr];
      if ((P[PNr].Type) and (Type == MODULE_ALL)) return &P[PNr];
    }
  }
  return Peer;
}

struct_Peer   *FindPrevPeer  (struct_Peer *Peer, int Type) {
  if (Peer) {
    int PNr = Peer->PNumber;
    
    for (int i=0; i<MAX_PEERS; i++) {
      PNr--; if (PNr == -1) { PNr = MAX_PEERS-1; }
      if  (P[PNr].Type == Type)                   return &P[PNr];
      if ((P[PNr].Type) and (Type == MODULE_ALL)) return &P[PNr];
    }
  }
  return Peer;
}

int            FindHighestPeerId() {
  int HighestId = -1;
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Id > HighestId) HighestId = P[PNr].Id;
  }
  return HighestId;
}

bool isPDC(struct_Peer *Peer) {
  if (Peer) return ((Peer->Type == SWITCH_1_WAY) or (Peer->Type == SWITCH_2_WAY) or 
                    (Peer->Type == SWITCH_4_WAY) or (Peer->Type == SWITCH_8_WAY) or
                    (Peer->Type == PDC_SENSOR_MIX));   
  return false;   
}

bool isBat(struct_Peer *Peer) {
  if (Peer) return ((Peer->Type == BATTERY_SENSOR) or (Peer->Type == PDC_SENSOR_MIX));
  return false;
}

bool isSwitch(struct_Periph *Periph) {
  if (Periph) return (Periph->Type == SENS_TYPE_SWITCH); 
  return false;     
}

bool isSensor(struct_Periph *Periph) {
  if (Periph) return((Periph->Type == SENS_TYPE_AMP) or (Periph->Type == SENS_TYPE_VOLT));
  return false;
}

bool isSensorVolt(struct_Periph *Periph) {
  if (Periph) return (Periph->Type == SENS_TYPE_VOLT);
  return false;
}

bool isSensorAmp(struct_Periph *Periph) {
  if (Periph) return (Periph->Type == SENS_TYPE_AMP);
  return false;
}

bool PeriphChanged(struct_Peer *Peer, int Start, int Stop) {
  int ret = false;
  if (Peer) {
    if (Stop == 0) Stop = Start;
    for (int Si=Start; Si++; Si<Stop+1) {
      if (Si == MAX_PERIPHERALS) break;
      if (Peer->Periph[Si].Changed) ret = true;
    }
  }
  return ret;
}

