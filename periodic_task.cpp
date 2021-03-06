/*
	https://github.com/psr2016/psr/blob/master/firmware/libs/controllib/periodic_task.cpp
*/

#include <stdio.h>
#include "periodic_task.h"

static PeriodicTask * task_list = NULL, * task_list_end = NULL;

PeriodicTask::PeriodicTask(const char * name, int period, float time_unit, int jitter, bool insert_as_last)
    : m_name(name), m_period(period), m_jitter(jitter), m_time_unit(time_unit)
{
    if (insert_as_last) {
        // insert at the end of the list
        if (task_list == NULL) { // if the task_list is empty...
            task_list = this; // let this task be in the task_list
        }
        else {
            task_list_end->m_task_next = this; // add this task at the end of the list
        }
        task_list_end = this; // adjust the pointer
        m_task_next = NULL; // at this point the next_task is null, ready to be assigned
    }
    else {
        // insert at the head of the list
        if (task_list == NULL) {
            task_list = this; // let this task be in the task_list
            task_list_end = this; // adjust the pointer
            m_task_next = NULL;
        }
        else { // shift all tasks and insert this at the head
            m_task_next = task_list;
            task_list = this;
        }
    }

    m_real_time_period = (time_unit * period) / 1000.0; // times are given in milliseconds and then converted into seconds
    m_on = false;
}

void PeriodicTask::execute()
{
    if (!m_on) return;

    if (m_ticks == m_jitter)
        run();
    ++m_ticks;
    if (m_ticks >= m_period) m_ticks = 0;
}

void run_all_tasks()
{
    PeriodicTask * p = task_list;
    while (p != NULL) {
        p->execute();
        p = p->m_task_next;
    }
}
