#include <Arduino.h>
#include "PeerClass.h"
#include "main.h"

PeerClass   *ActivePeer, *ActivePDC, *ActiveBat, *ActiveSelection;
PeriphClass *ActiveSens, *ActiveSwitch, *ActivePeriph;

int  PeriphClass::_ClassId = 1;
int  PeerClass::_ClassId = 1;

char ExportImportBuffer[250];

#pragma region PeriphClass::Declaration
PeriphClass::PeriphClass()
{
    _Id = _ClassId;
    _ClassId++;

    strcpy(_Name, "n.n.");
    _Type = 0;  
    _Pos = 0;       
    _isADS = false;
    _IOPort = 0;
    _Nullwert = 0;
    _VperAmp = 0;
    _Vin = 0;
    _Value = 0;
    _OldValue = 0;
    _Changed = false;
    _PeerId = 0;
}
void  PeriphClass::Setup(const char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId)
{
    strcpy(_Name, Name);
    _Type = Type;
    _isADS = isADS;
    _IOPort = IOPort;
    _Nullwert = Nullwert;
    _VperAmp = VperAmp;
    _PeerId = PeerId;
}
#pragma endregion PeriphClass::Declaration
#pragma region PeerClass::Declaration
PeerClass::PeerClass()
{
    _Id = _ClassId;
    _ClassId++;

    strcpy(_Name, "n.n.") ;
    _Type = 0;  
    _SleepMode = false;
    _DebugMode = false;
    _DemoMode = false;
    _PairMode = false;
    _Changed = false;
    _TSLastSeen = 0;
}
void  PeerClass::Setup(const char* Name, int Type, const uint8_t *BroadcastAddress, bool SleepMode, bool DebugMode, bool DemoMode, bool PairMode)
{
    strcpy(_Name, Name);
    _Type = Type;
    memcpy(_BroadcastAddress, BroadcastAddress, 6);
    _SleepMode = SleepMode;
    _DebugMode = DebugMode;
    _DemoMode  = DemoMode;
    _PairMode  = PairMode;
    
    for (int Si=0; Si<MAX_PERIPHERALS; Si++) Periph[Si].SetPos(Si);
}      
char* PeerClass::Export() 
{
    char ReturnBufferPeriph[40];

    snprintf(ExportImportBuffer, sizeof(ExportImportBuffer), "%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d", 
                        _Name, _Type, 
                        _BroadcastAddress[0], _BroadcastAddress[1], _BroadcastAddress[2],
                        _BroadcastAddress[3], _BroadcastAddress[4], _BroadcastAddress[5],
                        _SleepMode, _DebugMode, _DemoMode);
                        
    for (int Si=0; Si<MAX_PERIPHERALS; Si++)
    { 
        snprintf(ReturnBufferPeriph, sizeof(ReturnBufferPeriph), ";%s;%d",
                 Periph[Si].GetName(), Periph[Si].GetType());

        strcat(ExportImportBuffer, ReturnBufferPeriph);
    }

    return ExportImportBuffer;
}
void PeerClass::Import(char *Buf) 
{
    strcpy(_Name, strtok(Buf, ";"));
    _Type = atoi(strtok(NULL, ";"));
    _BroadcastAddress[0] = atoi(strtok(NULL, ";"));
    _BroadcastAddress[1] = atoi(strtok(NULL, ";"));
    _BroadcastAddress[2] = atoi(strtok(NULL, ";"));
    _BroadcastAddress[3] = atoi(strtok(NULL, ";"));
    _BroadcastAddress[4] = atoi(strtok(NULL, ";"));
    _BroadcastAddress[5] = atoi(strtok(NULL, ";"));
    _SleepMode = atoi(strtok(NULL, ";"));
    _DebugMode = atoi(strtok(NULL, ";"));
    _DemoMode  = atoi(strtok(NULL, ";"));

    for (int Si=0; Si<MAX_PERIPHERALS; Si++)
    {
        Periph[Si].SetName(strtok(NULL, ";"));
        Periph[Si].SetType(atoi(strtok(NULL, ";")));
        Periph[Si].SetPos(Si);
        Periph[Si].SetPeerId(_Id);
    }
}
        
void  PeerClass::PeriphSetup(int Pos, const char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId)
{
    Periph[Pos].Setup(Name, Type, isADS, IOPort, Nullwert, VperAmp, Vin, PeerId);
}
int   PeerClass::GetPeriphId(char *Name)
{
    for (int P=0; P<MAX_PERIPHERALS; P++)
    {   
        if (strcmp(Name, Periph[P].GetName()) == 0) return Periph[P].GetId();
    }
    return -1;
}
void  PeerClass::SetPeriphValue(char *Name, float Value)
{
   for (int P=0; P<MAX_PERIPHERALS; P++)
    {   
        if (strcmp(Name, Periph[P].GetName()) == 0) Periph[P].SetValue(Value); 
    }    
}
float PeerClass::GetPeriphValue(char *Name)
{
   for (int P=0; P<MAX_PERIPHERALS; P++)
    {   
        if (strcmp(Name, Periph[P].GetName()) == 0) return Periph[P].GetValue(); 
    }    

    return -1;
}
float PeerClass::GetPeriphNullwert(char *Name) 
{
    for (int P=0; P<MAX_PERIPHERALS; P++)
    {   
        if (strcmp(Name, Periph[P].GetName()) == 0) return Periph[P].GetNullwert(); 
    }    

    return -1;
}
void  PeerClass::SetPeriphNullwert(char *Name, float Nullwert)
{
    for (int P=0; P<MAX_PERIPHERALS; P++)
    {   
        if (strcmp(Name, Periph[P].GetName()) == 0) Periph[P].SetNullwert(Nullwert);
    }    
}
PeriphClass *PeerClass::GetPeriphPtr(char *Name)
{
    for (int P=0; P<MAX_PERIPHERALS; P++)
    {
        if (strcpy(Periph[P].GetName(), Name) == 0) return &Periph[P];
    }
    return NULL;
}
#pragma endregion PeerClass::Declaration

PeerClass *FindPeerByMAC(const uint8_t *BroadcastAddress)
{
    PeerClass *Peer;
    for(int i = 0; i < PeerList.size(); i++){

		Peer = PeerList.get(i);

		if (memcmp(Peer->GetBroadcastAddress(), BroadcastAddress, 6) == 0) return Peer;
	}
    return NULL;
}
PeerClass *FindPeerById(int Id)
{
    PeerClass *Peer;
    for(int i = 0; i < PeerList.size(); i++){

		Peer = PeerList.get(i);

		if (Peer->GetId() == Id) return Peer;
    }
    return NULL;
}
PeerClass *FindPeerByName(char *Name)
{
    PeerClass *Peer;
    for(int i = 0; i < PeerList.size(); i++) 
    {   
        Peer = PeerList.get(i);
        if (strcmp(Peer->GetName(), Name) == 0) return Peer;
    }
    return NULL;
}
PeerClass *FindFirstPeer(int Type)
// returns first Peer with Type, otherwise NULL
{
    for(int i = 0; i < PeerList.size(); i++) 
    {   
        if ((PeerList.get(i)->GetType() == Type) or (Type == MODULE_ALL)) return PeerList.get(i);
    }
    return NULL;
}
PeerClass *FindNextPeer(PeerClass *P, int Type, bool circular)
// returns next Peer, tries PeerList.size() times, otherwise returns NULL
{
    PeerClass *Peer;
    int ActualPeerIndex;

    //Get PeerIndex in List
    for(int i = 0; i < PeerList.size(); i++) 
    {   
        if (PeerList.get(i) == P) ActualPeerIndex = i;
    }

    for (int i=0; i<PeerList.size(); i++)
    {       
        ActualPeerIndex++;
        if (ActualPeerIndex == PeerList.size()) 
        {   
            if (!circular) return NULL;
            ActualPeerIndex = 0;
        }

        Peer = PeerList.get(ActualPeerIndex);
        
        if (Type == MODULE_ALL) return Peer;
        if (Type == Peer->GetType()) return Peer;
    }
    return NULL;
}
PeerClass *FindPrevPeer(PeerClass *P, int Type, bool circular)
// returns previous Peer, tries PeerList.size() times, otherwise returns NULL
{
    PeerClass *Peer;
    int ActualPeerIndex;

    //Get PeerIndex in List
    for(int i = 0; i < PeerList.size(); i++) 
    {   
        if (PeerList.get(i) == P) ActualPeerIndex = i;
    }

    for (int i=0; i<PeerList.size(); i++)
    {       
        ActualPeerIndex--;
        if (ActualPeerIndex == -1) 
        {   
            if (!circular) return NULL;
            ActualPeerIndex = PeerList.size()-1;
        }

        Peer = PeerList.get(ActualPeerIndex);
        
        if (Type == MODULE_ALL) return Peer;
        if (Type == Peer->GetType()) return Peer;
    }
    return NULL;
}

PeriphClass *FindPeriphById(int Id)
{
    for(int i = 0; i < PeriphList.size(); i++) 
    {   
        if (PeriphList.get(i)->GetId() == Id) return PeriphList.get(i);
    }
    return NULL;
}

PeriphClass *FindFirstPeriph(PeerClass *P, int Type)
// returns first Periph of Type, otherwise returns NULL;
{
    
    for (int i=0; i<MAX_PERIPHERALS; i++)
    {   
        int PType = P->GetPeriphType(i);

        switch (Type) {
            case SENS_TYPE_SENS:    if ((PType == SENS_TYPE_VOLT) or (PType == SENS_TYPE_AMP)) return P->GetPeriphPtr(i); break;
            case SENS_TYPE_VOLT:    if  (PType == SENS_TYPE_VOLT)                              return P->GetPeriphPtr(i); break;
            case SENS_TYPE_AMP:     if  (PType == SENS_TYPE_AMP)                               return P->GetPeriphPtr(i); break;
            case SENS_TYPE_SWITCH:  if  (PType == SENS_TYPE_SWITCH)                            return P->GetPeriphPtr(i); break;
        }
    }
    return NULL;
}
PeriphClass *FindNextPeriph(PeriphClass *PeriphT, int Type, bool circular)
// finds next Periph with given Type from Peer, otherwise NULL
{
    
    
    PeriphClass *Periph;
    PeerClass *P = FindPeerById(PeriphT->GetPeerId());
    int PeriphPos = PeriphT->GetPos();

    for (int i=0; i<MAX_PERIPHERALS; i++)
    {   PeriphPos++;

        if (PeriphPos == MAX_PERIPHERALS)
        {
            if (!circular) return NULL;
            PeriphPos = 0;
        }
    
        Periph = P->GetPeriphPtr(PeriphPos);
        if  (Type == Periph->GetType()) return Periph;
        if ((Type == SENS_TYPE_SENS) and (Periph->IsSensor())) return Periph;
    }
    return NULL;
}
PeriphClass *FindPrevPeriph(PeriphClass *PeriphT, int Type, bool circular)
// returns previous Periph with give Type from Peer, otherwise NULL
{
    PeriphClass *Periph;
    PeerClass *P = FindPeerById(PeriphT->GetPeerId());
    int PeriphPos = PeriphT->GetPos();

    for (int i=0; i<MAX_PERIPHERALS; i++)
    {   PeriphPos--;

        if (PeriphPos == -1)
        {
            if (!circular) return NULL;
            PeriphPos = MAX_PERIPHERALS-1;
        }
    
        Periph = P->GetPeriphPtr(PeriphPos);
        if  (Type == Periph->GetType()) return Periph;
        if ((Type == SENS_TYPE_SENS) and (Periph->IsSensor())) return Periph;
    }
    return NULL;
}
