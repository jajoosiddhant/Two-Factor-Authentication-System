/**
 * @file timer.c
 * @author Siddhant Jajoo and Satya Mehta
 * @brief All function related to timer have been defined in this file.
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "timer.h"

/**
 * @brief - This function creates and starts the respective timer.
 * 
 * @param timer_handle - This signifies which timer needs to be initialized.
 * The values can be:   TIMER_TEMP
 *                      TIMER_LIGHT
 *                      TIMER_HB
 * @return err_t - Error value (0 for success)
 */
err_t timer_init(uint8_t timer_handle)
{
    if (timer_handle == TIMER_RETRY)
    {
        struct itimerspec trigger_retry;
        struct sigevent sev_temp;
        memset(&sev_temp, 0, sizeof(struct sigevent));
        memset(&trigger_retry, 0, sizeof(struct itimerspec));

        sev_temp.sigev_notify = SIGEV_THREAD;
        sev_temp.sigev_notify_function = &timer_handler;
        sev_temp.sigev_value.sival_int = timer_handle;
        if (timer_create(CLOCK_REALTIME, &sev_temp, &timeout_retry))
        {
            error_log("ERROR: timer_create(temp); in timer_init() function", ERROR_DEBUG, P2);
        }
        else
        {
            msg_log("Temperature Timer initialized.\n", DEBUG, P0, CONTROL_NODE);
        }

        //Setting the first timer interval and the repeating timer interval
        trigger_retry.it_value.tv_sec = RETRY_INTERVAL_SEC;
        trigger_retry.it_interval.tv_sec = RETRY_INTERVAL_SEC;
        trigger_retry.it_value.tv_nsec = RETRY_INTERVAL_NSEC;
        trigger_retry.it_interval.tv_nsec = RETRY_INTERVAL_NSEC;
        retry_flag = 0;
        if (timer_settime(timeout_retry, 0, &trigger_retry, NULL))
        {
            error_log("ERROR: timer_settime(temp); in timer_init function", ERROR_DEBUG, P2);
        }
        else
        {
            msg_log("Temperature Timer started.\n", DEBUG, P0, CONTROL_NODE);
        }
    }
    else if (timer_handle == TIMER_HB)
    {
        struct itimerspec trigger_hb;
        struct sigevent sev_hb;
        memset(&sev_hb, 0, sizeof(struct sigevent));
        memset(&trigger_hb, 0, sizeof(struct itimerspec));

        sev_hb.sigev_notify = SIGEV_THREAD;
        sev_hb.sigev_notify_function = &timer_handler;
        sev_hb.sigev_value.sival_int = timer_handle;
        if (timer_create(CLOCK_REALTIME, &sev_hb, &timeout_hb))
        {
            error_log("ERROR: timer_create(hb); in timer_init() function", ERROR_DEBUG, P2);
        }
        else
        {
            msg_log("Heartbeat Timer initialized.\n", DEBUG, P0, CONTROL_NODE);
        }

        //Setting the first timer interval and the repeating timer interval
        trigger_hb.it_value.tv_sec = HB_INTERVAL_SEC;
        trigger_hb.it_interval.tv_sec = HB_INTERVAL_SEC;
        trigger_hb.it_value.tv_nsec = HB_INTERVAL_NSEC;
        trigger_hb.it_interval.tv_nsec = HB_INTERVAL_NSEC;

        if (timer_settime(timeout_hb, 0, &trigger_hb, NULL))
        {
            error_log("ERROR: timer_settime(hb); in timer_init function", ERROR_DEBUG, P2);
        }
        else
        {
            msg_log("Heartbeat Timer started.\n", DEBUG, P0, CONTROL_NODE);
        }
    }
    return OK;
}

/**
 * @brief - This function is invoked on timer expiration.
 * 
 * @param sigval - This parameter is passed in the timer_init function in their respective cases.
 */
void timer_handler(union sigval sv)
{
    if (sv.sival_int == TIMER_RETRY)
    {
        struct packet_struct obj;
        msg_log("Retry timer fired.\n", DEBUG, P0, CONTROL_NODE);
        if(retry_flag == 0)
        {
            mq_receive(packet_mq, (char *)&obj, sizeof(obj), 0);
        }
        if(retry_flag == 3)
        {
            timer_delete(timeout_retry);
            msg_log("Remote Node is offline\n", DEBUG, P0, CONTROL_NODE);
        }
        send_bytes(obj);
        retry_flag++;
    }
    else if (sv.sival_int == TIMER_HB)
    {
        hb_send(CLEAR_HB);
        msg_log("In Timer Handler: Heartbeat Timer fired.\n", DEBUG, P0, CONTROL_NODE);
    }
}

/**
 * @brief - This function deletes all the timers created.
 * 
 * @return err_t 
 */
err_t timer_del(void)
{
    if (timer_delete(timeout_retry))
    {
        perror("ERROR: timer_delete(temp); in timer_del() function");
    }

    if (timer_delete(timeout_light))
    {
        perror("ERROR: timer_delete(light); in timer_del() function");
    }

    if (timer_delete(timeout_hb))
    {
        perror("ERROR: timer_delete(hb); in timer_del() function");
    }

    return OK;
}