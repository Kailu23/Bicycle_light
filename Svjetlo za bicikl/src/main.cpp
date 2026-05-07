#include "I2Cdev.h"
#include "MPU6050.h"
 
#define BREAK_LED 3
#define LEFT_TILT_LED 5
#define RIGHT_TILT_LED 6
 
#define ACCEL_Y_MAX 9000
#define ACCEL_Y_MIN -24000
#define TILT_MAX 90
 
#define ACCEL_X_MIN 10000
#define ACCEL_X_MAX 17000
#define ACCEL_MAX 255
 
MPU6050 mpu;
 
struct vec3
{
    int16_t x, y, z;
};
 
vec3 accel;
vec3 gyro;
bool blink;
unsigned long blink_toggle = 0;
 
 
void setup()
{
// Dosta ovog je kopirano iz onog primjera što si poslao ako nije očito lol
    Wire.begin();
 
    Serial.begin(9600);
 
    // Ovo vjv možemo i maknuti kad smo gotovi
    Serial.println("Initializing MPU...");
    mpu.initialize();
    Serial.println("Testing MPU6050 connection...");
    if (mpu.testConnection() == false)
    {
        Serial.println("MPU6050 connection failed");
        while (true)
            ;
    }
    else
    {
        Serial.println("MPU6050 connection successful");
    }
 
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
 
    pinMode(LEFT_TILT_LED, OUTPUT);
    pinMode(RIGHT_TILT_LED, OUTPUT);
    pinMode(BREAK_LED, OUTPUT);
}
 
void loop()
{
    mpu.getAcceleration(&accel.x, &accel.y, &accel.z);
    mpu.getRotation(&gyro.x, &gyro.y, &gyro.z);
 
    accel.y = map(accel.y, ACCEL_Y_MIN, ACCEL_Y_MAX, -TILT_MAX, TILT_MAX);
    int tilt_right_level = accel.y > 30 ? HIGH : LOW;
    int tilt_left_level = accel.y < -30 ? HIGH : LOW;
 
    int brake_level = constrain(map(accel.x, ACCEL_X_MIN, ACCEL_X_MAX, ACCEL_MAX, 0), 0, 255);
 
    // Mislim da bi to ovim trebao moći osi istestirati
    // Serial.print("\n");
    // Serial.print("Accel:\t");
    // Serial.print(accel.x);
    // Serial.print("\t");
    // Serial.print(accel.y);
    // Serial.print("\t");
    // Serial.print(accel.z);
    // Serial.print("Gyro:\t");
    // Serial.print(gyro.x);
    // Serial.print("\t");
    // Serial.print(gyro.y);
    // Serial.print("\t");
    // Serial.print(gyro.z);
    // Serial.print("\n");
    // delay(500);
    // return;
 
    // TODO: osim defineova će vjerojatno trebati i promjenit osi koje mapiramo ovdje
    // možda i tilt_*_level vrijednosti zamjeniti
    // možda i ACCEL_* vrijednosti zamjenit
    // int tilt_left_level = map(constrain(gyro.x, TILT_MIN, TILT_MID), TILT_MIN, TILT_MID, 0, 255);
    // int tilt_right_level = map(constrain(gyro.x, TILT_MID, TILT_MAX), TILT_MID, TILT_MAX, 0, 255);
    // int accel_level = map(constrain(accel.x, ACCEL_MIN, ACCEL_MAX), ACCEL_MIN, ACCEL_MAX, 0, 255);
    // Serial.print("\nTilt left: ");
    // Serial.print(tilt_left_level);
    // Serial.print("\nTilt right: ");
    // Serial.print(tilt_right_level);
    // Serial.print("\nAccel level: ");
    // Serial.print(brake_level);
 
    if (millis() > blink_toggle + 500) {
      blink = !blink;
      blink_toggle = millis();
    };
    if (blink) {
      digitalWrite(LEFT_TILT_LED, tilt_left_level);
      digitalWrite(RIGHT_TILT_LED, tilt_right_level);
    } else {
      digitalWrite(LEFT_TILT_LED, 0);
      digitalWrite(RIGHT_TILT_LED, 0);
    }
    analogWrite(BREAK_LED, brake_level);
}
 
float pitch, roll;
 
void getAngle(int Ax, int Ay, int Az) {
  double x = Ax;
  double y = Ay;
  double z = Az;
  pitch = atan(x / sqrt((y * y) + (z * z)));
  roll = atan(y / sqrt((x * x) + (z * z)));
  pitch = pitch * (180.0 / 3.14);
  roll = roll * (180.0 / 3.14) ;
}
