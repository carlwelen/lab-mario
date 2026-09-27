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

int prime = 1234567;

int timeoutcount = 0;

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
void handle_interrupt(unsigned cause) {
  /* Kod för att visa tid på 7-segment display */
  volatile int *timer_status = (volatile int *) 0x04000020;
  *timer_status = 0; /* timern har sett timeouten*/

  timeoutcount++;
  if (timeoutcount >= 10){

    if (counter == 60) { counter = 0; minutes += 1; }
    if (minutes == 60) { minutes = 0; hours += 1; }
    if (hours == 24) { hours = 0; }

    set_displays(0, counter % 10);
    set_displays(1, counter / 10);
    set_displays(2, minutes % 10);
    set_displays(3, minutes / 10);
    set_displays(4, hours % 10);
    set_displays(5, hours / 10);
    
    tick( &mytime );
    counter++;
  }

  
}

extern void enable_interrupt(void); 

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  volatile int *timer_control = (volatile int *) 0x04000024;
  volatile int *timer_periodl = (volatile int *) 0x04000028;
  volatile int *timer_periodh = (volatile int *) 0x0400002C;

  // period for 100 ms: 2,999,999 cycles
  *timer_periodl = 0xC6BF;
  *timer_periodh = 0x002D;

    // start + continuous, interrupt bit left at 0 (we poll)
  *timer_control = 0x7;   // 0b0110: START (bit 2) | CONT (bit 1)

  enable_interrupt();
}

/* Your code goes into main as well as any needed functions. */
int main() {
  labinit();
  while (1) {
    print("Prime: ");
    prime = nextprime( prime );
    print_dec( prime );
    print("\n");
  }
}


