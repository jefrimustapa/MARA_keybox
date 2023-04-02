#include <Arduino.h>
#include <Nextion.h>

//
char dataFromUI[4] = {0};

#define LED 2
#define SW_ON 1
#define SW_OFF 0

//page 0
NexButton btn_on_off = NexButton(0,3, "btn_on_off");
NexPicture p_led_on = NexPicture(0,5, "p_led_on");
NexPicture p_led_off = NexPicture(0,4, "p_led_off");
NexButton btn_pg1 = NexButton(0, 6, "b0");
NexPage page0 = NexPage(0, 0 , "page0");

//page 1
NexPage page1 = NexPage(1, 0 , "page1");
NexButton btn_pg0 = NexButton(1, 1, "b0");
NexScrolltext g0 = NexScrolltext(1, 2, "g0");

NexTouch * nex_listen_list[] = {&btn_on_off, &p_led_on, &p_led_off, &btn_pg1, &btn_pg0};

void btn_on_off_Release(void * ptr){
  char buf[10] = {0};
  btn_on_off.getText(buf, 10);
  printf("btn_on_off txt:%s\n", buf);
  if(strcmp(buf, "OFF")==0){
    digitalWrite(LED_BUILTIN, 1);
  }else{
    digitalWrite(LED_BUILTIN, 0);
  }
}


void btn_page1_Release(void * ptr){
  page1.show();
  g0.setText("This data is from ESP32..");
}

void btn_page0_Release(void * ptr){
  page0.show();

}



void setup() {  
  
  dbSerial.begin(115200);
  nexSerial.begin(9600);

  printf("nexInit() return:%d\n", nexInit());

  //Serial.begin(115200);
  printf("\nin setup\n");

  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, GPIO_MODE_INPUT_OUTPUT);
  //Serial.begin(115200);
  //Serial2.begin(9600); //comm to HMI , TX2:GPIO17, RX2:GPIO16
  btn_on_off.attachPop(btn_on_off_Release, &btn_on_off);
  btn_pg1.attachPop(btn_page1_Release, &btn_pg1);
  btn_pg0.attachPop(btn_page0_Release, &btn_pg0);
  
}



void loop() {
  

  //handle data from HMI ---> ESP
  // printf("Serial2.available:%d\n", Serial2.available() );
  // while(Serial2.available() > 0){
  //   Serial2.readBytesUntil('\n', dataFromUI, 4);
  //   printf("dataFromUI[0]: %d\n", dataFromUI[0]);
  //   if(dataFromUI[0] == SW_ON)
  //     digitalWrite(LED_BUILTIN, 1);
  //   else
  //     digitalWrite(LED_BUILTIN, 0);
  // }

  nexLoop(nex_listen_list);
}