void init_diff_drive();
void moveStepper(float distance_mm, float speed, float acceleration);
void turnStepper(float angle_deg, float speed, float acceleration);
void disableSteppers();
void stopStepper();

void toggle_avoiding();