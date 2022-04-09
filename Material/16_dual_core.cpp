#include <Arduino.h>

TaskHandle_t task0, task1;

int counter = 0;

void taskHandler0(void *pvParameters)
{
  Serial.println("task0");
  for (;;)
  {

    Serial.print("Task 0 @ Counter : ");
    // get core id
    Serial.println(counter);
    counter++;
    delay(500);
  }
}

void taskHandler1(void *pvParameters)
{
  Serial.println("task1");
  for (;;)
  {
    Serial.print("                           Task 1 @ Counter : ");
    // get core id
    Serial.println(counter);
    delay(3000);
    if (counter > 20)
    {
      // stop task0
      vTaskSuspend(task0);
      delay(5000);
      // start task0
      vTaskResume(task0);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  xTaskCreatePinnedToCore(taskHandler0, "Task0", 10000, NULL, 1, &task0, 0);
  xTaskCreatePinnedToCore(taskHandler1, "Task1", 10000, NULL, 1, &task1, 1);
}

void loop()
{
  delay(100);
}