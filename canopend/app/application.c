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
static void neck_pdo()
{
    static int16_t last_yaw, last_last_yaw;
    static int16_t last_pitch, last_last_pitch;
    static int16_t last_roll, last_last_roll;
    float yaw_f, pitch_f, roll_f;

    // Discard jumps of 0.2 Rad (~12 degrees)
    if((abs(OD_yaw - last_yaw) > 200) && (abs(OD_yaw - last_last_yaw) > 200))
        yaw_f = last_yaw / 1000.0;
    else yaw_f = OD_yaw / 1000.0;

    if(last_yaw != OD_yaw)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: Yaw=%.3f\n", yaw_f);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_last_yaw = last_yaw;
    last_yaw = OD_yaw;

    // Discard jumps of 0.2 Rad (~12 degrees)
    if((abs(OD_pitch - last_pitch) > 200) && (abs(OD_pitch - last_last_pitch) > 200))
        pitch_f = last_pitch / 1000.0;
    else pitch_f = OD_pitch / 1000.0;

    if(last_pitch != OD_pitch)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: Pitch=%.3f\n", pitch_f);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_last_pitch = last_pitch;
    last_pitch = OD_pitch;

    // Discard jumps of 0.2 Rad (~12 degrees)
    if((abs(OD_roll - last_roll) > 200) && (abs(OD_roll - last_last_roll) > 200))
        roll_f = last_roll / 1000.0;
    else roll_f = OD_roll / 1000.0;

    if(last_roll != OD_roll)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: Roll=%.3f\n", roll_f);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_last_roll = last_roll;
    last_roll = OD_roll;
}

static void drawer_pdo()
{
    static float last_temp;

    if(last_temp != OD_drawerTemperature)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: DrawerTemp=%.1f\n", OD_drawerTemperature);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_temp = OD_drawerTemperature;
	
	 static float last_heading;

    if(last_heading != OD_heading)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: Head=%.1f\n", OD_heading);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_heading = OD_heading;
}

static void head_pdo()
{
    static float last_temp;
    static int16_t last_rssi;
    static uint8_t last_fanspeed;

    if(last_temp != OD_headTemperature)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: HeadTemp=%.1f\n", OD_headTemperature);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_temp = OD_headTemperature;

    if(last_rssi != OD_RSSI)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: RSSI=%d\n", OD_RSSI);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_rssi = OD_RSSI;

    if(last_fanspeed != OD_headFans)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: Fans=%u\n", OD_headFans);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_fanspeed = OD_headFans;
}

static void left_pdo()
{
    static int16_t last_rpm;
    static float last_temp;
    static int16_t last_current;

    if(last_rpm != OD_leftRPM)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: LeftRPM=%d\n", OD_leftRPM);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_rpm = OD_leftRPM;

    if(last_temp != OD_leftTemperature)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: LeftTemp=%.1f\n", OD_leftTemperature);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_temp = OD_leftTemperature;

    if(last_current != OD_leftCurrent)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: LeftCurrent=%.2f\n", OD_leftCurrent/1000.0);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_current = OD_leftCurrent;
}

static void right_pdo()
{
    static int16_t last_rpm;
    static float last_temp;
    static int16_t last_current;

    if(last_rpm != OD_rightRPM)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: RightRPM=%d\n", OD_rightRPM);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_rpm = OD_rightRPM;

    if(last_temp != OD_rightTemperature)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: RightTemp=%.1f\n", OD_rightTemperature);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_temp = OD_rightTemperature;

    if(last_current != OD_rightCurrent)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: RightCurrent=%.2f\n", OD_rightCurrent/1000.0);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_current = OD_rightCurrent;
}

static void power_pdo()
{
    static float last_soc;
    static float last_ahused;

    if(last_soc != OD_soC)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: SoC=%.1f\n", OD_soC);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_soc = OD_soC;

    if(last_ahused != OD_ahused)
    {
        char buf[40];
        int len = sprintf(buf, "PDO: AhUsed=%f\n", OD_ahused);
        #ifdef DEBUG
        printf("%s", buf);
        #endif
        CO_command_write(buf, len);
    }

    last_ahused = OD_ahused;
}


/******************************************************************************/
void app_program1ms(void){
    static uint8_t count;

    CO->NMT->operatingState = CO_NMT_OPERATIONAL;
    switch(++count)
    {
        case 20:
            neck_pdo();
            break;
        case 40:
            drawer_pdo();
            break;
        case 60:
            head_pdo();
            break;
        case 80:
            left_pdo();
            right_pdo();
            break;
        case 100:
            power_pdo();
            count = 0;
            break;
    }
}
