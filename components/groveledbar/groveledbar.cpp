#include "groveledbar.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace groveledbar {

static const char *const TAG = "display.groveledbar";

void GroveLedBarDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Grove LED Bar...");

  this->clk_pin_->setup();
  this->clk_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
  this->clk_pin_->digital_write(false);
  this->dio_pin_->setup();
  this->dio_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
  this->dio_pin_->digital_write(false);

  this->display();
}
void GroveLedBarDisplay::dump_config() {
  ESP_LOGCONFIG(TAG, "Grove LED Bar:");
  LOG_PIN("  CLK Pin: ", this->clk_pin_);
  LOG_PIN("  DIO Pin: ", this->dio_pin_);
  LOG_UPDATE_INTERVAL(this);
}


void GroveLedBarDisplay::update() {
  if (this->writer_.has_value())
    (*this->writer_)(*this);
  this->display();
}

float GroveLedBarDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

void GroveLedBarDisplay::send_(uint16_t bits) {
  for (uint32_t i = 0, clk = 0; i < 16; i++) {
    this->dio_pin_->digital_write(bits & 0x8000 ? true : false);
    this->clk_pin_->digital_write(clk);
    clk = ~clk;
    bits <<= 1;
  }
}

void GroveLedBarDisplay::latch_() {
  this->dio_pin_->digital_write(false);
  this->clk_pin_->digital_write(true);
  this->clk_pin_->digital_write(false);
  this->clk_pin_->digital_write(true);
  this->clk_pin_->digital_write(false);
  delayMicroseconds(240);
  this->dio_pin_->digital_write(true);
  this->dio_pin_->digital_write(false);
  this->dio_pin_->digital_write(true);
  this->dio_pin_->digital_write(false);
  this->dio_pin_->digital_write(true);
  this->dio_pin_->digital_write(false);
  this->dio_pin_->digital_write(true);
  this->dio_pin_->digital_write(false);
  delayMicroseconds(240);
}

void GroveLedBarDisplay::display() {
  for (uint8_t i = 0; i < 10; i++)
  {
    ESP_LOGV(TAG, "LED %i: %02X", i, this->led_[i]);
  }

  // Chip has quirks with the latch being based on both clock signals and timing
  // See https://github.com/Makuna/NeoPixelBus/discussions/763
  // Latch, set, latch seems to give most consistent results
  this->latch_();

  if (this->on_)
  {
    for (uint32_t i = 0; i < 10; i++) {
      this->send_(this->led_[i]);
    }
    // Not sure why but 2 null commands seem to offset correctly so that all 10 LEDs are used.
    // Without this, the red and amber LEDs don't show.
    this->send_(0x00);
    this->send_(0x00);
    this->latch_();
  }
}

void GroveLedBarDisplay::set_led(uint8_t led, bool state) {
  if (state) {
    this->led_[led] = 0xFF;
  } else {
    this->led_[led] = 0x00;
  }
}


}  // namespace groveledbar
}  // namespace esphome
