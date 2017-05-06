// MultiStepper.pde
// -*- mode: C++ -*-
// Use MultiStepper class to manage multiple steppers and make them all move to
// the same position at the same time for linear 2d (or 3d) motion.

#include <Arduino.h>
#include <drawingrobot.h>
#include <Servo.h>
#include <stdio.h>
#include <hyundairemote.h>

// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
// LEFT motor pins: 2,3,4,5 => 2,4,3,5
// RIGHT motor pins: 8,9,10,11  //5,6,7,8 => 5,7,6,8; 9,10,11,12 => 9,11,10,12

//AccelStepper sr(AccelStepper::FULL4WIRE, 2, 4, 3, 5);
//AccelStepper sl(AccelStepper::FULL4WIRE, 9, 11, 10, 12);

Servo servo;
HyundaiRemote remote(7);
void penup();

void switchOff() {
    penup();
    Serial.println("Stopping");
    delay(500);
    while(true);
}

void setup() {
  Serial.begin(9600);
  servo.attach(6);
  penup();
  Serial.println("Hello");

  remote.enable();
  pinMode(2, INPUT); //switch
  attachInterrupt(digitalPinToInterrupt(2), switchOff, RISING);
}

double wheelDiameter = 66.2;
double wheelBase = 132.5;
float speed = 200;


DrawingRobot robot(
    8, 3, 4, 5,
    9,10,11,12,
    wheelDiameter, wheelBase, speed);

void penup() {
    servo.write(40);
}

void pendown() {
    servo.write(160);
}



void draw() {
    robot.rightarc(360, 50);
};

int prevAction = -1;
void act2(int action) {
    if (action == remote.RECEIVED_NOTHING) return;

    Serial.print("Action:");
    Serial.println(action);

    Serial.println(action);

    if (action == remote.STOP) {

        return;
    }

    if (action == remote.LEFT) {
        robot.left(10);
        return;
    }

    if (action == remote.RIGHT) {
        robot.right(10);
        return;
    }

    if (action == remote.BACKWARD) {
        robot.backward(30);
        return;
    }
    if (action == remote.FORWARD) {
        robot.forward(30);
        return;
    }
    if (action == remote.HIT) {
        pendown();
        delay(100);
        penup();
    }
}

void loop() {
    act2(remote.getValue());
    //Serial.println(digitalRead(2));
}
