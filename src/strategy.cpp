#include "strategy.h"

#include "actuators.h"
#include "common.h"
#include "diff_drive.h"
#include "id.hh"

#define yellow_angle 1
#define blue_angle -1
#define homologation false

extern enum side current_side;
extern uint8_t pami_id;

/*
    Evolutek Strategies France 2026
*/

void strat_pami_1(int side) {
    servo_drop_r();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    moveStepper(-500, 3000, 1000);
    turnStepper(-1 * side * 90, 3000, 1000);

    moveStepper(-600, 3000, 1000);

    turnStepper(side * 90, 3000, 1000);
    servo_drop_l();
    turnStepper(-1 * side * 90, 3000, 1000);

    moveStepper(-50, 3000, 1000);

    turnStepper(side * 90, 3000, 1000);
    moveStepper(-250, 3000, 1000);
}

void strat_pami_2(int side) {
    moveStepper(-500, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(-1 * side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    moveStepper(-550, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    moveStepper(50, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();

    turnStepper(side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    moveStepper(-300, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(-1 * side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-300, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    servo_drop_l();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(-1 * side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-300, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(30 * side, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    servo_drop_r();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(-1 * (30 + 90) * side, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-200, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void strat_pami_ninja(int side)
{
    constexpr int dst = 210;
    constexpr int speed = 3000;
    constexpr int accel = 1500;

    moveStepper(70, 200, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    moveStepper(125, 200, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(dst, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();

    moveStepper(-125, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 93, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    moveStepper(-400, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(-side * 15, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-200, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();

    moveStepper(200, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(side * (15 + 90), 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-55, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    //toggle_avoiding();
}

void TaskStrategy(void* pvParameters)
{
    init_diff_drive();

    // start_straight_line(SIDE_BLUE);
    int inverse = current_side == SIDE_YELLOW ? 1 : -1;

    if (id == 0)
        strat_pami_1(inverse);
    else if (id == 1)
        strat_pami_2(inverse);
    else if (id ==2)
        strat_pami_ninja(inverse);
    else
        Serial.println("no strat specified for this id");
    
    stopStepper();
    disableSteppers();
    vTaskDelete(NULL);
}