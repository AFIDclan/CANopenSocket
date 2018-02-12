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
#include <stdlib.h>


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
    static int16_t last_yaw, last_last_yaw;
    static int16_t last_pitch, last_last_pitch;
    static int16_t last_roll, last_last_roll;

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

        // Discard jumps of 0.2 Rad (~12 degrees)
        if((abs(yaw - last_yaw) > 200) && (abs(yaw - last_last_yaw) > 200))
            yaw_f = last_yaw / 1000.0;
        else yaw_f = yaw / 1000.0;

        if((abs(pitch - last_pitch) > 200) && (abs(pitch - last_last_pitch) > 200))
            pitch_f = last_pitch / 1000.0;
        else pitch_f = pitch / 1000.0;

        if((abs(roll - last_roll) > 200) && (abs(roll - last_last_roll) > 200))
            roll_f = last_roll / 1000.0;
        else roll_f = roll / 1000.0;

        // Send data to socket if there was a change
        if(last_yaw != yaw || last_pitch != pitch || last_roll != roll)
        {
            len = sprintf(buf, "PDO: %.5f %.5f %.5f\r\n", yaw_f, pitch_f, roll_f);
            puts(buf);
            CO_command_write(buf, len);
        }

        last_last_yaw = last_yaw;
        last_last_pitch = last_pitch;
        last_last_roll = last_roll;

        last_yaw = yaw;
        last_pitch = pitch;
        last_roll = roll;
    }
}
