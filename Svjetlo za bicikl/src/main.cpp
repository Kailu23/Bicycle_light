#include "I2Cdev.h"
#include "MPU6050.h"

#define BREAK_LED B1
#define LEFT_TILT_LED B0
#define RIGHT_TILT_LED A7

#define TILT_MAX 45
#define TILT_MIN -45
#define TILT_MID 0
#define ACCEL_MIN 0
#define ACCEL_MAX -5

MPU6050 mpu;

struct vec3
{
    int16_t x, y, z;
};

vec3 accel;
vec3 gyro;

void setup()
{
// Dosta ovog je kopirano iz onog primjera što si poslao ako nije očito lol
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

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

    // Mislim da bi to ovim trebao moći osi istestirati
    Serial.print("Accel:\t");
    Serial.print(accel.x);
    Serial.print("\t");
    Serial.print(accel.y);
    Serial.print("\t");
    Serial.print(accel.z);
    Serial.print("\n");
    Serial.print("Gyro:\t");
    Serial.print(gyro.x);
    Serial.print("\t");
    Serial.print(gyro.y);
    Serial.print("\t");
    Serial.print(gyro.z);
    Serial.print("\n");
    // return;

    // TODO: osim defineova će vjerojatno trebati i promjenit osi koje mapiramo ovdje
    // možda i tilt_*_level vrijednosti zamjeniti
    // možda i ACCEL_* vrijednosti zamjenit
    int tilt_left_level = map(constrain(gyro.x, TILT_MIN, TILT_MID), TILT_MIN, TILT_MID, 0, 255);
    int tilt_right_level = map(constrain(gyro.x, TILT_MID, TILT_MAX), TILT_MID, TILT_MAX, 0, 255);
    int accel_level = map(constrain(accel.x, ACCEL_MIN, ACCEL_MAX), ACCEL_MIN, ACCEL_MAX, 0, 255);

    analogWrite(LEFT_TILT_LED, tilt_left_level);
    analogWrite(RIGHT_TILT_LED, tilt_right_level);
    analogWrite(BREAK_LED, accel_level);
}