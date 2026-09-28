/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

volatile int counter = 0;
volatile int minutes = 0;
volatile int hours = 0;

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

void set_leds(int led_mask){
  volatile int *p;
  p = (volatile int*) 0x04000000;
  *p = led_mask;
}

void set_displays(int display_number, int value){
  /* Skriv noll till en bitplats gör att den lyser */
  int baseadress = 0x04000050;
  (display_number *= (0x10));
  baseadress += display_number;
  
  volatile int *p;
  p = (volatile int*) baseadress;

  int digit_patterns[10] = {
    192,  // 0 (11000000)
    249,  // 1 (11111001)
    164,  // 2 (10100100)
    176,  // 3 (10110000)
    153,  // 4 (10011001)
    146,  // 5 (10010010)
    130,  // 6 (10000010)
    248,  // 7 (11111000)
    128,  // 8 (10000000)
    144   // 9 (10010000)
  };

  int pattern = digit_patterns[value];

  *p = pattern;

}

int get_sw(void){
  /* mem adress is 0x04000010 for switches*/
  return *(volatile int*) 0x04000010 & 0x3FF;
}

int get_btn(void){
  return *(volatile int*) 0x040000d0 & 0x1;
}



/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  set_displays(1, 0);
  set_displays(2, 0);
  set_displays(3, 0);
  set_displays(4, 0);
  set_displays(5, 0);
  set_displays(0, 8);
  labinit();

  for (int i = 0; i < 16; i++) {   // 0000 -> 1111
    set_leds(i);
    delay(2);                      // ~1 "second"
  }

  // Enter a forever loop
  while (1) {

    if (get_sw() & (1 << 7)) break;

    if(get_btn()){
      int sw = get_sw();
      int select = (sw >> 8) & 0x3;
      int value = sw & 0x3F;
      if (select == 1) counter = value;
      else if (select == 2) minutes = value;
      else if (select == 3) hours = value;
    }
    
    if (counter == 60){
      counter = 0;
      minutes += 1; 
    }

    if (minutes == 60){
      minutes = 0; 
      hours += 1;
    }

    if(hours == 24){
      break;
    }

    set_displays(0, counter % 10);
    set_displays(1, counter / 10);
    set_displays(2, minutes % 10);
    set_displays(3, minutes / 10);
    set_displays(4, hours % 10);
    set_displays(5, hours / 10);
    
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 2 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
    
    /* set timme minut sekund*/

    counter++;
  }
}


