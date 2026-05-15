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
    Movement wrappers
*/

void recal(int dx, int speed, int accel) {
    toggle_avoiding();
    moveStepper(dx, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();
}

// your robot need to decel
// forward = 1 and backward = -1
void moveEnd(int dx, int speed, int accel) {
    int epsilon = dx / 30;

    moveStepper(dx - epsilon, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(epsilon, 600, 300);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

/*
    Evolutek Strategies France 2026
*/

void preparation_pami_1(int side) {
    moveStepper(-50, 500, 300);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    recal(100, 500, 300);

    moveStepper(-300, 1000, 900);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void preparation_pami_2(int side) {
    moveStepper(-50, 500, 300);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    recal(100, 500, 300);

    moveStepper(-300, 1000, 900);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void strat_pami_1(int side) {
    servo_drop_l();
    vTaskDelay(400 / portTICK_PERIOD_MS);

    moveStepper(-250, 2000, 900);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(-1 * side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // recal
    recal(100, 500, 300);
    moveStepper(-50, 500, 300);

    vTaskDelay(100 / portTICK_PERIOD_MS);
    moveStepper(-300, 2000, 900);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    moveStepper(-175, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();

    //recal(150, 600, 300);
    servo_drop_r();
    vTaskDelay(400 / portTICK_PERIOD_MS);

    moveEnd(-550, 3000, 1000);
    turnStepper(-1 * side * 90, 3000, 1000);
    moveEnd(-50, 3000, 1000);
}

void strat_pami_2(int side) {
    moveStepper(-475, 3000, 1000);
    turnStepper(-1 * side * 90, 3000, 1000);

    moveStepper(-450, 3000, 1000);
    moveStepper(50, 3000, 1000);

    turnStepper(side * 90, 3000, 1000);
    moveStepper(-300, 3000, 1000);
    turnStepper(-1 * side * 90, 3000, 1000);

    moveStepper(-300, 3000, 1000);

    turnStepper(side * 90, 3000, 1000);
    servo_drop_l();
    turnStepper(-1 * side * 90, 3000, 1000);

    moveStepper(-300, 3000, 1000);

    turnStepper(30 * side, 3000, 1000);
    servo_drop_r();
    turnStepper(-1 * (30 + 90) * side, 3000, 1000);

    moveStepper(-200, 3000, 1000);
}

void start_strat_pami_evo_ninja(int side)
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
        start_strat_pami_evo_ninja(inverse);
    else
        Serial.println("no strat specified for this id");

    stopStepper();
    disableSteppers();
    vTaskDelete(NULL);
}

void TaskPreparationStrategy(void *pvParameters) {
    init_diff_drive();

    int inverse = current_side == SIDE_YELLOW ? 1 : -1;

    if (id == 0)
        preparation_pami_1(inverse);
    else if (id == 1)
        preparation_pami_2(inverse);
    else
        Serial.println("no preparation strat specified for this id");

    stopStepper();
    vTaskDelete(NULL);
}