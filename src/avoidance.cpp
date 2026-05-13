#include "avoidance.h"
#include "matchTimer.h"

/* Sensors */
#define USW1 15
#define USW2 2
#define CAPTOR1 41
#define CAPTOR2 21

bool obstacleFrontLeft = false;
bool obstacleFrontRight = false;

bool obstacleRearLeft = false;
bool obstacleRearRight = false;

bool getObstacleFront() {
    return obstacleFrontLeft || obstacleFrontRight;
}

bool getObstacleFrontLeft() {
    return obstacleFrontLeft;
}

bool getObstacleFrontRight() {
    return obstacleFrontRight;
}

bool getObstacleRear() {
    return obstacleRearLeft || obstacleRearRight;
}

bool getObstacleRearLeft() {
    return obstacleRearLeft;
}

bool getObstacleRearRight() {
    return obstacleRearRight;
}

void init_sensors() {
    pinMode(USW1, INPUT);
    pinMode(USW2, INPUT);
    pinMode(CAPTOR1, INPUT);
    pinMode(CAPTOR2, INPUT);
}

static constexpr size_t obstacle_buf_size = 5;
static bool obstacle_front_right_buf[obstacle_buf_size] = { false };
static bool obstacle_front_left_buf[obstacle_buf_size] = { false };
static bool obstacle_rear_left_buf[obstacle_buf_size] = { false };
static bool obstacle_rear_right_buf[obstacle_buf_size] = { false };
static size_t current = 0;

static bool obstacle_detect(bool buffer[])
{
    size_t t_count = 0;
    for(size_t i = 0; i < obstacle_buf_size; i++)
    {
        if(buffer[i])
            return true;
    }

    return false;
    //return t_count >= (obstacle_buf_size / 2);
}

void TaskAvoidance(void *pvParameters) {
    init_sensors();
    
    while (getMatchTime() < matchTotalDuration) {
        // Check bumper sensors

        obstacle_front_left_buf[current]= digitalRead(USW1) != LOW;
        obstacle_front_right_buf[current]= digitalRead(USW2) != LOW;
        obstacle_rear_left_buf[current]= digitalRead(CAPTOR1) == LOW;
        obstacle_rear_right_buf[current]= digitalRead(CAPTOR2) == LOW;
        vTaskDelay(10);

        obstacleFrontLeft = obstacle_detect(obstacle_front_left_buf);
        obstacleFrontRight = obstacle_detect(obstacle_front_right_buf);
        obstacleRearLeft = obstacle_detect(obstacle_rear_left_buf);
        obstacleRearRight = obstacle_detect(obstacle_rear_right_buf);

        current = (current + 1) % obstacle_buf_size;
    }

    vTaskDelete(NULL);
}