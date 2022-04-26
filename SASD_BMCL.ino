//https://dl.espressif.com/dl/package_esp32_index.json
// Hello! I'm NATSAKORN L. I'm student 3C at BMCL of Electronics Engineering at KMITL.
#include "BluetoothSerial.h"
//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif
/*
#include<WiFi.h>
#define SSID_NAME "BMCL 3C-2021"
#define SSID_PASS "bmcllcmb"
IPAddress local_ip = {192,168,1,1};
IPAddress gateway = {192,168,1,1};
IPAddress subnet = {255,255,255,0};
*/
//
BluetoothSerial BTSerial;
void setup() {
  Serial.begin(115200);
  BTSerial.begin("BMCL"); //Bluetooth device name
  pinMode(13, INPUT);     //--- Sensor ECG (AD8232)
  pinMode(33, INPUT);     //--- Sensor Photodiode
  pinMode(34, INPUT);     //--- Sensor Thermal
  pinMode(35, INPUT);     //--- Record Button
  pinMode(2, OUTPUT);     //--- LED Red
  pinMode(4, OUTPUT);     //--- LED Green
  pinMode(18, OUTPUT);    //--- OXIMETER Infrared
  pinMode(19, OUTPUT);    //--- OXIMETER Red
}

void loop() {
  char str_t[8], str_ir[8], str_rd[8], str_ecg[8], str_bth[8], str_stp[8]; int i_round = 0;
      // Set array size
  String str_nz_ms = ""; String str_nz_ir = ""; String str_nz_rd = ""; String str_nz_ecg = ""; String str_nz_bth = ""; String data_store = ""; String data_now = "";
      // Reset data
  //----------------------------------//
  int delaySampling = 1;              //  <== Freq samplig is delaySampling * 4
  int nSampling = 14; int tti = 0;    //  <== Assembly data all sensors  nSampling times. Then, sent at round nSampling.
  //----------------------------------//
  digitalWrite(2,1);      // LED Red on
  digitalWrite(4,0);      // LED Green off
  if(digitalRead(35)==1){           // If press record button (For start)
    digitalWrite(2,0);              // LED Red off
    digitalWrite(4,1);              // LED Green on
    delay(1000);
    BTSerial.println(999);        Serial.println(999);        // Sent secret code to MATLAB for at here is ready!
    BTSerial.println(nSampling);  Serial.println(nSampling);  // Sent quantity sampling to set config at MATLAB
    long time_ms_i = millis();      // Initial time when start
    while(digitalRead(35)==LOW){    // Run until press record button agian
      i_round++;                    // Count round for compare nSampling
      long time_ms = millis()-time_ms_i;    // Keep now time in millisecond
      digitalWrite(18, 1);          // OXIMETER Infrared on
      delay(delaySampling);         // Rise time
      int ppg_ir = analogRead(33);  // Read photodiode sensor when Infrared on
      delay(delaySampling);
      digitalWrite(18, 0);          // OXIMETER Infrared off
      digitalWrite(19, 1);          // OXIMETER Red on
      delay(delaySampling);         // Rise time
      int ppg_red = analogRead(33); // Read photodiode sensor when Red on
      delay(delaySampling);
      digitalWrite(19, 0);          // OXIMETER Red off
      int ecg = analogRead(13);     // Read AD8232 sensor
      int bth = analogRead(34);     // Read Thermal sensor
      int stp = digitalRead(35);    // Read Record button (Stop)

      sprintf(str_t,"%lu",time_ms); sprintf(str_ir,"%d",ppg_ir); sprintf(str_rd,"%d",ppg_red); sprintf(str_ecg,"%d",ecg); sprintf(str_bth,"%d",bth); sprintf(str_stp,"%d",stp);
            // Replace data to each string
      //:::::::: Part count char and add "0" into string for amount is full enought :::::://
      for(int i_ms = 1; i_ms <= 8-(String(str_t).length()); i_ms++){                      //
        str_nz_ms = str_nz_ms + '0';                                                      //
      }                                                                                   //
      for(int i_ir = 1; i_ir <= 4-(String(str_ir).length()); i_ir++){                     //
        str_nz_ir = str_nz_ir + '0';                                                      //
      }                                                                                   //
      for(int i_rd = 1; i_rd <= 4-(String(str_rd).length()); i_rd++){                     //
        str_nz_rd = str_nz_rd + '0';                                                      //
      }                                                                                   //
      for(int i_ecg = 1; i_ecg <= 4-(String(str_ecg).length()); i_ecg++){                 //
        str_nz_ecg = str_nz_ecg + '0';                                                    //
      }                                                                                   //
      for(int i_bth = 1; i_bth <= 4-(String(str_bth).length()); i_bth++){                 //
        str_nz_bth = str_nz_bth + '0';                                                    //
      }                                                                                   //
      //:::::::::::::::::::::::::::::::: End part :::::::::::::::::::::::::::::::::::::::://
      String data_now = str_nz_ms + String(str_t) + str_nz_ir + String(str_ir) + str_nz_rd + String(str_rd) + str_nz_ecg + String(str_ecg) + str_nz_bth + String(str_bth) + String(str_stp);
      data_store = data_store + data_now;
            // Assembly all data only this round
      str_nz_ms = ""; str_nz_ir = ""; str_nz_rd = ""; str_nz_ecg = ""; str_nz_bth = "";
            // Reset all array
      if(i_round >= nSampling){       // If this round equal with nSampling
        BTSerial.println(data_store); Serial.println(data_store);                                             // Sent all data to laptop in wireless and wired system
        data_store = ""; str_nz_ms = ""; str_nz_ir = ""; str_nz_rd = ""; str_nz_ecg = ""; str_nz_bth = "";    // Reset all array
        i_round = 0;                                                                                          // Reset round
      }
      /*
      BTSerial.println(millis()-b);       
      BTSerial.println(ir);               
      BTSerial.println(red);              
      BTSerial.println(analogRead(13));   
      BTSerial.println(analogRead(27));   
      BTSerial.println(analogRead(34));   
      BTSerial.println(0);                
      BTSerial.println(digitalRead(35));
      */
      
//      if(digitalRead(35)==1){
//        BTSerial.println(999);
//        while(1){
//        digitalWrite(2,1);
//        digitalWrite(4,0);
//        delay(500);
//        digitalWrite(2,0);
//        digitalWrite(4,1);
//        delay(500);
//        }
//      }
    }
    BTSerial.println(999); Serial.println(999);     // Sent secret code to MATLAB for at here is stop!
    digitalWrite(2,1);      // LED Red on
    digitalWrite(4,1);      // LED Green on
    delay(2000);
    digitalWrite(2,0);      // LED Red off
    digitalWrite(4,0);      // LED Green off
    delay(1000);
  }
}
