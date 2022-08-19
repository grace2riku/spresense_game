/****************************************************************************
 * apps/examples/bmi160/sixaxis_main.c
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdio.h>

#include <nuttx/sensors/bmi160.h>

#include <stdlib.h>
#include <math.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define ACC_DEVPATH      "/dev/accel0"

#define rad_to_deg(a) (a/M_PI*180)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

float convertRawGyro(int gRaw) {
  // since we are using 2000 degrees/seconds range
  // -2000 maps to a raw value of -32768
  // +2000 maps to a raw value of 32767
  float g = (gRaw * 2000.0) / 32768.0;

  return g;
}

/****************************************************************************
 * sixaxis_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  int fd;
  struct accel_gyro_st_s data;
  uint32_t prev;

  volatile float dt;
  volatile float degree_x, degree_y, degree_z;
  volatile float gx, gy, gz;
  volatile float pre_gx, pre_gy, pre_gz;
  int print_display_count;


  fd = open(ACC_DEVPATH, O_RDONLY);
  if (fd < 0)
    {
      printf("Device %s open failure. %d\n", ACC_DEVPATH, fd);
      return -1;
    }

  prev = 0;
  degree_x = degree_y = degree_z = 0;
  dt = 0;
  pre_gx = pre_gy = pre_gz = 0;
  print_display_count = 0;

  for (; ; )
    {
      int ret;

      ret = read(fd, &data, sizeof(struct accel_gyro_st_s));
      if (ret != sizeof(struct accel_gyro_st_s))
        {
          fprintf(stderr, "Read failed.\n");
          break;
        }


      /* If sensing time has been changed, show 6 axis data. */

      if (prev != data.sensor_time)
        {
          dt = (abs(data.sensor_time - prev) * 10) / 1000.0; // sensor_time count resolution: 10ms
          prev = data.sensor_time;

          gx = convertRawGyro(data.gyro.x);
          gy = convertRawGyro(data.gyro.y);
          gz = convertRawGyro(data.gyro.z);

          degree_x += (pre_gx + gx) * dt / 2;
          degree_y += (pre_gy + gy) * dt / 2;
          degree_z += (pre_gz + gz) * dt / 2;

          degree_x = fmodf(degree_x, 360.0);
          degree_y = fmodf(degree_y, 360.0);
          degree_z = fmodf(degree_z, 360.0);

          pre_gx = gx;
          pre_gy = gy;
          pre_gz = gz;

//          degree_x = rad_to_deg(rad_x);
//          degree_y = rad_to_deg(rad_y);
//          degree_z = rad_to_deg(rad_z);

          ++print_display_count;
          if (print_display_count >= 100) {
            print_display_count = 0;

            printf("\033[2J");  // 画面クリア
            printf("\033[%d;%dH", 0, 0);  // 移動 高さ, 横

            printf("[%" PRIu32 "] %d, %d, %d / %d, %d, %d\n",
                  data.sensor_time,
                  data.gyro.x, data.gyro.y, data.gyro.z,
                  data.accel.x, data.accel.y, data.accel.z);

            printf("dt=%f, gx=%f, gy=%f, gz=%f, deg(x)=%f, deg(y)=%f, deg(z)=%f\n", 
              dt, gx, gy, gz,
              degree_x, degree_y, degree_z);

            fflush(stdout);
          }

        }

      usleep(10000);
//      sleep(1);


    }

  close(fd);

  return 0;
}
