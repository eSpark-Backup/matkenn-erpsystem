#include "io.hpp"

#define TAG "GPIO"

void IO::setPin(gpio_num_t number)
{
    this->number = number;
};

bool IO::inputMode()
{
    this->config = {
        .pin_bit_mask = (1UL<<number),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    if(gpio_config(&this->config) == ESP_OK)
    {
        ESP_LOGI(TAG, "inputMode Done!");
        return true;
    }
    else
    {
        ESP_LOGE(TAG, "inputMode Failed");
        return false;
    }
}

bool IO::outputMode()
{
    this->config = {
        .pin_bit_mask = (1UL<<this->number),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    if(gpio_config(&this->config) == ESP_OK)
    {
        ESP_LOGI(TAG, "OutputMode Done!");
        return true;
    }
    else
    {
        ESP_LOGE(TAG, "OutputMode Failed");
        return false;
    }
}
int IO::getState()
{
    return (gpio_get_level(number));
}

void IO::setState(int state)
{
   gpio_set_level(this->number, state);
}

