#ifndef JEEPIFY_H
#define JEEPIFY_H

// Touch
#define SWIPE_LEFT  10
#define SWIPE_RIGHT 11
#define SWIPE_DOWN  12
#define SWIPE_UP    13
#define LONG_PRESS   5
#define DBLCLICK     4
#define CLICK        3
#define HOLD         2
#define TOUCHED      1
#define LONG_PRESS_INTERVAL 300

#define MAX_PERIPHERALS 5
#define MAX_PEERS       10
#define MAX_STATUS      10
#define SCHWELLE        0.0005

// Module-Types
#define SWITCH_1_WAY        1
#define SWITCH_2_WAY        2
#define SWITCH_4_WAY        4
#define SWITCH_8_WAY        8
#define PDC                 9
#define PDC_SENSOR_MIX      10
#define BATTERY_SENSOR      20
#define MONITOR_ROUND       30
#define MONITOR_BIG         31
#define MODULE_ALL          99

// Sensor-Types
#define SENS_TYPE_SWITCH  1
#define SENS_TYPE_AMP     2
#define SENS_TYPE_VOLT    3
#define SENS_TYPE_SENS    4
#define SENS_TYPE_EQUAL   5
#define SENS_TYPE_ALL     6
#define NOT_FOUND        99

// Screens
#define S_MENU            1
#define S_STATUS         50
#define S_SENSOR1       101
#define S_SENSOR4       104
#define S_SWITCH1       111
#define S_SWITCH4       114
#define S_SWITCH8       118
#define S_MULTI         119
#define S_JSON          110
#define S_PEER          120
#define S_PEERS         121
#define S_PEER_SEL      122
#define S_PERI_SEL      123
#define S_SETTING       130
#define S_PAIRING       140
#define S_CAL_VOL       150
#define S_EICHEN        151

// Intervals
#define TOUCH_INTERVAL    100
#define SCREEN_INTERVAL   1000
#define MSGLIGHT_INTERVAL 300
#define PING_INTERVAL     1000
#define MSG_INTERVAL      1000
#define LOGO_INTERVAL     3000
#define OFFLINE_INTERVAL  20000
#define PAIR_INTERVAL     30000
#define SLEEP_INTERVAL    5000

//#define WAIT_FOR_MAMA     20000

// Fonts
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
#define AA_FONT_MONO  NotoSansMonoSCB20 

//Colors
#define RED2RED     0
#define GREEN2GREEN 1
#define BLUE2BLUE   2
#define BLUE2RED    3
#define GREEN2RED   4
#define RED2GREEN   5
#define TFT_GREY    0x2104 
#define TFT_RUBICON 0xa841

// Round-Monitor Touch
const int I2C_SDA  = 4;
const int I2C_SCL  = 5;
const int TP_INT   = 0;
const int TP_RST   = 1;
const int Rotation = 0;

// ESP32-3248S035 Touch
#define TOUCH_SDA  33
#define TOUCH_SCL  32
#define TOUCH_INT  21
#define TOUCH_RST 25
#define TOUCH_WIDTH  480
#define TOUCH_HEIGHT 320

//structs
struct struct_Periph {
    char        Name[20];
    int         Id;
    int         Type;      //1=Switch, 2=Amp, 3=Volt
    bool        isADS;
    int         IOPort;
    float       NullWert;
    float       VperAmp;
    int         Vin;
    float       Value;
    float       OldValue;
    bool        Changed;
    int         PeerId;
    void        *Peer;
};
typedef struct struct_Periph struct_Periph;

struct struct_Peer {
    char       Name[20];
    int        Id;
    int        PNumber;
    u_int8_t   BroadcastAddress[6];
    uint32_t   TSLastSeen;
    int        Type;  // 
    bool       SleepMode;
    bool       DebugMode;
    bool       DemoMode;
    bool       ReadyToPair;
    struct_Periph Periph[MAX_PERIPHERALS]; 
};
typedef struct struct_Peer struct_Peer;

#define MULTI_SCREENS 10 // MAX_PEERS+10 0-9 Peers, 10-19 Multi
#define MULTI_SCREEN_ROWS 2
#define MULTI_SCREEN_COLS 2
#define PERIPH_PER_SCREEN MULTI_SCREEN_COLS*MULTI_SCREEN_ROWS

struct struct_MultiScreen {
  char           Name[20];
  int            Id;
  struct_Peer   *Peer    [PERIPH_PER_SCREEN];
  int            PeerId  [PERIPH_PER_SCREEN];
  struct_Periph *Periph  [PERIPH_PER_SCREEN];
  int            PeriphId[PERIPH_PER_SCREEN];
  bool           Used;
};
typedef struct struct_MultiScreen struct_MultiScreen;

struct struct_Button {
  int x, y, w, h;
  int TxtColor, BGColor;
  char Name[20];
  bool Status;
};
struct struct_Touch {
  uint16_t x0, x1, y0, y1;
  bool TouchedOld;
  bool Touched;
  uint8_t  Gesture;
  uint32_t TSFirstTouch;
  uint32_t TSReleaseTouch;
};

#endif