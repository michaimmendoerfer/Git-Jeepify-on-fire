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
    for(int i = 0; i < PeerList.size(); i++){

		PeerClass *Peer = PeerList.get(i);

		if ((memcmp(Peer->GetBroadcastAddress(), BroadcastAddress, 6) == 0) return Peer;
	}
    for (int D=0; D<MAX_PEERS; D++)
    {   
        if (memcmp(Peer[D].GetBroadcastAddress(), BroadcastAddress, 6) == 0) return &Peer[D];
    }
    return NULL;
}
PeerClass *FindPeerById(int Id)
{
    for (int D=0; D<MAX_PEERS; D++)
    {   
        if (Peer[D].GetId() == Id) return &Peer[D];
    }
    return NULL;
}

PeerClass *FindNextPeer(PeerClass *P, int Type)
{
    for (int D=0; D<MAX_PEERS; D++)
    {   
        if (Peer[D].GetId() == P->GetId()) 
        {
            int DSearch = D;
            for (int DNext=0; DNext<MAX_PEERS; DNext++)
            {    
                DSearch++;
                if (DSearch == MAX_PEERS) DSearch = 0;
                if (Peer[DSearch].GetType() == Type) return &Peer[DSearch];
                if ((Peer[DSearch].GetType() > 0) and (Type = MODULE_ALL)) return &Peer[DSearch];
            }
        }
    }
    return NULL;
}
PeerClass *FindPrevPeer(PeerClass *P, int Type)
{
    for (int D=0; D<MAX_PEERS; D++)
    {   
        if (Peer[D].GetId() == P->GetId()) 
        {
            int DSearch = D;
            for (int DNext=0; DNext<MAX_PEERS; DNext++)
                DSearch--;
                if (DSearch == -1) DSearch = MAX_PEERS-1;
                if (Peer[DSearch].GetType() == Type) return &Peer[D];
                if ((Peer[DSearch].GetType() > 0) and (Type = MODULE_ALL)) return &Peer[D];
        }
    }
    return NULL;
}
PeerClass *FindPeerByName(char *Name)
{
    for (int D=0; D<MAX_PEERS; D++)
    {   
        if (strcmp(Peer[D].GetName(), Name) == 0) return &Peer[D];
    }
    return NULL;
}
PeriphClass *FindNextPeriph(PeriphClass *PeriphT, int Type, bool PeerOnly)
{
    PeerClass *PeerT = FindPeerById(PeriphT->GetPeerId());
    if (PeerT)
    {
        for (int P=0; P<MAX_PERIPHERALS; P++)
        {   
            ////////////if (PeerT->GetPeriphId(P) == PeriphT->GetId()) 
            {
                int PSearch = P;
                for (int PNext=0; PNext<MAX_PERIPHERALS; PNext++)
                {    
                    PSearch++;
                    if (PSearch == MAX_PERIPHERALS) PSearch = 0;
                    
                    if (PeriphT.GetType() == Type) return &Periph[PSearch];
                    if ((Peer[DSearch].GetType() > 0) and (Type = MODULE_ALL)) return &Peer[D];
                }

            }
        }
    }
}
//