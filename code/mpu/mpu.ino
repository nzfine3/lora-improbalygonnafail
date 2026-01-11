#include "MPU9250.h"
#include <Kalman.h>

MPU9250 mpu;

// Kalman filters
Kalman kalmanRoll;
Kalman kalmanPitch;

// Timing
uint32_t prev_ms = 0;
uint32_t kalman_timer = 0;

// Filtered angles
double kalRoll, kalPitch;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {
        while (1) {
            Serial.println("MPU connection failed.");
            delay(5000);
        }
    }

    // Initialize Kalman with current angles
    mpu.update();
    kalmanRoll.setAngle(mpu.getRoll());
    kalmanPitch.setAngle(mpu.getPitch());

    kalman_timer = micros();
}

void loop() {
    if (mpu.update()) {

        // Run at ~40 Hz
        if (millis() > prev_ms + 25) {

            // Time delta
            double dt = (micros() - kalman_timer) / 1000000.0;
            kalman_timer = micros();

            // Raw angles from MPU fusion
            double roll  = mpu.getRoll();
            double pitch = mpu.getPitch();

            // Gyro rates (deg/sec)
            double gyroXrate = mpu.getGyroX();
            double gyroYrate = mpu.getGyroY();

            // Kalman filtering
            kalRoll  = kalmanRoll.getAngle(roll,  gyroXrate, dt);
            kalPitch = kalmanPitch.getAngle(pitch, gyroYrate, dt);

            print_roll_pitch_yaw();

            prev_ms = millis();
        }
    }
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");

    // Yaw = raw (magnetometer-based)
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");

    // Kalman filtered
    Serial.print(kalPitch, 2);
    Serial.print(", ");
    Serial.println(kalRoll, 2);
}