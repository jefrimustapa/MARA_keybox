#include <Arduino.h>
#include <Nextion.h>
#include <WiFi.h>

#pragma region "HMI"
//--------------------------VARIABLES-----------------------------
// page 0
NexButton btn_on_off = NexButton(0, 3, "btn_on_off");
NexPicture p_led_on = NexPicture(0, 5, "p_led_on");
NexPicture p_led_off = NexPicture(0, 4, "p_led_off");
NexButton btn_pg1 = NexButton(0, 6, "b0");
NexPage page0 = NexPage(0, 0, "page0");

// page1
NexPage page1 = NexPage(1, 0, "page1");
NexButton btn_pg0 = NexButton(1, 1, "b0");
NexScrolltext g0 = NexScrolltext(1, 2, "g0");
NexTouch *nex_listen_list[] = {&btn_on_off, &p_led_on, &p_led_off, &btn_pg1, &btn_pg0};

//--------------------------CALLBACKS-----------------------------
void btn_on_off_Release(void *ptr)
{
  char buf[10] = {0};
  btn_on_off.getText(buf, 10);
  printf("btn_on_off txt:%s\n", buf);
  if (strcmp(buf, "OFF") == 0)
  {
    digitalWrite(LED_BUILTIN, 1);
  }
  else
  {
    digitalWrite(LED_BUILTIN, 0);
  }
}

void btn_page1_Release(void *ptr)
{
  page1.show();
  g0.setText("This data is from ESP32..");
}

void btn_page0_Release(void *ptr)
{
  page0.show();
}
#pragma endregion

#pragma region "ESP32"
//--------------------------DEFINE-----------------------------
// char dataFromUI[4] = {0};
#define LED 2
#define SW_ON 1
#define SW_OFF 0

//--------------------GLOBAL-VARIABLES-------------------------

void setup()
{

  dbSerial.begin(115200);
  nexSerial.begin(9600);
  //Serial.begin(115200);
  //Serial2.begin(9600); //comm to HMI , TX2:GPIO17, RX2:GPIO16
  //printf("nexInit() return:%d\n", nexInit());
  bool bNex = nexInit();
  printf("\nin setup\n");

  pinMode(LED_BUILTIN, GPIO_MODE_INPUT_OUTPUT);
 

  // HMI setup
  btn_on_off.attachPop(btn_on_off_Release, &btn_on_off);
  btn_pg1.attachPop(btn_page1_Release, &btn_pg1);
  btn_pg0.attachPop(btn_page0_Release, &btn_pg0);

  // wifi setup
  WiFi.begin();
  WiFi.mode(WIFI_STA); // set to station mode
  WiFi.disconnect();   // disconnect from AP.
}

int scanWifi()
{
  printf("scanning WiFi AP\n");
  int nAP = WiFi.scanNetworks();

  printf("scan complete, nAP: %d found\n", nAP);
  if (nAP > 0)
  {
    for (int i = 0; i < nAP; i++)
    {
      printf("SSID: %15s, ", WiFi.SSID(i).c_str());
      printf("RSSI: %3d, ", WiFi.RSSI(i));
      printf("Encryption: ");
      switch (WiFi.encryptionType(i))
      {
      case WIFI_AUTH_OPEN:
        printf("Open");
        break;
      case WIFI_AUTH_WEP:
        printf("WEP");
        break;
      case WIFI_AUTH_WPA_PSK:
      case WIFI_AUTH_WPA2_PSK:
      case WIFI_AUTH_WPA2_WPA3_PSK:
      case WIFI_AUTH_WPA3_PSK:
        printf("WPA/WPA2/WPA3");
        break;
      default:
        printf("Others, %d", WiFi.encryptionType(i));
        break;
      }
      printf("\n");
    }
  }

  delay(4000);

  return nAP;
}

void loop()
{

  printf("in main loop\n");
   // wifi scanning
  scanWifi();

  //nexLoop(nex_listen_list);

 
  
}
#pragma endregion