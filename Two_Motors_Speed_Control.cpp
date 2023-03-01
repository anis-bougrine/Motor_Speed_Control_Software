//Headers
#include <PID_v1.h>  
#include <util/atomic.h>

//Macros for Arduino Mega pins definition
#define encoder_A_Right 18  
#define encoder_A_Left 2    
#define motor_left_up 8
#define motor_left_down 9
#define motor_right_up 4
#define motor_right_down 5
#define pwm_Left 11
#define pwm_Right 13

//Variables
double Kp_Right=3.5, Ki_Right=9,Kd_Right=0.5;  
double Kp_Left=4.5, Ki_Left=10.9 ,Kd_Left=0.6;
double Setpoint, Input_Right, Output_Right, Input_Left, Output_Left;    
volatile int Current_NbTicks_Right=0; //Since this variable coning to ba accessed during interruptions it has to be volatile to prevent any cache read operation from processor optimization.
volatile int Current_NbTicks_Left=0;                           
int Last_Sampling=0;                  
int speed_Right=0; 
int speed_Left =0; 
PID PID_Right(&Input_Right, &Output_Right, &Setpoint, Kp_Right, Ki_Right, Kd_Right, DIRECT);
PID PID_Left(&Input_Left, &Output_Left, &Setpoint, Kp_Left, Ki_Left, Kd_Left, DIRECT);

//Setup
void setup()
{
pinMode(encoder_A_Right, INPUT_PULLUP);
pinMode(encoder_A_Left, INPUT_PULLUP);
pinMode(motor_left_up, OUTPUT);
pinMode(motor_left_down, OUTPUT);
pinMode(motor_right_up, OUTPUT);
pinMode(motor_right_down, OUTPUT);
pinMode(pwm_Right, OUTPUT);
pinMode(pwm_Left, OUTPUT);
analogWrite (pwm_Right,0);
analogWrite (pwm_Left,0);
attachInterrupt(digitalPinToInterrupt(encoder_A_Right),counter_Encoder_Right, RISING);
attachInterrupt(digitalPinToInterrupt(encoder_A_Left),counter_Encoder_Left, FALLING);
Input_Right = 0;
Input_Left = 0;
Setpoint = 20;                   
PID_Right.SetMode(AUTOMATIC);
PID_Left.SetMode(AUTOMATIC);
PID_Right.SetOutputLimits(-255, 255);
PID_Left.SetOutputLimits(-255, 255);
}

//The loop mode
void loop(){

//sampling (Te = 10ms)
if(millis()-Last_Sampling>10)
{
    Last_Sampling=millis();

    /*If the volatile variable size is bigger than 8 bits then the processor are not going to read it in one step because of its 8bits architecture.
    So we have to protect the read operation of the volatile variable from any interruption that can access it and consequently leads 
    to false read operation. The ATOMIC_BLOCK macro disable interrupts*/
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
          speed_Right=Current_NbTicks_Right;
          speed_Left=Current_NbTicks_Left;
          Current_NbTicks_Right=0;
          Current_NbTicks_Left=0;
          }
    //PIDs regulators calculations
    Input_Right=speed_Right;
    Input_Left=speed_Left;
    PID_Right.Compute();
    PID_Left.Compute();
    int output_Right=(int)Output_Right;
    int output_Left=(int)Output_Left;
    analogWrite (pwm_Right,output_Right); 
    digitalWrite(motor_right_up, HIGH);
    digitalWrite(motor_right_down, LOW);
    analogWrite (pwm_Left,output_Left); 
    digitalWrite(motor_left_up, HIGH);
    digitalWrite(motor_left_down, LOW);
            
}
}

/**
 * @brief Set the new tick value returned from right motor encoder, executed every interruption (10ms)
 * @return (void)
 */void counter_Encoder_Right()
{
  Current_NbTicks_Right++;
}

/**
 * @brief Set the new tick value returned from left motor encoder, executed every interruption (10ms)
 * @return (void)
 */void counter_Encoder_Left()
{
  Current_NbTicks_Left++;
} 
