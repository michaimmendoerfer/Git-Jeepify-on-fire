#include <Arduino.h>
#include "PeerClass.h"
#include "main.h"

PeerClass   *ActivePeer, *ActivePDC, *ActiveBat, *ActiveSelection;
PeriphClass *ActiveSens, *ActiveSwitch, *ActivePeriph;

int  PeriphClass::_ClassId = 0;
int  PeerClass::_ClassId = 0;

#pragma region PeriphClass::Declaration
PeriphClass::PeriphClass()
{
    _Id = _ClassId;
    _ClassId++;
}
void  PeriphClass::Setup(int Pos, char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId)
{
    strcpy(_Name, Name);
    _Type = Type;
    _Pos = Pos;
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
}
void  PeerClass::Setup(char* Name, int Type, uint8_t *BroadcastAddress, bool SleepMode, bool DebugMode, bool DemoMode, bool PairMode)
{
    strcpy(_Name, Name);
    _Type = Type;
    for (int b=0; b<6; b++)_BroadcastAddress[b] = BroadcastAddress[b];
    _SleepMode = SleepMode;
    _DebugMode = DebugMode;
    _DemoMode  = DemoMode;
    _PairMode  = PairMode;
}      
void  PeerClass::SetupPeriph(int Pos, char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId)
{
    Periph[Pos].Setup(Pos, Name, Type, isADS, IOPort, Nullwert, VperAmp, Vin, PeerId);
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

PeerClass *FindPeerByMAC(uint8_t *BroadcastAddress)
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
PeerClass *FindNextPeer(PeerClass *P, int Type, bool circular)
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
        if (ActualPeerIndex = PeerList.size()) 
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
{
    PeerClass *Peer;
    int ActualPeerIndex;

    //Get PeerIndex in List
    for(int i = 0; i < PeerList.size(); i++) 
    {   
        if (PeerList.get(i) == P) ActualPeerIndex = i;
    }

    for (int i=0; i<PeerList.size(); i++)
    {   ActualPeerIndex--;
        if (ActualPeerIndex > -1)
        {
            Peer = PeerList.get(ActualPeerIndex);
            
            if (Type == MODULE_ALL) return Peer;
            if (Type == Peer->GetType()) return Peer;
        }
        else
        {
            if (circular) ActualPeerIndex = PeerList.size();
            else return NULL;
        }
    }
    return NULL;
}
PeriphClass *FindNextPeriph(PeriphClass *PeriphT, int Type, bool circular)
{
    PeriphClass *Periph;
    PeerClass *Peer = FindPeerById(PeriphT->GetPeerId());
    int PeriphPos = PeriphT->GetPos();

    for (int i=0; i<MAX_PERIPHERALS; i++)
    {   PeriphPos++;
        if (PeriphPos < MAX_PERIPHERALS)
        {
            Periph = Peer->GetPeriphPtr(PeriphPos);
            if  (Type == Periph->GetType()) return Periph;
            if ((Type == SENS_TYPE_SENS) and (Periph->IsSensor())) return Periph;
        }
        else
        {
            if (circular) PeriphPos = -1;
            else return NULL;
        }
    }
}
PeriphClass *FindPrevPeriph(PeriphClass *PeriphT, int Type, bool circular)
{
    PeriphClass *Periph;
    PeerClass *Peer = FindPeerById(PeriphT->GetPeerId());
    int PeriphPos = PeriphT->GetPos();

    for (int i=0; i<MAX_PERIPHERALS; i++)
    {   PeriphPos--;
        if (PeriphPos > -1)
        {
            Periph = Peer->GetPeriphPtr(PeriphPos);
            if  (Type == Periph->GetType()) return Periph;
            if ((Type == SENS_TYPE_SENS) and (Periph->IsSensor())) return Periph;
        }
        else
        {
            if (circular) PeriphPos = MAX_PERIPHERALS;
            else return NULL;
        }
    }
}
//