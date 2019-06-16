Blockly.JavaScript['bmp180.gettemperature'] = function (block) {
  return [
  'DEV_I2C1.BMP180('+ block.getFieldValue('channel') +', 0x77)'+'.gettemperature()', Blockly.JavaScript.ORDER_ATOMIC];
};
Blockly.JavaScript['bmp180.get_absolute_pressure'] = function (block) {
  return [
  'DEV_I2C1.BMP180('+ block.getFieldValue('channel') +', 0x77)'+'.get_absolute_pressure()', Blockly.JavaScript.ORDER_ATOMIC];
};
Blockly.JavaScript['bmp180.get_sealevel_pressure'] = function (block) {
  return [
  'DEV_I2C1.BMP180('+ block.getFieldValue('channel') +', 0x77)'+'.get_sealevel_pressure('+block.getFieldValue('altitude')+')', Blockly.JavaScript.ORDER_ATOMIC];
};
Blockly.JavaScript['bmp180.get_absolute_altitude'] = function (block) {
  return [
  'DEV_I2C1.BMP180('+ block.getFieldValue('channel') +', 0x77)'+'.get_absolute_altitude('+block.getFieldValue('pressure_ref')+')', Blockly.JavaScript.ORDER_ATOMIC];
};