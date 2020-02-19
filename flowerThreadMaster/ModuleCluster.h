#include <Arduino.h>
#include <Wire.h>

class ModuleCluster{
  public:
    ModuleCluster();
    ~ModuleCluster();
    
    ModuleCluster(Arduino_h::byte address, int numModules);
    void read();
    void print();
    
  private:
    Arduino_h::byte address;
    int numModules;
    int numBytes;
    int numValues;
    int valueArrIndex;

    //values come in as [high byte, low byte, high byte, low byte, ...]
    Arduino_h::byte* byteValues;
    uint16_t* intValues;
    
    Arduino_h::byte* midiCC;
    Arduino_h::byte midiChannel;

    void parseBytes();
};
