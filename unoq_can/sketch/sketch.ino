#include <CAN.h>
#include <Arduino_RouterBridge.h>

//Create defines
#define STM32_CAN_ID 0x446
#define UNOQ_CAN_ID 0x101

String toBinary(uint32_t value, int bits);
void onCanMessage(CanFDMsg const &msg, void *user_data);

String bSOF, bID, bRTR, bIDE, bR, bDLC, bDATA, bCRC, bACK, bEOF;
/************
NOTE: Arduino Zephyr CAN Library:

CanMsg: can_id, can_data_len, can_data

begin() - Starts controller in classic CAN mode
beginFD() - Starts controller in newer CAN FD mode

addReceiveFilter(can_id, mask, extended(boolean)) 


*************/

void setup()
{
  Monitor.begin();
  Bridge.begin();


  //CAN Filters                     can_id, mask(std 11bit), extended
  int filterId = CAN.addReceiveFilter(STM32_CAN_ID, 0x7FF, false);

  Monitor.println("UNO Q CAN Receiver starting...");

  //Start Classic CAN at 500 kbit/s
  if (!CAN.begin(CanBitRate::BR_500k))
  {
    Monitor.println("ERROR: CAN failed to start!");

    while (1)
    {
      //Stop here if CAN initialization failed
    }
  }

  //Setup CAN Rx Callback
  CAN.onReceive(onCanMessage);

  Monitor.println("CAN started successfully at 500 kbit/s");
  Monitor.println("Waiting for CAN frames...");

  
}

void loop()
{
  
}


//Convert ints to binary string
String toBinary(uint32_t value, int bits){

  String result = "";
  
  for (int i = bits - 1; i >= 0; i--){


    //Shift the binary i places and then AND it with 1 to mask the bit.
    uint8_t bit = (value >> i) & 1;

    if (bit == 1){
      result += '1';
    }
    else{
      result += '0';
    }
    
  }

  return result;
  
}

//CAN Callback on Rx
void onCanMessage(CanFDMsg const &msg, void *user_data){

    (void)user_data;

  
    
    Monitor.print("RX: ");
    Monitor.println(msg);

    //Print CAN Frame (Regular)
    Monitor.print("CAN FRAME: ID=0x");
    Monitor.print(msg.getStandardId(), HEX);
    
    Monitor.print(" RTR=0");
    
    Monitor.print(" IDE=");
    Monitor.print(msg.isStandardId() ? 0 : 1);
    
    Monitor.print(" r0=0");
    
    Monitor.print(" DLC=");
    Monitor.print(msg.data_length);
    
    Monitor.print(" Data=");
    
    for (int i = 0; i < msg.data_length; i++)
    {
        Monitor.print("0x");
        Monitor.print(msg.data[i], HEX);
        Monitor.print(" ");
    }
    
    //Print CAN Frame (Binary)
    Monitor.print("CAN FRAME: ");
    
    Monitor.print("SOF=");
    Monitor.print("0");
    
    Monitor.print(" ID=");
    Monitor.print(toBinary(msg.getStandardId(), 11));
    
    Monitor.print(" RTR=");
    Monitor.print("0");
    
    Monitor.print(" IDE=");
    if (msg.isStandardId())
    {
        Monitor.print("0");
    }
    else
    {
        Monitor.print("1");
    }
    
    Monitor.print(" r0=");
    Monitor.print("0");
    
    Monitor.print(" DLC=");
    Monitor.print(toBinary(msg.data_length, 4));
    
    Monitor.print(" DATA=");
    
    for (int i = 0; i < msg.data_length; i++)
    {
        Monitor.print(toBinary(msg.data[i], 8));
    
        if (i < msg.data_length - 1)
        {
            Monitor.print(" ");
        }
    }
    
    Monitor.println();

    //Convert to Binary (Pass through to Python MPU)
    //*****************
    bSOF = "0";
    bID = toBinary(msg.getStandardId(), 11);
    bRTR = "0";
    bIDE =(msg.isStandardId()) ? "0" : "1";
    bR = "0";
    bDLC = toBinary(msg.data_length, 4);
    bDATA = "";
    for (int i = 0; i < msg.data_length; i++){
      bDATA += toBinary(msg.data[i], 8); 
    }
    

    //notify <> call - call function without awaiting response
    Bridge.notify("printBinaryPY", bSOF, bID, bRTR, bIDE, bR, bDLC, bDATA);
    

    //*************************
    //Send data back to STM32
    //
    uint8_t txData[2] = {
      0x4F,
      0x4B
    };
    CanMsg txMsg(CanStandardId(UNOQ_CAN_ID), sizeof(txData), txData);
    int result = CAN.write(txMsg);
    
    if (result == 1)
    {
      Serial.println("[CAN TX] ");
      Serial.println(txMsg);
    }
    else
    {
      Serial.println("[CAN TX ERROR] result = ");
      Serial.println(result);
    }
  
  
}