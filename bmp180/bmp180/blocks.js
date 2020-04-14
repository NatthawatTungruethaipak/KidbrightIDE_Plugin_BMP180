// i2c channel 0 to 64
var channel_array = [];
    for (var i = 0;i <= 64; i++) {
        channel_array.push([String(i), String(i)]);
    }
Blockly.Blocks["bmp180.gettemperature"] = {
    init: function () {
        this.appendDummyInput()
            .appendField(Blockly.Msg.BMP180_TEXT_TITLE_temperature)
            .appendField(Blockly.Msg.BMP180_TEXT_CHANNEL)
            .appendField(new Blockly.FieldDropdown(channel_array), 'channel')
            .appendField(Blockly.Msg.BMP180_TEXT_INCELSIUS);
        this.setOutput(true,'Number');
        this.setInputsInline(true);
        this.setPreviousStatement(false);
        this.setNextStatement(false);
        this.setColour(58);
        this.setTooltip(Blockly.Msg.BMP180_TEXT_TOOLTIP_temperature);
        this.setHelpUrl(Blockly.Msg.BMP180_TEXT_HELPURL);
    }
};
Blockly.Blocks["bmp180.get_absolute_pressure"] = {
    init: function () {
        this.appendDummyInput()
            .appendField(Blockly.Msg.BMP180_TEXT_TITLE_absolute_pressure)
            .appendField(Blockly.Msg.BMP180_TEXT_CHANNEL)
            .appendField(new Blockly.FieldDropdown(channel_array), 'channel')
            .appendField(Blockly.Msg.BMP180_TEXT_INMILLIBAR);
        this.setOutput(true,'Number');
        this.setInputsInline(true);
        this.setPreviousStatement(false);
        this.setNextStatement(false);
        this.setColour(58);
        this.setTooltip(Blockly.Msg.BMP180_TEXT_TOOLTIP_absolute_pressure);
        this.setHelpUrl(Blockly.Msg.BMP180_TEXT_HELPURL);
    }
};
Blockly.Blocks["bmp180.get_sealevel_pressure"] = {
    init: function () {
        this.appendDummyInput()
        .appendField(Blockly.Msg.BMP180_TEXT_TITLE_sealevel_pressure)
        .appendField(Blockly.Msg.BMP180_TEXT_CHANNEL)
        .appendField(new Blockly.FieldDropdown(channel_array), 'channel')
        .appendField(Blockly.Msg.BMP180_TEXT_ALTITUDE)
        .appendField(new Blockly.FieldNumber(1.5), "altitude")
        .appendField(Blockly.Msg.BMP180_TEXT_INMILLIBAR);
        this.setOutput(true,'Number');
        this.setInputsInline(true);
        this.setPreviousStatement(false);
        this.setNextStatement(false);
        this.setColour(58);
        this.setTooltip(Blockly.Msg.BMP180_TEXT_TOOLTIP_sealevel_pressure);
        this.setHelpUrl(Blockly.Msg.BMP180_TEXT_HELPURL);
    }
};
Blockly.Blocks["bmp180.get_absolute_altitude"] = {
    init: function () {
        this.appendDummyInput()
        .appendField(Blockly.Msg.BMP180_TEXT_TITLE_absolute_altitude)
        .appendField(Blockly.Msg.BMP180_TEXT_CHANNEL)
        .appendField(new Blockly.FieldDropdown(channel_array), 'channel')
        .appendField(Blockly.Msg.BMP180_TEXT_PRESSURE_REF)
        .appendField(new Blockly.FieldNumber(1008.00), "pressure_ref")
        .appendField(Blockly.Msg.BMP180_TEXT_INMETER);
        this.setOutput(true,'Number');
        this.setInputsInline(true);
        this.setPreviousStatement(false);
        this.setNextStatement(false);
        this.setColour(58);
        this.setTooltip(Blockly.Msg.BMP180_TEXT_TOOLTIP_absolute_altitude);
        this.setHelpUrl(Blockly.Msg.BMP180_TEXT_HELPURL);
    }
};
