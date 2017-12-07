/*
 * Application interface for CANopenSocket.
 *
 * @file        application.c
 * @author      Janez Paternoster
 * @copyright   2016 Janez Paternoster
 *
 * This file is part of CANopenSocket, a Linux implementation of CANopen
 * stack with master functionality. Project home page is
 * <https://github.com/CANopenNode/CANopenSocket>. CANopenSocket is based
 * on CANopenNode: <https://github.com/CANopenNode/CANopenNode>.
 *
 * CANopenSocket is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "CANopen.h"
#include "CO_command.h"
#include <stdio.h>


/******************************************************************************/
void app_programStart(void){

}


/******************************************************************************/
void app_communicationReset(void){

}


/******************************************************************************/
void app_programEnd(void){

}


/******************************************************************************/
void app_programAsync(uint16_t timer1msDiff){

}


/******************************************************************************/
void app_program1ms(void){
    static uint8_t count;
    static int16_t last_yaw, last_pitch, last_roll;

    CO->NMT->operatingState = CO_NMT_OPERATIONAL;
    if(++count >= 100)
    {
        int16_t yaw, pitch, roll;
        float yaw_f, pitch_f, roll_f;
        char buf[40];
        int len;

        count = 0;
        yaw = ((OD_look >> 32) & 0xFFFF);
        pitch = ((OD_look >> 16) & 0xFFFF);
        roll = (OD_look & 0xFFFF);

        if(yaw == last_yaw && pitch == last_pitch && roll == last_roll)
            return;

        yaw_f = yaw / 1000.0;
        pitch_f = pitch /1000.0;
        roll_f = roll / 1000.0;

        last_yaw = yaw;
        last_pitch = pitch;
        last_roll = roll;

        len = sprintf(buf, "Y%f P%f R%f", yaw_f, pitch_f, roll_f);
        puts(buf);
        CO_command_write(buf, len);
    }
}
