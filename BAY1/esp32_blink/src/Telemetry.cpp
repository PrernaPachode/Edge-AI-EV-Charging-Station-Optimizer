#include <ArduinoJson.h>
#include "telemetry.h"
#include "network.h"
#include "state.h"
#include "config.h"

void publishTelemetry() {
  if (!mqtt.connected()) return;

  //store the values as key value pair format 
  StaticJsonDocument<350> doc;
  doc["bayId"] = BAY_ID;
  doc["voltage"] = round(voltage * 10) / 10.0;
  doc["current"] = round(current * 10) / 10.0;
  doc["power"] = round(power * 10) / 10.0;
  doc["temperature"] = round(temperature * 10) / 10.0;
  doc["bayStatus"] = bayStatus;
  //adding 2 more values to the buffer
  doc["predictedArrivalProb"] = round(predictedArrivalProb * 100)/100.0;
  doc["predictedDurationMin"] = predictedDurationMin;
  // add load decision, throttle level, overload
  doc["throttleLevel"] = throttleLevel;
  doc["loadDecision"]= loadDecision;
  doc["overloadActive"] = overloadActive;

  char buffer[350];
  serializeJson(doc, buffer);

  //push the data to the cloud
  mqtt.publish("v1/devices/me/telemetry", buffer);
  Serial.print("[MQTT >>] ");
  Serial.println(buffer);
}

