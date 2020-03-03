#include <Arduino.h>
#include <Wire.h>


enum PrintType{
  INT,
  BYTE
};


class ModuleCluster{
  public:
    ModuleCluster();
    ~ModuleCluster();
    
    ModuleCluster(Arduino_h::byte address, int numModules, Arduino_h::HardwareSerial* ser);
    void read();
    void print(PrintType pt);
    Arduino_h::HardwareSerial* getSerial();
    
  private:
    Arduino_h::byte address;
    int numModules;
    int numBytes;
    int numValues;
    int valueArrIndex;
    unsigned long startTime;
    Arduino_h::HardwareSerial* ser;

    //values come in as [high byte, low byte, high byte, low byte, ...]
    Arduino_h::byte* byteValues;
    uint16_t* intValues;
    
    Arduino_h::byte* midiCC;
    Arduino_h::byte midiChannel;

    void parseBytes();
};
