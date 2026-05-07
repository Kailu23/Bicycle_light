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
    Wire.begin();

    Serial.begin(9600);

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