#include "Module.h"

Module::Module(int index, Pot* _pots[3]){
  this->index = index;
  memcpy(this->pots, _pots, sizeof(Pot*) * 3);
}

void Module::setPots(Pot* _pots[3]){
  memcpy(this->pots, _pots, sizeof(Pot*) * 3);
}

void Module::read(){
  for (int i = 0; i < 3; i++){
    pots[i]->read();
  }
}
