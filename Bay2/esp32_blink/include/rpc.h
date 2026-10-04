#ifndef RPC_H
#define RPC_H

#include <Arduino.h>

void handleRpc(String requestId, char* payload);
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif 
