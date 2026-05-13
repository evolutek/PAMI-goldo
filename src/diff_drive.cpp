#include <AccelStepper.h>
#include "avoidance.h"

#include "id.hh"

/* Motors */
#define M1_STEP 48
#define M1_DIR 16
#define M2_STEP 38
#define M2_DIR 39
#define M_EN 8

// HardwareSerial MySerial0(0);
AccelStepper stepper_left = AccelStepper(stepper_left.DRIVER, M2_STEP, M2_DIR);
AccelStepper stepper_right = AccelStepper(stepper_left.DRIVER, M1_STEP, M1_DIR);

// Constants for PAMI

#define ENTRAXE_MM 133.0f
#define WHEEL_RADIUS_MM 29.90f
#define MICROSTEP_FACTOR 8
#define STEPS_PER_REVOLUTION (360 * MICROSTEP_FACTOR / 1.8f)

#define DIST_MM_PER_REVOLUTION (WHEEL_RADIUS_MM * 2 * M_PI) // Distance traveled for a fulli wheel revolution (in mm)

#define STEPS_PER_MM_THEORY (STEPS_PER_REVOLUTION / DIST_MM_PER_REVOLUTION)
#define STEPS_PER_MM (STEPS_PER_MM_THEORY * 1.0f)

#define STEPS_PER_DEG_THEORY ((STEPS_PER_MM * M_PI * ENTRAXE_MM) / 360.0)
#define STEPS_PER_DEG (STEPS_PER_DEG_THEORY * 1.0f)

constexpr float steps_per_mm_pami = STEPS_PER_MM;
constexpr float diff_steps_per_deg_pami = STEPS_PER_DEG;
int32_t targetLeft = 0;
int32_t targetRight = 0;

bool enable_avoiding = true;

void toggle_avoiding()
{
  enable_avoiding = !enable_avoiding;
}

void TaskStepper(void *pvParameters)
{                    // This is a task.
  vTaskDelete(NULL); // Delete myself
}

void init_diff_drive()
{
  stepper_left.setMaxSpeed(1000 * steps_per_mm_pami);     // 100mm/s @ 80 steps/mm
  stepper_left.setAcceleration(1000 * steps_per_mm_pami); // 2000mm/s^2
  stepper_left.setEnablePin(M_EN);
  stepper_left.setPinsInverted(true, false, true); /* LEFT dir is inverted */
  stepper_left.enableOutputs();

  stepper_right.setMaxSpeed(1000 * steps_per_mm_pami);     // 100mm/s @ 80 steps/mm
  stepper_right.setAcceleration(1000 * steps_per_mm_pami); // 2000mm/s^2
  stepper_right.setEnablePin(M_EN);
  stepper_right.setPinsInverted(false, false, true);
  stepper_right.enableOutputs();
}

void stopStepper()
{
  stepper_left.stop();
  stepper_right.stop();
}

void moveStepper(float distance_mm, float speed, float acceleration)
{
  bool detected = false;
  int32_t steps = distance_mm * steps_per_mm_pami;
  targetLeft += steps;
  targetRight += steps;
  bool shouldInitMove = true;

  while (shouldInitMove || (stepper_left.isRunning() || stepper_right.isRunning()))
  {
    if (enable_avoiding && 
      ((distance_mm > 0.f && getObstacleFront()) || (distance_mm < 0.f && getObstacleRear())))
    {
      stopStepper();
      shouldInitMove = true;
      vTaskDelay(1);
      continue;
    }
    if(shouldInitMove) {
      shouldInitMove = false;
      // Reset curr pos to re-init accel ramps
      stepper_left.setCurrentPosition(stepper_left.currentPosition());
      stepper_right.setCurrentPosition(stepper_right.currentPosition());
      stepper_left.setMaxSpeed(speed * steps_per_mm_pami);
      stepper_right.setMaxSpeed(speed * steps_per_mm_pami);
      stepper_left.setAcceleration(acceleration * steps_per_mm_pami);
      stepper_right.setAcceleration(acceleration * steps_per_mm_pami);
      stepper_left.moveTo(targetLeft);
      stepper_right.moveTo(targetRight);
    }
    stepper_left.run();
    stepper_right.run();
  }
  vTaskDelay(1);
}

void turnStepper(float angle_deg, float speed, float acceleration)
{
  float steps = angle_deg * diff_steps_per_deg_pami;

  targetLeft += steps;
  targetRight -= steps;

  stepper_left.setMaxSpeed(speed * steps_per_mm_pami);
  stepper_right.setMaxSpeed(speed * steps_per_mm_pami);
  stepper_left.setAcceleration(acceleration * steps_per_mm_pami);
  stepper_right.setAcceleration(acceleration * steps_per_mm_pami);
  stepper_left.moveTo(targetLeft);
  stepper_right.moveTo(targetRight);

  while ((stepper_left.isRunning() || stepper_right.isRunning()))
  {
    stepper_left.setMaxSpeed(speed * steps_per_mm_pami);
    stepper_right.setMaxSpeed(speed * steps_per_mm_pami);
    stepper_left.run();
    stepper_right.run();
  }
  vTaskDelay(1);
}

void disableSteppers()
{
  stepper_left.disableOutputs();
  stepper_right.disableOutputs();
}
