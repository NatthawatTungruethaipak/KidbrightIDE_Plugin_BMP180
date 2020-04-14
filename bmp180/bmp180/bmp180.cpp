/**
 * bmp180.cpp
 *
 * Created by Natthawat Tungruethaipak, 10 April 2020
 * Contact via e-mail:natthawat.tungruethaipak@mail.kmutt.ac.th
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "bmp180.h"

#define BMP180_REG_CONTROL 0xF4
#define BMP180_REG_RESULT 0xF6

#define BMP180_COMMAND_TEMPERATURE 0x2E
#define BMP180_COMMAND_PRESSURE0 0x34
#define BMP180_COMMAND_PRESSURE1 0x74
#define BMP180_COMMAND_PRESSURE2 0xB4
#define BMP180_COMMAND_PRESSURE3 0xF4

BMP180::BMP180(int bus_ch, int dev_addr)
{
  channel = bus_ch;
  address = dev_addr;
  polling_ms = BMP180_POLLING_MS;
  // printf("\nget in cpp\n");
  // printf("\n%d %d\n",channel, address);
}

void BMP180::init(void)
{
  pressure = 0;
  temperature = 0;
  index_init = 0;
  index_int16_t = 0;
  index_uint16_t = 0;
  state = s_detect;
  // printf("\nget in init method\n");
}

int BMP180::prop_count(void)
{
  return 1;
}

bool BMP180::prop_name(int index, char *name)
{
  if (index == 0)
  {
    snprintf(name, DEVICE_PROP_NAME_LEN_MAX, "%s", "temperature");
    return true;
  }

  // not supported
  return false;
}

bool BMP180::prop_unit(int index, char *unit)
{
  if (index == 0)
  {
    snprintf(unit, DEVICE_PROP_UNIT_LEN_MAX, "%sC", "\xc2\xb0");
    return true;
  }

  // not supported
  return false;
}

bool BMP180::prop_attr(int index, char *attr)
{
  if (index == 0)
  {
    get_attr_str(attr, PROP_ATTR_READ_FLAG | PROP_ATTR_TYPE_NUMBER_FLAG); // read only, number
    return true;
  }

  // not supported
  return false;
}

bool BMP180::prop_read(int index, char *value)
{
  if (index == 0)
  {
    snprintf(value, DEVICE_PROP_VALUE_LEN_MAX, "%f", temperature);
    return true;
  }

  // not supported
  return false;
}

bool BMP180::prop_write(int index, char *value)
{
  // not supported
  return false;
}

void BMP180::process(Driver *drv)
{
  // printf("\nget in process method\n");
  I2CDev *i2c = (I2CDev *)drv;
  uint8_t pointer;
  uint8_t data[4];

  if (state == s_detect)
  {
    // printf("\nwait detected\n");
    // stamp polling tickcnt
    polling_tickcnt = get_tickcnt();
    // detect i2c device
    if (i2c->detect(channel, address) == ESP_OK)
    {
      // printf("\nwait detected\n");
      index_init = 0;
      index_int16_t = 0;
      index_uint16_t = 0;
      state = s_init;
    }
    else
    {
      state = s_error;
    }
  }

  else if (state == s_init)
  {
    unsigned char init[11] = {0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE};
    if (index_init > 10)
    {
      state = s_calc;
    }
    else
    {
      i2c->read(channel, address, &init[index_init], 1, data, 2);
      if ((index_init < 3) || (index_init > 5))
      {
        bmp180_reg_int16[index_int16_t] = (int16_t)((data[0] << 8) | data[1]);
        printf("%d\n", bmp180_reg_int16[index_int16_t]);
        index_int16_t++;
      }
      else
      {
        bmp180_reg_uint16[index_uint16_t] = (((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        printf("%u\n", bmp180_reg_uint16[index_uint16_t]);
        index_uint16_t++;
      }
      index_init++;
    }
  }
  else if (state == s_calc)
  {
    c3 = 160.0 * pow(2, -15) * bmp180_reg_int16[2];
    c4 = pow(10, -3) * pow(2, -15) * bmp180_reg_uint16[0];
    b1 = pow(160, 2) * pow(2, -30) * bmp180_reg_int16[3];
    c5 = (pow(2, -15) / 160) * bmp180_reg_uint16[1];
    c6 = bmp180_reg_uint16[2];
    mc = (pow(2, 11) / pow(160, 2)) * bmp180_reg_int16[6];
    md = (bmp180_reg_int16[7] / 160);
    x0 = bmp180_reg_int16[0];
    x1 = 160.0 * pow(2, -13) * bmp180_reg_int16[1];
    x2 = pow(160, 2) * pow(2, -25) * bmp180_reg_int16[4];
    y0 = c4 * pow(2, 15);
    y1 = c4 * c3;
    y2 = c4 * b1;
    p0 = (3791.0 - 8.0) / 1600.0;
    p1 = 1.0 - 7357.0 * pow(2, -20);
    p2 = 3038.0 * 100.0 * pow(2, -36);
    state = s_write_temperature;
  }

  else if (state == s_write_temperature)
  {
    data[0] = BMP180_REG_CONTROL;
    data[1] = BMP180_COMMAND_TEMPERATURE;
    if (i2c->write(channel, address, data, 2) == ESP_OK)
    {
      tickcnt = get_tickcnt();
      state = s_read_temperature;
    }
    else
    {
      state = s_error;
    }
  }

  else if (state == s_read_temperature)
  {
    // read delay for 5ms
    if (is_tickcnt_elapsed(tickcnt, 5))
    {
      pointer = BMP180_REG_RESULT;
      char charBuffer[20];
      double tu, a;
      int temporary;
      if (i2c->read(channel, address, &pointer, 1, data, 2) == ESP_OK)
      {
        tu = (data[0] * 256.0) + data[1];
        a = c5 * (tu - c6);
        temperature = a + (mc / (a + md));
        temporary = (temperature * 100);
        temperature = (temporary / 100.0000);
        // printf("This is temperature: \n");
        // printf("%f\n", temperature);
        tickcnt = get_tickcnt();
        state = s_write_pressure;
      }
      else
      {
        state = s_error;
      }
    }
  }

  else if (state == s_write_pressure)
  {
    data[0] = BMP180_REG_CONTROL;
    data[1] = BMP180_COMMAND_PRESSURE3;
    if (i2c->write(channel, address, data, 2) == ESP_OK)
    {
      tickcnt = get_tickcnt();
      state = s_read_pressure;
    }
    else
    {
      state = s_error;
    }
  }

  else if (state == s_read_pressure)
  {
    // read delay for 5ms
    if (is_tickcnt_elapsed(tickcnt, 26))
    {
      pointer = BMP180_REG_RESULT;
      char charBuffer[20];
      double pu, s, x, y, z;
      int temporary;
      if (i2c->read(channel, address, &pointer, 1, data, 3) == ESP_OK)
      {

        pu = (data[0] * 256.0) + data[1] + (data[2] / 256.0);
        s = temperature - 25.0;
        x = (x2 * pow(s, 2)) + (x1 * s) + x0;
        y = (y2 * pow(s, 2)) + (y1 * s) + y0;
        z = (pu - x) / y;
        pressure = (p2 * pow(z, 2)) + (p1 * z) + p0;
        // printf("This is temperature: \n");
        // printf("%f\n", temperature);
        tickcnt = get_tickcnt();
        state = s_wait;
        // tell system that this module initialized
        initialized = true;
        // clear error flag
        error = false;
      }
      else
      {
        state = s_error;
      }
    }
  }

  else if (state == s_wait)
  {
    if (is_tickcnt_elapsed(tickcnt, polling_ms))
    {
      state = s_detect;
    }
  }
  else if (state == s_error)
  {
    // clear initialized flag
    pressure = 0;
    temperature = 0;
    initialized = false;
    error = true;
    // get current tickcnt
    tickcnt = get_tickcnt();
    // goto wait and retry with detect state
    state = s_wait;
  }
}

double BMP180::gettemperature(void)
{
  double val;
  //printf("hello world\n");
  val = temperature;
  //val = 15.000;
  return val;
}

double BMP180::get_absolute_pressure(void)
{
  double val;
  int temporary;
  //printf("hello world\n");
  val = pressure;
  temporary = (pressure * 100);
  val = (temporary / 100.0000);
  //val = 15.000;
  return val;
}

double BMP180::get_sealevel_pressure(double altitude)
{
  double val;
  int temporary;
  //printf("hello world\n");
  val = (pressure / pow(1 - (altitude / 44330.0), 5.255));
  temporary = (pressure * 100);
  val = (temporary / 100.0000);
  //val = 15.000;
  return val;
}
double BMP180::get_absolute_altitude(double pressure_ref)
{
  double val;
  int temporary;
  //printf("hello world\n");
  val = (44330.0 * (1 - pow(pressure / pressure_ref, 1 / 5.255)));
  temporary = (val * 100);
  val = (temporary / 100.0000);
  //val = 15.000;
  return val;
}
