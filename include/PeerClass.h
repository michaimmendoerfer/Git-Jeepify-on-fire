#ifndef PEERCLASS_H
#define PEERCLASS_H

#include <Arduino.h>
#include "Jeepify.h"

extern PeerClass *ActivePeer;
extern PeerClass *ActivePDC;
extern PeerClass *ActiveBat;
extern PeerClass *ActiveSelection;
extern PeriphClass *ActiveSens;
extern PeriphClass *ActiveSwitch;
extern PeriphClass *ActivePeriph;

class PeriphClass {
    static int  _ClassId;

    private:
        char        _Name[20];
        int         _Id;
        int         _Type;      //1=Switch, 2=Amp, 3=Volt
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
        void  Setup(char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId);
        
        bool  SetName(char* Name) { strcpy(_Name, Name); return true; }
        char *GetName(){ return (_Name); }
        int   GetId() { return _Id; }
        void  SetId(int Id) { _Id = Id; }
        int   GetType() { return _Type; }
        void  SetType(int Type) { _Type = Type; }
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

        PeriphClass *PeriphClass::GetPtrToSelf() { return this; }
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
        
        void  SetupPeriph(int P, char* Name, int Type, bool isADS, int IOPort, float Nullwert, float VperAmp, int Vin, int PeerId);
        
        char *GetPeriphName(int P) { return Periph[P].GetName(); }
        bool  SetPeriphName(int P, char *Name) { Periph[P].SetName(Name); return true; }
        
        PeriphClass *GetPeriphPtr(int P);
        PeriphClass *GetPeriphPtr(char *Name);
        
        int   GetPeriphId(char *Name);
        int   GetPeriphId(int PosPeriph) { return Periph[PosPeriph].GetId(); }
        char *GetPeriphName(int P);
        bool  SetPeriphName(int P, char *Name);
        
        float GetPeriphValue(int P);
        float GetPeriphValue(char *Name);
        void  SetPeriphValue(int P, float Value);
        void  SetPeriphValue(char *Name, float Value);
        
        float GetPeriphOldValue(int P);
        void  SetPeriphOldValue(int P, float Value) { Periph[P].SetOldValue(Value); }
        
        void  SetPeriphChanged(int P, bool Changed);
        bool  GetPeriphChanged(int P);
        bool  PeriphChanged(int P);
        
        bool  PeriphHasADS(int P);
        bool  GetPeriphADS(int P);
        
        int   GetPeriphType(int P);
        
        int   GetPeriphVin(int P);
        void  SetPeriphVin(int P, float Vin);
        
        float GetPeriphVperAmp(int P);
        void  SetPeriphVperAmp(int P, float VperAmp);
        
        int   GetPeriphIOPort(int P);

        float GetPeriphNullwert(int P);
        float GetPeriphNullwert(char *Name);
        void  SetPeriphNullwert(int P, float Nullwert);
        void  SetPeriphNullwert(char *Name, float Nullwert);

        PeriphClass *GetPeriphPtr(int P);
        PeriphClass *GetPeriphPtr(char *Name);
        

};

PeerClass *FindPeerByMAC(uint8_t *BroadcastAddress);
PeerClass *FindEmptyPeer();

#endif