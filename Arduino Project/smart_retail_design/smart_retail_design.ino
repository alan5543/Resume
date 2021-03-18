#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "rebate.h"
#include "logo.h"
#include "WiFiEsp.h"
#include "ThingSpeak.h"

char ssid[] = "EE3070_P1800_1";    //  your network SSID (name) 
char pass[] = "EE3070P1800";   // your network password
int keyIndex = 80;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
//SoftwareSerial Serial1(6, 7); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = 1328000;
const char * myWriteAPIKey = "I70YOC72X60M4UHH";

#define SCREEN_WIDTH 128 // OLED
#define SCREEN_HEIGHT 64 // OLED
//Adafruit_SSD1306 display(-1);//OLED

//set for OLED
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
boolean pay_screen = true;

//set for RFID
#define SS_PIN 53
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

//set for button and LED
# define BUTTON_PIN 35  // 按鍵的接腳
int buttonState = 0;   // 按鈕的狀態
int debounceDelay = 200; // debounce delay (ms)
unsigned long lastMillis; // record last millis
#define LED_1 3 //define 1st LED pin
#define LED_2 8 //define 2nd LED 6->8
#define LED_3 9 //define 3rd LED 7->9

//define product price
double product_p1 = 0;
double product_p2 = 0;
double product_p3 = 0;
byte p1[4];
byte p2[4];
byte p3[4];
int product_no = 1;

//set for key and access card
int kcount = 0;
boolean start = true;
boolean start_pay = false;
boolean used_Rebate = false;
boolean view_mote = false;
boolean recog_first_key = false;

//set for product card
double total = 0;
double discount_total = 0;
double discount_rate = 0;
double save_money = 0;
int counter_1 = 0;
int counter_2 = 0;
int counter_3 = 0;

//set for rebate system
rebate rlist[10];
int rpoint = 0;
int rvalue = 5;

int i = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(ESP_BAUDRATE);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  //Set for Button and LED
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  close_led();

  // 偵測是否安裝好OLED了
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();

  //Add the card 1 and set the price
  Serial.println(F("Scan Product 1:"));//***OLED:screen_product1
  scanProduct(i);
  add_card(p1);
  read_price(product_p1);

  //Add the card 2 and set the price
  Serial.println(F("Scan Product 2:")); //***OLED:screen_product2
  scanProduct(i);
  add_card(p2);
  read_price(product_p2);

  //Add the card 3 and set the price
  Serial.println(F("Scan Product 3:")); //***OLED:screen_product3
  scanProduct(i);
  add_card(p3);
  read_price(product_p3);
  Serial.println(F("Finishing Setting. Can start shopping now.")); //***OLED:screen_shopping
  

}

void loop() {

  // put your main code here, to run repeatedly:
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("EE3070_P1800_1");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

Restart:

  if(pay_screen){
    payment_method();
    pay_screen = false;
  }

  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == LOW && kcount == 2){
  //agree the use of rebate
    if(debounced() && digitalRead(BUTTON_PIN) == LOW){
      if(rpoint == 0){
        Serial.println("You don't have any rebate points to use. Please pay it directly and earn rebate. ");//***OLED:error_rebate
        error_rebate();
      }
      else{
        //used the rebate now
        used_Rebate = true;
        use_rebate(nuidPICC[0],nuidPICC[1],nuidPICC[2],nuidPICC[3]);
        discount_total = discount_total - rpoint*rvalue;
        
        //avoid long press of the button
        while(digitalRead(BUTTON_PIN) == LOW);
  
        //display
        pointrefund(rpoint);
        delay(2000);
        after_rebate();
        three_blink();
        Serial.print("You have used ");
        Serial.print(rpoint);
        Serial.println(" points of rebate.");
        Serial.print("The total is reduced by $");
        Serial.println(rpoint*rvalue);
        Serial.println();
      }
    }
    
  }
  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));


  
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  show_rebate();

  boolean is_product = (rfid.uid.uidByte[0] == 156 && rfid.uid.uidByte[1] == 37 && rfid.uid.uidByte[2] == 20 && rfid.uid.uidByte[3] == 48)||
                       (rfid.uid.uidByte[0] == p2[0] && rfid.uid.uidByte[1] == p2[1] && rfid.uid.uidByte[2] == p2[2] && rfid.uid.uidByte[3] == p2[3]) ||
                       (rfid.uid.uidByte[0] == p1[0] && rfid.uid.uidByte[1] == p1[1] && rfid.uid.uidByte[2] == p1[2] && rfid.uid.uidByte[3] == p1[3])||
                       (rfid.uid.uidByte[0] == p3[0] && rfid.uid.uidByte[1] == p3[1] && rfid.uid.uidByte[2] == p3[2] && rfid.uid.uidByte[3] == p3[3]);

  boolean is_key = rfid.uid.uidByte[0] == nuidPICC[0] && rfid.uid.uidByte[1] == nuidPICC[1] &&
                    rfid.uid.uidByte[2] == nuidPICC[2] && rfid.uid.uidByte[3] == nuidPICC[3];

  if(!recog_first_key){
    reset(); //reset all the variable
    if(!is_product){
      //Recognize the key card first
      recog_first_key = true;
      kcount++;
      
      //OLED Action
      // 顯示Adafruit的LOGO，算是開機畫面
      display.display();
      delay(1000); // 停1秒
      // 清除畫面
      display.clearDisplay();
  
      testdrawstyles();    // 測試文字
      testdrawbitmap();    // 顯示圖形
      
      if(start == true){
        //record the first key card
        Serial.println(F("A key card has been detected."));
        first_LED();
        
        // Store NUID into nuidPICC array (Store Key Card)
        for (byte i = 0; i < 4; i++) {
          nuidPICC[i] = rfid.uid.uidByte[i];
        }
  
        Serial.print(F("The Key Card ID is: "));
        printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
        Serial.print(F("The total is $"));
        Serial.print(total);
        Serial.println();
        
        start = false; 
      }
    }
    else{
      //Not key card type
      Serial.println(F("Wrong Key Card Type."));                //*** OLED:error_wrong_card_type
      error_wrong_card_type();
      Serial.println(F("Please Scan your payment card."));      //***
    }
  }
  else{
    //Have recognized the key card already
    
    
    if(rfid.uid.uidByte[0] == nuidPICC[0] &&rfid.uid.uidByte[1] == nuidPICC[1] && 
      rfid.uid.uidByte[2] == nuidPICC[2] && rfid.uid.uidByte[3] == nuidPICC[3]){   //key card
        kcount++;

      if(start == false){
        if(kcount == 2){
          //asking use rebate or not?
          start_pay = true;
          rpoint = search_rebate(nuidPICC[0],nuidPICC[1],nuidPICC[2],nuidPICC[3]);
          Serial.print("You have ");
          Serial.print(rpoint);
          Serial.print(" rebate points in your card.");
          Serial.println();
          Serial.println(F("Use rebate or not?"));
          Serial.println(F("If use, Press the button."));
          Serial.println(F("If not, Scan the card to pay."));
          Serial.println();
          refundtable();
          three_LED_on();
        }
        if(kcount == 3){
          //calculate rebate
          int rp = cal_rebate(discount_total);
          add_rebate(nuidPICC[0],nuidPICC[1],nuidPICC[2],nuidPICC[3],rp);
          
          //finishing payment
          Serial.print(F("Please pay $"));
          Serial.print(discount_total);       //total payment
          Serial.println();
          Serial.print(F("You have earned "));
          Serial.print(rp);
          Serial.print(F(" point of rebate. "));
          Serial.print(F("Thank you."));
          int ans = discount_total;
          ThingSpeak.writeField(myChannelNumber, 1, ans, myWriteAPIKey);
          delay(20000);

          //OLED
          finish(rp);
          delay(2000);
          three_blink();
          
          start_pay = false; //reset
          start = true;
          used_Rebate = false;
          recog_first_key = false;
          pay_screen = true;
          kcount == 0;
          counter_1 = 0;
          counter_2 = 0;
          counter_3 = 0;
          
          //thank you
          thankyou();
          delay(1000);

          //restart shopping
          turn_off();
          payment_method();
        }
      }
      
    }
    
  
    if(!start && !is_key){
      if(rfid.uid.uidByte[0] == 156 &&     //access key (Blue tag = 156 225 39 23)
      rfid.uid.uidByte[1] == 37 &&          //1.do check list
      rfid.uid.uidByte[2] == 20 &&        //2.do remove function
      rfid.uid.uidByte[3] == 48){
        if(!used_Rebate){ //Once used Rebate, cant flow back to change
          view_mote = !view_mote;
          kcount = 1; //for backward
          start_pay = false;
          middle_LED_Blink();
          
          if(view_mote){                        //*careful to the admin mote
            //show the list of product
            Serial.println(F("Get into the view mote"));
            Serial.println(F("You can remove the product if want."));
            Serial.println();
            showList(counter_1,counter_2,counter_3);
          }
          else if (!view_mote){
            //Back to normal mote
            Serial.println(F("Back to the normal mote"));
            Serial.println(F("You can add the product if want."));
            Serial.println();
            showamount(total);
          }
        }
        else{
          //You cannot change the product after start rebate
          Serial.println(F("You cannot change the product after doing rebate.")); //***OLED:error_no_change
          error_no_change();
        }
      }
      else if(rfid.uid.uidByte[0] == p2[0] && //alan's ID: 2D 36 8B 98
      rfid.uid.uidByte[1] == p2[1] && 
      rfid.uid.uidByte[2] == p2[2] && 
      rfid.uid.uidByte[3] == p2[3]){
        if(!start_pay){
            if(!view_mote){
              //add product
              total = total + product_p2;
              counter_2 ++;
              call_discount();
              Serial.println(F("Add Product 2 from cart"));
              //OLED
              addBlueProd();
              showamount(total);
            }
            else if(view_mote){
              //remove product
              total = total - product_p2;
              counter_2 --;
              call_discount();
              Serial.println(F("Remove Product 2 from cart"));
              negative_handle();
              //OLED
              //removeBlueProd();
              removeProd(2);
              showList(counter_1,counter_2,counter_3);
            }
//            //common action
//            discount_rate = cal_discount(total);
//            discount_total = cal_dis_total(total, discount_rate);
//            save_money = total - discount_total;
            //display
            middle_LED_Blink();
            serial_display();
            view_product();
          }
          else{
            //no product can get in when going payment
            Serial.println(F("Cannot add thing when doing payment")); //***OLED:error_no_add
            error_no_add();
          }
      }
      else if(rfid.uid.uidByte[0] == p1[0] && //jack's ID: FD CA 88 98 (253 202 136 152)
      rfid.uid.uidByte[1] == p1[1] && 
      rfid.uid.uidByte[2] == p1[2] && 
      rfid.uid.uidByte[3] == p1[3]){
          if(!start_pay){
            if(!view_mote){
              //add product
              total = total + product_p1;
              counter_1 ++;
              call_discount();
              Serial.println(F("Add Product 1 from cart"));
              //OLED
              addRedProd();
              showamount(total);
            }
            else if(view_mote){
              //remove product
              total = total - product_p1; 
              counter_1 --;
              call_discount();
              Serial.println(F("Remove Product 1 from cart"));
              negative_handle();
              //OLED
              //removeRedProd();
              removeProd(1);
              showList(counter_1,counter_2,counter_3);
            }
//            //common action
//            discount_rate = cal_discount(total);
//            discount_total = cal_dis_total(total, discount_rate);
//            save_money = total - discount_total;
            //display
            middle_LED_Blink();
            serial_display();
            view_product();
          }
          else{
            //no product can get in when going payment
            Serial.println(F("Cannot add thing when doing payment")); //***OLED:error_no_add
            error_no_add();
          }
      }
      else if(rfid.uid.uidByte[0] == p3[0] && //ray's ID: BD 60 89 98
      rfid.uid.uidByte[1] == p3[1] && 
      rfid.uid.uidByte[2] == p3[2] && 
      rfid.uid.uidByte[3] == p3[3]){
          if(!start_pay){
            if(!view_mote){
              //add product
              total = total + product_p3;
              counter_3 ++;
              call_discount();
              Serial.println(F("Add Product 3 from cart"));
              //OLED
              addGreenProd();
              showamount(total);
            }
            else if(view_mote){
              //remove product
              total = total - product_p3; 
              counter_3 --;
              call_discount();
              Serial.println(F("Remove Product 3 from cart"));
              negative_handle();
              //OLED
              //removeGreenProd();
              removeProd(3);
              showList(counter_1,counter_2,counter_3);
            }
//            //common action
//            discount_rate = cal_discount(total);
//            discount_total = cal_dis_total(total, discount_rate);
//            save_money = total - discount_total;
            //display
            middle_LED_Blink();
            serial_display();
            view_product();
          }
          else{
            //no product can get in when going payment
            Serial.println(F("Cannot add thing when doing payment")); //***OLED:error_no_add
            error_no_add();
          }
      }
      else{
          //other inrelavent product
          //return Wrong Product
          Serial.println(F("The product was not recorded in our shop. Sorry.")); //***OLED:error_no_record
          error_no_record();
      }
    }

  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

//************************************************************************************
//Function Area
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void close_led(void){
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
}

double cal_discount(double total){
    if(total >= 300 & total <= 500){
      return 0.1;
    }
    else if(total >= 500 && total <= 1000){
      return 0.25;
    }
    else if(total >= 1000){
      return 0.35;
    }
    else{
      return 0;
    }
}

void add_card(byte p[4]){
  while(1){
  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    for (byte i = 0; i < 4; i++) {
      p[i] = rfid.uid.uidByte[i];
      Serial.println(p[i]);
    }
    break;
    }
  }
  for(byte i = 0; i < 4; i++){
    Serial.print(p[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void read_price(double &x){ 
  Serial.println();
  Serial.print("Enter the Price of Product "); //*** OLED:screen_enter_price
  screen_enter_price();
  Serial.print(product_no);
  Serial.println(": ");
  String temp = "";
  while(1){
    if(Serial.available() > 0){
      temp = Serial.readString();
      x = temp.toDouble();
      break;
    }
  }
  Serial.print("Product ");
  Serial.print(product_no);
  Serial.print(" = $");
  Serial.println(x);
  product_no++;
  Serial.println();
}

void call_discount(void){
    discount_rate = cal_discount(total);
    discount_total = cal_dis_total(total, discount_rate);
    save_money = total - discount_total;
}

double cal_dis_total(double total, double discount_rate){
    double temp = total * (1-discount_rate);
    return temp;
}

void view_product(void){
  Serial.print(F("Number of Red Product: "));
  Serial.println(counter_1);
  Serial.print(F("Number of Blue Product: "));
  Serial.println(counter_2);
  Serial.print(F("Number of Green Product: "));
  Serial.println(counter_3);
  Serial.println("**********************************");
  Serial.println();
}

void serial_display (void){
    Serial.print(F("The total is $"));
    Serial.print(total);
    Serial.println();

    Serial.print(F("The discount rate is "));
    Serial.print(discount_rate*100);
    Serial.print("% off.");
    Serial.println();
    
    Serial.print(F("The discount total is $"));
    Serial.print(discount_total);
    Serial.println();

    Serial.print(F("The money saved is $"));
    Serial.print(save_money);
    Serial.println();
    Serial.println();
    
}

void reset(){
  total = 0;
  discount_total = 0;
  discount_rate = 0;
  save_money = 0;
  counter_1 = 0;
  counter_2 = 0;
  counter_3 = 0;
  kcount = 0;
}

void negative_handle(){
  int temp = 0;
  boolean thres = false;
  if(counter_1 < 0){ //$50
    temp = -1*counter_1;
    total = total + temp*product_p1;
    call_discount();
    counter_1 = 0;
    thres = true;
  }
  if(counter_2 < 0){ //$100
    temp = -1*counter_2;
    total = total + temp*product_p2;
    call_discount();
    counter_2 = 0;
    thres = true;
  }
  if(counter_3 < 0){ //$200
    temp = -1*counter_3;
    total = total + temp*product_p3;
    call_discount();
    counter_3 = 0;
    thres = true;
  }
  if(thres == true){
    Serial.println();
    Serial.println(F("It is already in minimum. Can't Remove."));
    Serial.println();
    error_minimum();
  }
}

int search_rebate(byte r1, byte r2, byte r3, byte r4){
  for(byte i=0; i<sizeof(rlist)/sizeof(rlist[0]);i++){
    rebate temp = rlist[i];
    if(temp.firstbyte() == r1 && temp.secondbyte() == r2 && temp.thirdbyte() == r3 && temp.forthbyte() == r4){
      return temp.getPoint();
    }
  }
  return 0;
}

void use_rebate(int r1, int r2, int r3, int r4){
  for(int i=0; i<sizeof(rlist)/sizeof(rlist[0]);i++){
    rebate temp = rlist[i];
    if(temp.firstbyte() == r1 && temp.secondbyte() == r2 && temp.thirdbyte() == r3 && temp.forthbyte() == r4){
      rlist[i].reduce_point();
      break;
    }
  }
}

void add_rebate(int r1, int r2, int r3, int r4, int pt){
  Serial.println("In the add search");
  boolean found = false;
  for(int i=0; i<sizeof(rlist)/sizeof(rlist[0]);i++){
    rebate temp = rlist[i];
    if(temp.firstbyte() == r1 && temp.secondbyte() == r2 && temp.thirdbyte() == r3 && temp.forthbyte() == r4){
      Serial.println("Found");
      rlist[i].add_point(pt);
      found = true; //find the same card in the list
      break;
    }
  }
  if(!found){  //didnt find the card in the list, so add the new card to the list
    for(int i = 0; i<sizeof(rlist)/sizeof(rlist[0]);i++){
      rebate temp = rlist[i];
      if(temp.firstbyte() == 0 && temp.secondbyte() == 0 && temp.thirdbyte() == 0 && temp.forthbyte() == 0){
        rlist[i] = rebate(r1, r2, r3, r4, pt);
        break;
      }
    }
  }
}

int cal_rebate(double all){
  int x = all/100;
  return x;
}

void reset_rebate(void){
  for(int i=0;i<sizeof(rlist)/sizeof(rlist[0]);i++){
    rlist[i] = rebate(0,0,0,0,0);
  }
}

void show_rebate(void){
  for(int i=0;i<sizeof(rlist)/sizeof(rlist[0]);i++){
    Serial.print(rlist[i].firstbyte());
    Serial.print(", ");
    Serial.print(rlist[i].secondbyte());
    Serial.print(", ");
    Serial.print(rlist[i].thirdbyte());
    Serial.print(", ");
    Serial.print(rlist[i].forthbyte());
    Serial.print(", ");
    Serial.print(rlist[i].getPoint());
    Serial.println();
  }
}

//For button Design
boolean debounced() { // check if debounced
  unsigned long currentMillis = millis(); // get current elapsed time
  if ((currentMillis - lastMillis) > debounceDelay) {
    lastMillis = currentMillis; // update lastMillis with currentMillis
    return true; // debounced
  }
  else {return false;} // not debounced
}

//************************************************************************************
//Display Area
void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(2);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("WELCOME!");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}

void testdrawbitmap(void) {
  //display.clearDisplay();
  //顯示圖形，x,y,圖形文字,寬,高,1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.drawBitmap(0,16,logo_bmp, 128, 48, 1);
  display.display();  // 要有這行才會把文字顯示出來
  delay(1000);
}

void showList(int counter_1, int counter_2, int counter_3){
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Red product: ");
display.println(counter_1);
display.setCursor(0, 20);
display.print("Blue product: ");
display.println(counter_2);
display.setCursor(0, 40);
display.print("Green product: ");
display.println(counter_3);
display.display();
delay(1000);  
}

void showamount(double total) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.print("Total Amount: $");
  display.println(discount_total);
  display.drawLine(0,9,128,9,1);
  display.setCursor(0, 16);
  display.print("Discount: ");
  display.println(discount_rate*100);
  display.setCursor(90, 16);
  display.print("% off");
  display.setCursor(0, 32);
  display.print("Origin Price: $");
  display.println(total);
  display.setCursor(0, 48);
  display.print("Saved: $");
  display.println(save_money);
  display.display();
  delay(1000); 
}

void addRedProd(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Red product is added to the cart.");
display.display();
delay(1000);
}

void addBlueProd(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(2);
display.setCursor(0, 0);
display.print("Blue product is added to the cart.");
display.display();
delay(1000);
}

void addGreenProd(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Green product is added to the cart.");
display.display();
delay(1000);
}

void removeRedProd(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Red product is removed from the cart.");
display.display();
delay(1000);
}

void removeBlueProd(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Blue product is removed from the cart.");
display.display();
delay(1000);
}

void removeGreenProd(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Green product is removed from the cart.");
display.display();
delay(1000);
}

void removeProd(int x){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(0, 0);
  switch(x){
    case 1:
      display.print("Red product is removed from the cart.");
      break;
    case 2:
      display.print("Blue product is removed from the cart.");
      break;
    case 3:
      display.print("Green product is removed from the cart.");
      break;
    default:
      display.print("Error :D");
      break;
  }
  display.display();
  delay(1000);
}

void thankyou(void){
   // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Clear the buffer.
  display.clearDisplay();

// Display Text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.display();
  delay(100);
  display.clearDisplay();

  // Scroll full screen
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Thankyou");
  display.println("And");
  display.println("Welcome!");
  
  display.display();
  delay(2000);
  display.startscrollright(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  display.clearDisplay();
}

void refundtable(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(12, 10);
  display.println(discount_rate*100);
  display.setCursor(42, 10);
  display.print("%");
  display.setCursor(22, 20);
  display.print("OFF");
  display.drawLine(0,32,64,32,1);
  display.drawLine(64,0,64,64,1);
  display.setCursor(0, 40);
  display.println("You have ");
  display.setCursor(0, 50);
  display.print(rpoint);
  display.setCursor(10, 50);
  display.print("pts,use?");
  display.setCursor(90, 22);
  display.print("PAY");
  display.setCursor(80, 42);
  display.println(discount_total);
  display.display();
  delay(2000); 
}

void after_rebate(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(12, 10);
  display.println(discount_rate*100);
  display.setCursor(42, 10);
  display.print("%");
  display.setCursor(22, 20);
  display.print("OFF");
  display.drawLine(0,32,64,32,1);
  display.drawLine(64,0,64,64,1);
  display.setCursor(0, 40);
  display.println("NO");
  display.setCursor(10, 50);
  display.print("REBATE USE");
  display.setCursor(90, 22);
  display.print("PAY");
  display.setCursor(80, 42);
  display.println(discount_total);
  display.display();
  delay(2000); 
}

void pointrefund(int pt){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Rebate bk: $");
  display.println(pt*5);
  display.setCursor(0, 20);
  display.print("Consume ");
  int x = pt;
  display.println(x);
  display.setCursor(55, 20);
  display.print(" points rebate.");
  display.display();
  delay(1000); 
}

void finish(int pt){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Earn Rebate: ");
  display.println(pt);
  display.setCursor(0, 20);
  display.print("Consume $");
  int x = discount_total;
  display.println(x);
  display.setCursor(55, 20);
  display.print("");
  display.display();
  delay(1000); 
}

void payment_method(){    //*** OLED:The words are too big. Can make it smaller a bit
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.print("Payment Method");
  display.drawLine(0,10,128,10,1);
  display.setCursor(0, 20);
  display.print("1.Cash");
  display.setCursor(0, 30);
  display.print("2.Octopus");
  display.setCursor(0, 40);
  display.print("3.Visa");
  display.setCursor(0, 50);
  display.print("4.MasterCard");
  display.display();
  delay(1000); 
}

void scanProduct(int &i) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.print("Scan Product ");
  display.println(i);
  display.display();
  delay(1500); 
  if(i==4){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Finishing Setting. Can start shopping now.");
    display.display();
    delay(2000); 
  }
  i++;
}

void error_rebate(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("You don't have any rebate points to use. Please pay it directly and earn rebate.");
  display.display();
  delay(1500); 
}

void error_wrong_card_type(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Wrong Key Card Type.");
  display.print("Please Scan your payment card.");
  display.display();
  delay(1500); 
}

void error_no_change(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("You cannot change the product after doing rebate.");
  display.display();
  delay(1500); 
}

void error_no_add(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Cannot add thing when doing payment.");
  display.display();
  delay(1500); 
}

void error_no_record(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("The product was not recorded in our shop. Sorry.");
  display.display();
  delay(1500); 
}

void screen_enter_price(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Enter the price of product.");
  display.display();
  delay(1500); 
}

void error_minimum(){
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(1);
display.setCursor(0, 0);
display.print("Already in minimum. Can't Remove");
display.display();
delay(1000);
}
