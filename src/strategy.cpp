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
    moveStepper(-475, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(-1 * side * 90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    moveStepper(-450, 3000, 1000);
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

/*
    Evolutek Strategies Belgique 2026
*/

void start_straight_line(int side)
{
    // moveStepper(-2000, 750, 500);
    // moveStepper(500, 750, 500);
    moveStepper(3000, 500, 1000);
}

void start_strat_pami_evo_1(int side)
{
    moveStepper(-600, 1000, 500);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    servo_drop_r();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    int angle = 20;

    // rotate
    turnStepper(-1 * side * angle, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // drop
    servo_drop_l();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // rotate
    turnStepper(-1 * side * (45 - angle), 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-760, 1000, 500);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void start_strat_pami_evo_2(int side)
{
    moveStepper(-800, 1000, 500);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * -90, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 15, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    servo_drop_r();
    servo_drop_l();
    vTaskDelay(500 / portTICK_PERIOD_MS);

    turnStepper(-side * 15, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-400, 1000, 500);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(-side * 40, 3000, 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-345, 1000, 500);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}


void start_strat_pami_evo_ninja(int side)
{
    constexpr int dst = 320;
    constexpr int speed = 2500;
    constexpr int accel = 1000;
    turnStepper(side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    moveStepper(dst, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();

    moveStepper(-dst, speed / 2, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(-side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(110, 1000, 500);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    toggle_avoiding();
    moveStepper(dst, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    toggle_avoiding();

    moveStepper(-dst / 2 - 50, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-250, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void TaskStrategy(void* pvParameters)
{
    init_diff_drive();

    // start_straight_line(SIDE_BLUE);
    int inverse = current_side == SIDE_YELLOW ? 1 : -1;
    if (id == 1)
        strat_pami_1(inverse);
    else if (id == 2)
        strat_pami_2(inverse);
    else if (id == 4) // debug
        start_straight_line(inverse);
    else
        start_strat_pami_evo_ninja(inverse);

    stopStepper();
    disableSteppers();
    vTaskDelete(NULL);
}