#ifndef PEERCLASS_H
#define PEERCLASS_H

#include <Arduino.h>
#include "Jeepify.h"

class PeriphClass {
    static int  _ClassId;

    private:
        char        _Name[20];
        int         _Id;
        int         _Type;      //1=Switch, 2=Amp, 3=Volt
        int         _Pos;       //Periph 1..4.. from one peer
        bool        _isADS;
        int         _IOPort;
        float       _Nullwert;
        float       _VperAmp;
        int         _Vin;
        float       _Value;
        float       _OldValue;
        bool        _Changed;
        int         _PeerId;
    
    public:
        PeriphClass();
        void  Setup(int Pos, char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId);
        
        bool  SetName(char* Name) { strcpy(_Name, Name); return true; }
        char *GetName(){ return (_Name); }
        int   GetId() { return _Id; }
        void  SetId(int Id) { _Id = Id; }
        int   GetType() { return _Type; }
        void  SetType(int Type) { _Type = Type; }
        int   GetPos() { return _Pos; }
        bool  isADS() { return _isADS; }
        bool  GetADS() { return _isADS; }
        void  SetADS(bool isADS) { _isADS = isADS; }
        int   GetIOPort() { return _IOPort; }
        void  SetIOPort(int IOPort) { _IOPort = IOPort; }
        float GetNullwert() { return _Nullwert; }
        void  SetNullwert(float Nullwert) { _Nullwert = Nullwert; }
        float GetVperAmp() { return _VperAmp; }
        void  SetVperAmp(float VperAmp) { _VperAmp = VperAmp; }
        int   GetVin() { return _Vin; }
        void  SetVin(int Vin) { _Vin = Vin; }
        float GetValue() { return _Value; }
        void  SetValue(float Value) { _Value = Value; }
        float GetOldValue() { return _OldValue; }
        void  SetOldValue(float OldValue) { _OldValue = OldValue; }
        bool  hasChanged() { return _Changed; }
        bool  GetChanged() { return _Changed; }
        void  SetChanged(bool Changed) { _Changed = Changed; }
        int   GetPeerId() { return _PeerId; }
        void  SetPeerId(int PeerId) { _PeerId = PeerId; }

        PeriphClass *GetPtrToSelf() { return this; }
};

class PeerClass 
{
    static int _ClassId;

    private:
        char       _Name[20];
        int        _Id;
        int        _Type;  
        u_int8_t   _BroadcastAddress[6];
        bool       _SleepMode;
        bool       _DebugMode;
        bool       _DemoMode;
        bool       _PairMode;
        PeriphClass Periph[MAX_PERIPHERALS]; 
        uint32_t   _TSLastSeen;
        
    
    public:
        PeerClass();
        void  Setup(char* Name, int Type, uint8_t *BroadcastAddress, bool SleepMode, bool DebugMode, bool DemoMode, bool PairMode);
        
        bool  SetName(char *Name) { strcpy(_Name, Name); return true; }
        char *GetName() { return (_Name); }
        int   GetId() { return _Id; }
        void  SetId(int Id) { _Id = Id; }
        int   GetType() { return _Type; }
        void  SetType(int Type) { _Type = Type; }
        uint8_t *GetBroadcastAddress() { return _BroadcastAddress; }
        void     SetBroadcastAddress(int8_t *BroadcastAddress) { memcpy(_BroadcastAddress, BroadcastAddress, 6); }
        uint32_t GetTSLastSeen() { return _TSLastSeen; }
        void     SetTSLastSeen(uint32_t TSLastSeen) { _TSLastSeen = TSLastSeen; }
        bool  GetSleepMode() { return _SleepMode; }
        void  SetSleepMode(bool SleepMode) { _SleepMode = SleepMode; }
        bool  GetDebugMode() { return _DebugMode; }
        void  SetDebugMode(bool DebugMode) { _DebugMode = DebugMode; }
        bool  GetDemoMode() { return _DemoMode; }
        void  SetDemoMode(bool DemoMode) { _DemoMode = DemoMode; }
        bool  GetPairMode() { return _PairMode; }
        void  SetPairMode(bool PairMode) { _PairMode = PairMode; }
        bool  TogglePairMode() { _PairMode = !_PairMode; return _PairMode; }
        
        void  SetupPeriph(int Pos, char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId);
        
        char *GetPeriphName(int P) { return Periph[P].GetName(); }
        bool  SetPeriphName(int P, char *Name) { Periph[P].SetName(Name); return true; }
        
        int   GetPeriphId(char *Name);
        int   GetPeriphId(int PosPeriph) { return Periph[PosPeriph].GetId(); }
        
        float GetPeriphValue(int P) { return Periph[P].GetValue(); }
        float GetPeriphValue(char *Name);
        void  SetPeriphValue(int P, float Value) { Periph[P].SetValue(Value); }
        void  SetPeriphValue(char *Name, float Value);
        
        float GetPeriphOldValue(int P) { return Periph[P].GetOldValue(); }
        void  SetPeriphOldValue(int P, float Value) { Periph[P].SetOldValue(Value); }
        
        void  SetPeriphChanged(int P, bool Changed) { Periph[P].SetChanged(Changed); }
        bool  GetPeriphChanged(int P) { return Periph[P].GetChanged(); }
        bool  PeriphChanged(int P) { return Periph[P].GetChanged(); }
        
        bool  PeriphHasADS(int P) { return Periph[P].GetADS(); }
        bool  GetPeriphADS(int P) { return Periph[P].GetADS(); }
        
        int   GetPeriphType(int P) { return Periph[P].GetType(); }
        
        int   GetPeriphVin(int P) { return Periph[P].GetVin(); }
        void  SetPeriphVin(int P, float Vin) { Periph[P].SetVin(Vin); }
        
        float GetPeriphVperAmp(int P){ return Periph[P].GetVperAmp(); }
        void  SetPeriphVperAmp(int P, float VperAmp) { return Periph[P].SetVperAmp(VperAmp); }
        
        int   GetPeriphIOPort(int P) { return Periph[P].GetIOPort(); }

        float GetPeriphNullwert(int P) { return Periph[P].GetNullwert(); }
        float GetPeriphNullwert(char *Name);
        void  SetPeriphNullwert(int P, float Nullwert) { Periph[P].SetNullwert(Nullwert); }
        void  SetPeriphNullwert(char *Name, float Nullwert);

        PeriphClass *GetPeriphPtr(int P) { return &Periph[P]; }
        PeriphClass *GetPeriphPtr(char *Name);
        

};

PeerClass *FindPeerByMAC(const uint8_t *BroadcastAddress);

extern PeerClass *ActivePeer;
extern PeerClass *ActivePDC;
extern PeerClass *ActiveBat;
extern PeerClass *ActiveSelection;
extern PeriphClass *ActiveSens;
extern PeriphClass *ActiveSwitch;
extern PeriphClass *ActivePeriph;

#endif