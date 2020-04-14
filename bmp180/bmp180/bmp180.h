/**
 * bmp180.h
 *
 * Created by Natthawat Tungruethaipak, 10 April 2020
 * Contact via e-mail:natthawat.tungruethaipak@mail.kmutt.ac.th
 */
#ifndef __BMP180_H__
#define __BMP180_H__

#include "driver.h"
#include "device.h"
#include "driver/gpio.h"
#include "i2c-dev.h"

#define BMP180_POLLING_MS 10
// ref : https://github.com/sparkfun/BMP180_Breakout_Arduino_Library
class BMP180 : public Device
{
private:
  enum
  {
    s_detect,
    s_wait_rtc,
    s_idle,
    s_error,
    s_wait,
    s_read_temperature,
    s_write_temperature,
    s_read_pressure,
    s_write_pressure,
    s_init,
    s_calc
  } state;
  TickType_t chkrtc_tickcnt, polling_tickcnt;
  double pressure, temperature;
  int index_init, index_int16_t, index_uint16_t;
  int16_t bmp180_reg_int16[8];
  uint16_t bmp180_reg_uint16[3];
  int16_t AC1, AC2, AC3, VB1, VB2, MB, MC, MD;
  uint16_t AC4, AC5, AC6;
  double c5, c6, mc, md, x0, x1, x2, y0, y1, y2, p0, p1, p2;
  double c3, c4, b1;

public:
  TickType_t tickcnt;
  // constructor
  BMP180(int bus_ch, int dev_addr);
  // override
  void init(void);
  void process(Driver *drv);
  int prop_count(void);
  bool prop_name(int index, char *name);
  bool prop_unit(int index, char *unit);
  bool prop_attr(int index, char *attr);
  bool prop_read(int index, char *value);
  bool prop_write(int index, char *value);
  // method
  double gettemperature(void);
  double get_absolute_pressure(void);
  double get_sealevel_pressure(double altitude);
  double get_absolute_altitude(double pressure_ref);
};

#endif
