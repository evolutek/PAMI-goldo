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
    toggle_avoiding();
    constexpr int dst = 210;
    constexpr int speed = 3000;
    constexpr int accel = 1500;

    moveStepper(70, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(125, 200, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(dst, speed, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-125, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    turnStepper(side * 93, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-600, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(100, 250, accel);

    vTaskDelay(100 / portTICK_PERIOD_MS);
    turnStepper(side * 90, 3000, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    moveStepper(-90, 250, accel);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    //toggle_avoiding();
}

void TaskStrategy(void* pvParameters)
{
    init_diff_drive();

    // start_straight_line(SIDE_BLUE);
    int inverse = current_side == SIDE_YELLOW ? 1 : -1;
    if (id == 0)
        start_strat_pami_evo_1(inverse);
    else if (id == 1)
        start_strat_pami_evo_2(inverse);
    else if (id ==2)
        start_strat_pami_evo_ninja(inverse);
    else
        Serial.println("no strat specified for this id");
    /*
    if(current_side == SIDE_YELLOW) {
        if(pami_id == 0)
            strat_groupie2_yellow_evo();
        else if(pami_id == 1)
            strat_groupie2_yellow_evo();
        else if(pami_id == 2)
            strat_groupie3_yellow_evo();
    } else if(current_side == SIDE_BLUE) {
        if(pami_id == 0)
            strat_groupie2_blue_evo();
        else if(pami_id == 1)
            strat_groupie2_blue_evo();
        else if(pami_id == 2)
            strat_groupie3_blue_evo();
    }
    */
    stopStepper();
    disableSteppers();
    vTaskDelete(NULL);
}