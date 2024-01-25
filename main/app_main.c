/**
 * @file app_main.c
 * @author Dmitri Lyalikov (Dlyalikov01@manhattan.edu)
 * @brief SPI Master Component Prototype and Test Driver for Interoperability with Jetson Nano Slave development
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

#elif CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32H2
#define GPIO_MOSI 7
#define GPIO_MISO 2
#define GPIO_SCLK 6
#define GPIO_CS 10

#elif CONFIG_IDF_TARGET_ESP32S3
#define GPIO_MOSI 11
#define GPIO_MISO 13
#define GPIO_SCLK 12
#define GPIO_CS 10

#endif //CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2


#ifdef CONFIG_IDF_TARGET_ESP32
#define SENDER_HOST HSPI_HOST

#else
#define SENDER_HOST SPI2_HOST

#endif

void init_buffer(char *buf)
{
    for (int i = 0; i < CONFIG_LOOPBACK_BUF_SIZE; i++) {
        buf[i] = i;
    }
}

void loopback_test(char *sendbuf, char *recvbuf)
{
    for (int i = 0; i < CONFIG_LOOPBACK_BUF_SIZE; i++) {
        assert(sendbuf[i] == recvbuf[i]);
    }
}


//Main application
void app_main(void)
{
    esp_err_t ret;
    spi_device_handle_t handle;

    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1
    };

    //Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg={
        .command_bits=0,
        .address_bits=0,
        .dummy_bits=0,
        .clock_speed_hz=CONFIG_SPI_MASTER_FREQ,
        .duty_cycle_pos=128,        //50% duty cycle
        .mode=0,
        .spics_io_num=GPIO_CS,
        .cs_ena_posttrans=3,        //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
        .queue_size=3
    };

    int n=0;
    char sendbuf[CONFIG_LOOPBACK_BUF_SIZE] = {0};
    char recvbuf[CONFIG_LOOPBACK_BUF_SIZE] = {0};

    init_buffer(&sendbuf);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    //Initialize the SPI bus and add the device we want to send stuff to.
    ret=spi_bus_initialize(SENDER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret==ESP_OK);
    ret=spi_bus_add_device(SENDER_HOST, &devcfg, &handle);
    assert(ret==ESP_OK);

    while(1) {

        t.length=sizeof(sendbuf)*8;
        t.tx_buffer=sendbuf;
        t.rx_buffer=recvbuf;
        // send each element in sendbuf one by one
        for (int i = 0; i <= 10; i++) {
            t.tx_buffer = &sendbuf[i];
            ret=spi_device_transmit(handle, &t);
            assert(ret==ESP_OK);
            printf("Sent: %i\n", sendbuf[i]);
        }
        // ret=spi_device_transmit(handle, &t);
        // printf("Received: %s\n", recvbuf);
        n++;
    }

// If test mode = 0 check loop back
#if CONFIG_TEST_MODE == 0
    loopback_test(sendbuf, recvbuf);
#endif

    //Never reached.
    ret=spi_bus_remove_device(handle);
    assert(ret==ESP_OK);
}
