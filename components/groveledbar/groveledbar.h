#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/core/time.h"

#include <vector>

namespace esphome {
namespace groveledbar {

class GroveLedBarDisplay;

using grove_led_bar_writer_t = std::function<void(GroveLedBarDisplay &)>;

class GroveLedBarDisplay : public PollingComponent {
 public:
  void set_writer(grove_led_bar_writer_t &&writer) { this->writer_ = writer; }

  void setup() override;

  void dump_config() override;

  void set_clk_pin(GPIOPin *pin) { clk_pin_ = pin; }
  void set_dio_pin(GPIOPin *pin) { dio_pin_ = pin; }

  float get_setup_priority() const override;

  void update() override;

  void set_led(uint8_t number, bool on);

  void set_on(bool on) { this->on_ = on; }

  void display();

 protected:
  void send_(uint16_t bits);
  void latch_();

  GPIOPin *dio_pin_;
  GPIOPin *clk_pin_;
  bool on_{true};
  optional<grove_led_bar_writer_t> writer_{};
  uint8_t led_[10];
};

}
}
