#include <basic811.h>   //for  LM3S811EVB

#define BUTTON_NUM 1
const int buttonPin[ BUTTON_NUM] = {PUSH1}; // 14/pc4 the number of the pushbutton pin,  SW1, SW2, SW3, SW4

#define LED_NUM 2
#define LED LED_BUILTIN
const int ledPin[LED_NUM]    = {LED1, LED2};   //13/pc5 48/pd7 the number of the LED pin, LED1, LED3, LED5, LED7

String  inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {
  SERx.begin(9600);
  SERx.print(F("BasicDuino V"));
  SERx.print( ARDUINO_BASIC_VAR);
  SERx.print(F(" By huaweiwx@sina.com\r\n>"));
  for (int i = 0; i < LED_NUM; i++)
    pinMode(ledPin[i], 1);
  inputString.reserve(32);
  ubasic_setup();
}

extern uint8_t  prog_ram[];
void loop() {
  int32_t val;
  int16_t adr;
  uint8_t v;
  if (stringComplete) {
    digitalWrite(LED, LOW);  //busy
    ubasic_init(inputString.c_str());
    switch (tokenizer_token())
    {
      case TOKENIZER_LIST:
        list_statement();
        break;
      case TOKENIZER_LOAD:
        load_statement();
        break;
      case TOKENIZER_SAVE:
        save_statement();
        break;
      case TOKENIZER_DIR:
        dir_statement();
        break;
      /*     case TOKENIZER_CONTINU:*/
      case TOKENIZER_RUN:
        ubasic_run();
        break;
      case TOKENIZER_NEW:
        prog_ram[0] = '\0';
        break;
      case TOKENIZER_SYS:
      case TOKENIZER_SET:
      case TOKENIZER_PINMODE:
      case TOKENIZER_DIGITALTOGGLE:
      case TOKENIZER_ANALOGWRITE:
      case TOKENIZER_DIGITALWRITE:
      case TOKENIZER_PRINT:
      case TOKENIZER_LET:
      case TOKENIZER_LCD:
      case TOKENIZER_CSR:
      case TOKENIZER_CLS:
      case TOKENIZER_EEWR:
        statement();
        break;
      case TOKENIZER_LEFTPAREN:
      case TOKENIZER_FUNC:     //eerd(adr) peek(adr) err() adc(cn) getbit(v,b)
      case TOKENIZER_NUMBER:
      case TOKENIZER_HEXNUMBER:
        val = expr();
        SERx.print(val);
        SERx.print(F("(0x"));
        SERx.print(val, 16);
        SERx.println(")");
        break;
      case TOKENIZER_VARIABLE:
        v = tokenizer_variable_num();
        accept(TOKENIZER_VARIABLE);
        adr = isArray();
        if (tokenizer_token() == TOKENIZER_EQ) //let
        {
          accept(TOKENIZER_EQ);
          val = expr();
          accept(TOKENIZER_CRLF);
          if (adr >= 0) {
            arrayWrite(v, adr, val);
          } else {
            ubasic_set_variable(v, val);
          }
        } else { // not let
          if (adr >= 0) {
            val = arrayRead(v, adr);
          } else {
            val = ubasic_get_variable(v);
          }
        };
        SERx.print(val);
        SERx.print(F("(0x"));
        SERx.print(val, 16);
        SERx.println(")");
        break;
      case TOKENIZER_CRLF:
        break;
      default:
        // printf_P(PSTR("ErrCmd: %d\n"), tokenizer_token());
        break;
    } //end switch
    inputString = "";
    stringComplete = false;
    SERx.print(F("\n>"));
    digitalWrite(LED, HIGH);
  } //endif
  serialEvent();
}

void serialEvent() {
  while (SERx.available()) {
    char inChar = (char)SERx.read();
    inputString += inChar;
    if ((inChar == '\r') || (inChar == '\n')) {
      stringComplete = true;
    }
  }
}


