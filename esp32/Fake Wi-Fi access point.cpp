#include <WiFi.h>
#include <esp_wifi.h>

const char* ssid = "FREE Wi-Fi";
const char* password = "";

void logConnectedDevices() {
    wifi_sta_list_t stationList;
    tcpip_adapter_sta_list_t adapter_sta_list;

    esp_wifi_ap_get_sta_list(&stationList);
    tcpip_adapter_get_sta_list(&stationList, &adapter_sta_list);

    Serial.println("Connected devices:");
    for (int i = 0; i < adapter_sta_list.num; i++) {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        Serial.print("MAC: ");
        for (int j = 0; j < 6; j++) {
            Serial.printf("%02X", station.mac[j]);
            if (j < 5) Serial.print(":");
        }
        Serial.print(", IP: ");
        Serial.println(ip4addr_ntoa((const ip4_addr_t*)&station.ip));
    }
}

void setup() {
    Serial.begin(115200);


    WiFi.softAP(ssid, password);
    Serial.println("Access point is running");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}

void loop() {
    logConnectedDevices();
    delay(10000);
}
