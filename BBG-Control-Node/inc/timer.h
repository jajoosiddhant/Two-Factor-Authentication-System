/**
 * @file timer.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file for timer.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"
#include "queue.h"
#include "uart.h"
#include "gpio.h"

//Timer intervals
#define RETRY_INTERVAL_SEC   (1)
#define RETRY_INTERVAL_NSEC  (500000000)

#define HB_INTERVAL_SEC (10)
#define HB_INTERVAL_NSEC (0)

//Timer Handle declarations
timer_t timeout_retry;
timer_t timeout_light;
timer_t timeout_hb;
struct itimerspec trigger_hb;
struct itimerspec trigger_retry;
//Function declarations
err_t timer_init(uint8_t timer_number);
void timer_handler(union sigval sv);
err_t timer_del(void);

#endif
