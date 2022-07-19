#include <Arduino.h>

TaskHandle_t task1, task2;

// xsemaphore
SemaphoreHandle_t xSemaphore = NULL;

int count = 0;

void taskHandler1(void *pvParameters)
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    Serial.println("task1");
    for (;;)
    {
        // Serial.print("Task 1 @ core : ");
        // // get core id
        // Serial.println(xPortGetCoreID());
        count++;

        if (count >= 10)
        {

            Serial.println();
            xSemaphoreGive(xSemaphore);
            vTaskDelay(50);
            count = 0;
            xSemaphoreTake(xSemaphore, portMAX_DELAY);
        }
        Serial.print(count);

        vTaskDelay(1000);
    }
}

void taskHandler2(void *pvParameters)
{
    Serial.println("task2");
    for (;;)
    {
        // Serial.print("Task 2 @ core : ");
        // // get core id
        // Serial.println(xPortGetCoreID());

        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        Serial.println(count);
        xSemaphoreGive(xSemaphore);
        vTaskDelay(50);
    }
}

void setup()
{
    Serial.begin(9600);
    xSemaphore = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(taskHandler1, "Task1", 10000, NULL, 1, &task1, 0);
    delay(500);
    xTaskCreatePinnedToCore(taskHandler2, "Task2", 10000, NULL, 1, &task2, 1);
}

void loop()
{
}