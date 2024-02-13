#include "pref-manager.h"

extern struct_Peer   P[MAX_PEERS];
extern struct_MultiScreen Screen[MULTI_SCREENS];

int PeerCount;

void ReportAll() {
  char Buf[100];
  String BufS;
  Serial.println("Report-All");
  preferences.begin("JeepifyPeers", true);
  
  for (int PNr=0; PNr< MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "%d:%s(%d) - ID:%d -  ", PNr, P[PNr].Name, P[PNr].Type, P[PNr].Id);
    Serial.print(Buf);
    for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
      snprintf(Buf, sizeof(Buf), "P%d:%s(%d), ", Si, P[PNr].Periph[Si].Name, P[PNr].Periph[Si].Type);
      Serial.print(Buf);
    }
    Serial.println();
  }
  
  for (int s=0; s<MULTI_SCREENS; s++) {
    (Screen[s].Used) ? Serial.println("used") : Serial.println("not used");
    if (Screen[s].Used) {
      snprintf(Buf, sizeof(Buf), "S%d:%s, Id=%d - ", s, Screen[s].Name, Screen[s].Id); Serial.println(Buf);
      for (int p=0; p<4; p++) {
        if (Screen[s].Periph[p]) {
          
          Serial.print(p);
          snprintf(Buf, sizeof(Buf), ": PeerId=%d, PeriphId=%d, PeriphName=%s", Screen[s].Periph[p]->PeerId, Screen[s].PeriphId[p], Screen[s].Periph[p]->Name);
          Serial.println(Buf);
        }
      }
      Serial.println();
    }
  }
  preferences.end();
}
void SavePeers() {
  Serial.println("SavePeers...");
  preferences.begin("JeepifyPeers", false);
  
  char Buf[50] = {}; String BufS;
  
  for (int PNr=0; PNr< MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "P%d", PNr);
    Serial.print("Sizeof()"); Serial.print(PNr); Serial.print(") = "); Serial.println(sizeof(P[PNr]));
    preferences.putBytes(Buf, &P[PNr], sizeof(P[PNr]));
  }

  Serial.println("jetzt kommt Multi");

  for (int s=0; s<MULTI_SCREENS; s++) {
    snprintf(Buf, sizeof(Buf), "S%d", s);
    preferences.putBytes(Buf, &Screen[s], sizeof(Screen[s]));
  }
  preferences.end();
}
void GetPeers() {
  preferences.begin("JeepifyPeers", true);
  
  char Buf[50] = {}; 
  PeerCount = 0;

  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "P%d", PNr); 
    //Serial.printf("Sizeof(P[%d]=", sizeof(P[PNr]));
    preferences.getBytes(Buf, &P[PNr], sizeof(P[PNr]));
    if (P[PNr].Id) PeerCount++;
    
    
    for (int SNr=0; SNr<MAX_PERIPHERALS; SNr++) {
      P[PNr].Periph[SNr].PeerId = P[PNr].Id;

      if (DebugMode) {
        snprintf(Buf, sizeof(Buf), "Periph %d: Name=%s, Type=%d, Id=%d, PeerId=%d", SNr, P[PNr].Periph[SNr].Name, P[PNr].Periph[SNr].Type, P[PNr].Periph[SNr].Id, P[PNr].Periph[SNr].PeerId);
        Serial.println(Buf);

        if (isSensor(&P[PNr].Periph[SNr]) and (ActiveSens   == NULL)) ActiveSens   = &P[PNr].Periph[SNr];
        if (isSwitch(&P[PNr].Periph[SNr]) and (ActiveSwitch == NULL)) ActiveSwitch = &P[PNr].Periph[SNr];
      }
    }
  }
  Serial.println("Multi-getpeer");
  for (int s=0; s<MULTI_SCREENS; s++) {
    snprintf(Buf, sizeof(Buf), "S%d", s);
    preferences.getBytes(Buf, &Screen[s], sizeof(Screen[s]));
    
    for (int p=0; p<PERIPH_PER_SCREEN; p++) {      
      if (Screen[s].PeerId[p]) {
        Screen[s].Peer[p]   = FindPeerById(Screen[s].PeerId[p]);
        Serial.print("Peer[p]=");   Serial.println(Screen[s].Peer[p]->Name);
      }
      if (Screen[s].PeriphId[p]) {
        Screen[s].Periph[p] = FindPeriphById(Screen[s].Peer[p], Screen[s].PeriphId[p]);
        Serial.print("Periph[p]="); Serial.println(Screen[s].Periph[p]->Name);
      }
    }  
  }
  preferences.end();
}
void RegisterPeers() {
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  memset(&peerInfo, 0, sizeof(peerInfo));

  // Register BROADCAST
  for (int b=0; b<6; b++) peerInfo.peer_addr[b] = (uint8_t) broadcastAddressAll[b];
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      PrintMAC(peerInfo.peer_addr); Serial.println(": Failed to add peer");
    }
    else {
      Serial.print (" ("); PrintMAC(peerInfo.peer_addr);  Serial.println(") added...");
    }

  // Register Peers
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Type > 0) {
      for (int b=0; b<6; b++) peerInfo.peer_addr[b] = (uint8_t) P[PNr].BroadcastAddress[b];
        if (esp_now_add_peer(&peerInfo) != ESP_OK) {
          PrintMAC(peerInfo.peer_addr); Serial.println(": Failed to add peer");
        }
        else {
          Serial.print("Peer: "); Serial.print(P[PNr].Name); 
          Serial.print (" ("); PrintMAC(peerInfo.peer_addr); Serial.println(") added...");
        }
    }
  }
}
void ClearPeers() {
  preferences.begin("JeepifyPeers", false);
    preferences.clear();
    Serial.println("JeepifyPeers cleared...");
  preferences.end();
}
void DeletePeer(struct_Peer *Peer) {
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Id == Peer->Id) {
      P[PNr].Name[0] = '\0';
      P[PNr].Id = 0;
      P[PNr].Type = 0;
      for (int i; i<6; i++) P[PNr].BroadcastAddress[i] = 0;
      P[PNr].TSLastSeen = 0;
      P[PNr].SleepMode = false;
      P[PNr].DebugMode = false;
    }
  }
  SendCommand(Peer, "Reset");
  
  for (int s=0; s<MULTI_SCREENS; s++) {
    for (int p=0; p<PERIPH_PER_SCREEN; p++) {
      if (Screen[s].PeerId[p] == Peer->Id) {
        Screen[s].PeerId[p] = 0;
        Screen[s].PeriphId[p] = 0;
        Screen[s].Periph[p] = NULL;
      }
    } 
  }
}