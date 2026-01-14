#include <LiquidCrystal.h>
#include <Keypad.h>
#include <math.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {0,1,2,3};
byte colPins[COLS] = {4,5,6,7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(8,9,10,11,12,13);

// ===== ENGINE =====
String firstNum="";
String secondNum="";
String op="";

bool enteringSecond=false;
bool showResult=false;
bool prefixMode=false;

int modeA=0, modeB=0, modeC=0, modeD=0;

// ===== SETUP =====
void setup(){
  lcd.begin(16,2);
  lcd.print("May tinh KHOA HOC");
  delay(1500);
  lcd.clear();
}

// ===== LOOP =====
void loop(){
  char key = keypad.getKey();
  if(!key) return;

  if(key=='*'){ clearAll(); return; }

  if(isDigit(key)){
    if(prefixMode) firstNum += key;
    else if(!enteringSecond) firstNum += key;
    else secondNum += key;
  }
  else if(key=='#'){
    calculate();
    showResult=true;
  }
  else if(key=='A') handleA();
  else if(key=='B') handleB();
  else if(key=='C') handleC();
  else if(key=='D') handleD();

  display();
}

// ===== OPERATOR KEYS =====
void handleA(){
  modeA=(modeA+1)%4;
  prefixMode=false;

  if(modeA==1){ op="+"; enteringSecond=true; }
  if(modeA==2){ op="pow"; enteringSecond=true; }
  if(modeA==3){ op="sqrt"; prefixMode=true; }
  if(modeA==0){ op="log";  prefixMode=true; }

  if(prefixMode){ firstNum=""; secondNum=""; }
}

void handleB(){
  modeB=(modeB+1)%4;
  prefixMode=false;

  if(modeB==1){ op="-"; enteringSecond=true; }
  if(modeB==2){ op="sin"; prefixMode=true; }
  if(modeB==3){ op="cos"; prefixMode=true; }
  if(modeB==0){ op="tan"; prefixMode=true; }

  if(prefixMode){ firstNum=""; secondNum=""; enteringSecond=false; }
}

void handleC(){
  modeC=(modeC+1)%4;
  prefixMode=false;

  if(modeC==1){ op="*"; enteringSecond=true; }
  if(modeC==2){ op="exp"; prefixMode=true; } // e^x
  if(modeC==3){ op="inv"; prefixMode=true; }
  if(modeC==0){ op="ln"; prefixMode=true; }

  if(prefixMode){ firstNum=""; secondNum=""; enteringSecond=false; }
}

void handleD(){
  modeD=(modeD+1)%3;
  prefixMode=false;

  if(modeD==1){ op="/"; enteringSecond=true; }
  if(modeD==2){ op="%"; enteringSecond=true; }
  if(modeD==0){ op=""; }
}

// ===== CALCULATION =====
void calculate(){
  double a = firstNum.toFloat();
  double b = secondNum.toFloat();
  double r=0;

  if(op=="+"||op=="-"||op=="*"||op=="/"||op=="pow"||op=="%"){
    if(op=="+") r=a+b;
    else if(op=="-") r=a-b;
    else if(op=="*") r=a*b;
    else if(op=="/"){ if(b==0){ error(); return;} r=a/b; }
    else if(op=="pow") r=pow(a,b);
    else if(op=="%") r=(a*b)/100;
  }
  else{
    if(op=="sqrt"){ if(a<0){ error(); return;} r=sqrt(a); }
    else if(op=="log"){ if(a<=0){ error(); return;} r=log10(a); }
    else if(op=="ln"){ if(a<=0){ error(); return;} r=log(a); }
    else if(op=="sin") r=sin(a*PI/180);
    else if(op=="cos") r=cos(a*PI/180);
else if(op=="tan") r=tan(a*PI/180);
    else if(op=="inv"){ if(a==0){ error(); return;} r=1/a; }
    else if(op=="exp") r=exp(a);
  }

  firstNum = formatNumber(r);
  secondNum="";
  op="";
  enteringSecond=false;
  prefixMode=false;

  // RESET thứ tự A B C D
  modeA=0; modeB=0; modeC=0; modeD=0;
}

// ===== FORMAT =====
String formatNumber(double x){
  return String(x,6);
}

// ===== UTIL =====
void error(){
  lcd.clear();
  lcd.print("Loi tinh toan");
  delay(1500);
  clearAll();
}

void clearAll(){
  firstNum="";
  secondNum="";
  op="";
  enteringSecond=false;
  showResult=false;
  prefixMode=false;
  modeA=0; modeB=0; modeC=0; modeD=0;
  lcd.clear();
}

// ===== DISPLAY =====
void display(){
  lcd.clear();
  lcd.setCursor(0,0);

  if(prefixMode){
    if(op=="sqrt") lcd.print("sqrt(");
    else if(op=="exp") lcd.print("e^(");
    else{ lcd.print(op); lcd.print("("); }

    lcd.print(firstNum);
    lcd.print(")");
  }
  else{
    lcd.print(firstNum);
    lcd.print(op);
    lcd.print(secondNum);
  }

  if(showResult){
    lcd.setCursor(0,1);
    lcd.print("=");
    lcd.print(firstNum);
  }
}
