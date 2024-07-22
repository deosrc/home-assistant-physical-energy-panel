import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display
from esphome.const import (
    CONF_CLK_PIN,
    CONF_DIO_PIN,
    CONF_ID,
    CONF_LAMBDA,
)

groveledbar_ns = cg.esphome_ns.namespace("groveledbar")
GroveLedBarDisplay = groveledbar_ns.class_("GroveLedBarDisplay", cg.PollingComponent)
GroveLedBarDisplayRef = GroveLedBarDisplay.operator("ref")

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(GroveLedBarDisplay),
        cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_DIO_PIN): pins.gpio_output_pin_schema,
    }
).extend(cv.polling_component_schema("1s"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    clk = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(clk))
    dio = await cg.gpio_pin_expression(config[CONF_DIO_PIN])
    cg.add(var.set_dio_pin(dio))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(GroveLedBarDisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
