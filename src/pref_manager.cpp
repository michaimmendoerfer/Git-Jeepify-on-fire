#include <Arduino.h>
#include <Preferences.h>
#include <esp_now.h>
#include <WiFi.h>
#include "pref_manager.h"
#include "peers.h"

int PeerCount;

Preferences preferences;

int GetPeerCount() { return PeerCount; }

void testP(){
  for (int i=0; i<MAX_PERIPHERALS; i++) 
  {
      Serial.printf("P[0].Periph[%d].Name = %s, &Name = ", i, P[0].Periph[i].Name);
      Serial.println((unsigned)(&P[0].Periph[i].Name[0]), HEX);

      struct_Peer *Peer = FindFirstPeer(MODULE_ALL);
      Serial.println(Peer->Periph[2].Name);
  }
}

void ReportAll() {
  char Buf[100];
  String BufS;
  Serial.println("Report-All - Array-Zugriff");
  for (int PNr=0; PNr< MAX_PEERS; PNr++) {      
    Serial.printf("%d:%s(%d) - ID:%d ---- ", PNr, P[PNr].Name, P[PNr].Type, P[PNr].Id);
    for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
      Serial.printf("%d:%s (%d), ", Si, P[PNr].Periph[Si].Name, P[PNr].Periph[Si].Type);
    }
    Serial.println();
  }
  
  for (int s=0; s<MULTI_SCREENS; s++) {
    Serial.print(Screen[s].Name); Serial.print(": ");
    (Screen[s].Used) ? Serial.println("used") : Serial.println("not used");
    
    if (Screen[s].Used) {
      Serial.printf("S%d:%s, Id=%d ---- ", s, Screen[s].Name, Screen[s].Id);
      for (int p=0; p<PERIPH_PER_SCREEN; p++) {
        if (Screen[s].Periph[p]->Type > 0) {
          Serial.printf("%d: PeerId=%d, PeriphId=%d, PeriphName=%s", p, Screen[s].Periph[p]->PeerId, Screen[s].PeriphId[p], Screen[s].Periph[p]->Name);
        }
      }
      Serial.println();
    }
  }
}
void ReportScreen(int s) {
  char Buf[100];
  String BufS;
  Serial.println("Report-Screen");
  
  Serial.print(Screen[s].Name); Serial.print(": ");
  (Screen[s].Used) ? Serial.println("used") : Serial.println("not used");
  
  if (Screen[s].Used) {
    snprintf(Buf, sizeof(Buf), "S%d:%s, Id=%d - ", s, Screen[s].Name, Screen[s].Id); Serial.println(Buf);
    for (int p=0; p<PERIPH_PER_SCREEN; p++) {
      if (Screen[s].Periph[p]->Type > 0) {
        Serial.print("---Value=");
        Serial.print(Screen[s].Periph[p]->Value, 2);
        snprintf(Buf, sizeof(Buf), ": ,Name=%s, PeerId=%d, PeriphId=%d", Screen[s].Periph[p]->Name, Screen[s].Periph[p]->PeerId, Screen[s].PeriphId[p]);
        Serial.println(Buf);
      }
    }
    Serial.println();
  }

}
void SavePeers() {
  Serial.println("SavePeers...");
  preferences.begin("JeepifyPeers", false);
  
  char Buf[50] = {};
  
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

  ChangesSaved = true;
  ReportAll();
}
void GetPeers() {
  preferences.begin("JeepifyPeers", true);
  
  char Buf[50] = {}; 
  PeerCount = 0;

  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "P%d", PNr); 
    preferences.getBytes(Buf, &P[PNr], sizeof(P[PNr]));
    if (P[PNr].Id) {
      PeerCount++;
      if (ActivePeer == NULL) ActivePeer = &P[PNr];
    }
    
    
    for (int SNr=0; SNr<MAX_PERIPHERALS; SNr++) {
      P[PNr].Periph[SNr].PeerId = P[PNr].Id;

      if (DebugMode) {
        snprintf(Buf, sizeof(Buf), "Periph %d: Name=%s, Type=%d, Id=%d, PeerId=%d", SNr, P[PNr].Periph[SNr].Name, P[PNr].Periph[SNr].Type, P[PNr].Periph[SNr].Id, P[PNr].Periph[SNr].PeerId);
        Serial.println(Buf);

        if (isSensor(&P[PNr].Periph[SNr]) and (ActiveSens   == NULL)) { 
          ActiveSens   = &P[PNr].Periph[SNr]; 
          if (DebugMode) { Serial.print("ActiveSens = "); Serial.println(ActiveSens->Name); }
        }
        if (isSwitch(&P[PNr].Periph[SNr]) and (ActiveSwitch == NULL)) {
          ActiveSwitch = &P[PNr].Periph[SNr];
          if (DebugMode) { Serial.print("ActiveSwitch = "); Serial.println(ActiveSwitch->Name); }
        }
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
  for (int b=0; b<6; b++) peerInfo.peer_addr[b] = 0xff;
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
void ClearInit() {
  preferences.begin("JeepifyInit", false);
    preferences.clear();
    Serial.println("JeepifyInit cleared...");
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