# Instructions

Below are the instructions for building your own energy panel. Note that these are intended as a guide only. You can and it is recommended to customise the build to your own preferences.

> :warning: The steps in these instructions may require you to wire components to mains electricity, or soldering of components to USB DC volatage. Performing steps incorrectly can result in short circuits, and fire. You should only continue if you are proficient in electronics. If you choose to continue, do so at your own risk.

## Materials

The physical components of the build are as follows:

- [IKEA SANNAHED Picture Frame](https://www.ikea.com/gb/en/p/sannahed-frame-black-60459118/)
- A piece of wood (~25cm square) for mounting components to.
- Circuit board stand-offs for mounting

The following electrical components are used:

- [Double column prototype board](https://www.amazon.co.uk/gp/product/B07YSCGBL7)
- [AZ-Delivery ESP32 Dev Board](https://www.amazon.co.uk/AZDelivery-NodeMcu-CP2102-Development-including/dp/B071P98VTG)
  - [3 pack available](https://www.amazon.co.uk/AZDelivery-NodeMcu-CP2102-Development-including/dp/B074RGW2VQ) if you have multiple projects in mind
- [PIR Sensor](https://thepihut.com/products/breadboard-friendly-mini-pir-motion-sensor-with-3-pin-header)
- [4-Digit Display](https://thepihut.com/products/grove-4-digit-display) (x4)
- [10 LED Bar Display](https://thepihut.com/products/grove-led-bar-v2-0) (x2)
- [6 female grove headers](https://thepihut.com/products/grove-female-header-dip-4p-2-0mm-10-pack)\*
- [PCB Header strips](https://www.amazon.co.uk/gp/product/B07Q1XBGFB):
  - 19 pin female "DuPont" connector (x2)\*\*
  - 3 pin femail "DuPont" connector\*\*

\* The pins on the grove connector are not spaced wide enough for the prototype board, but can be "coerced" into fitting.

\*\* Due to availability, I used 20 pin and 4 pin connectors.

Additionally, there are some 3D printed parts for mounting and to neaten the display.

## Physical Build

### Circuit Board

The LED displays I used were chosen due to them having backpacks for easier wiring and control. Each 7-Segment or LED bar display has 4 wires:

- 3.3V (VCC)
- Ground (GND)
- Data (DIO)
- Clock (CLK)

> **TODO: Add example images**

> :warning: Always check the pins as the LED Bar and 7-Segment displays I used have different ordering of the pins.

If you are using grove headers, you may find they do not have the correct spacing to fit into the prototype board. **With a cable connected to the female ports**, the pins can be bent slightly to allow them to be inserted, and then soldered to the board. Always have a cable connected when banding the pins so that the connector end of the pins remain in position while being bent.

The only component which differs is the PIR sensor which does not have a clock. **IMPORTANT: The PIR sensor linked above does not label the pins. Be sure to check the datasheet if you are unsure.**

The circuit board is quite straightforward:

- The 3.3V pin on the ESP32 connects to the positive rail
- The GND pin on the ESP32 connects to the negative rail
- All VCC pins connect to the positive rail
- All GND pins connect to the negative rail
- The remaining pins should all be clock or data pins. They each connect to a GPIO pin on the ESP32. Try to avoid strapping pins where possible.

> **TODO: Add image and diagram **

### Display

The display is built into an [IKEA SANNAHED Picture Frame](https://www.ikea.com/gb/en/p/sannahed-frame-black-60459118/) picture frame, with a pieces of wood holding the display to the front of the frame. This provides ample space behind for the electrical components.

> **TODO: Add images**

I recommend designing your layout first. Remember to plan for where your circuit board will be behind the frame, as you will likely be unable to use this space for any of the displays. The design I used is available at .....

> **TODO: Add images and doc link above**

Once you have a design, you can make cutouts in the wood for your various display components. I opted to 3D print some shrouds for around the displays to hide the edges of the cutouts. If you cut the edges neatly and accurately, this can be skipped.

See [3D Printing README](./3d_printing/README.md) for details.

You may wish to use the perspex sheet provided with the picture frame to improve the finish of the panel. This may require a hole to be made for the PIR sensor to be mounted through. Be careful to avoid cracking the perspex when drilling the hole. It may be best to sandwich the perspex between 2 pieces of wood for drilling.

## Software

### Home Assistant

Home Assistant does not provide sensors for the components of the energy dashboard by default, but they can be added. You will need to create or have available entities for the following. Make a note of the entitiy IDs for:

- Grid Energy Used Today
- Grid Energy Returned Today
- Solar Energy Produced Today
- Home Energy Used Today
- Current Solar Power Generation
- Current Grid Power Usage (Negative value should indicate return to grid)

For real-time and cumulative grid energy usage/return, I use a [Shelly EM](https://shellystore.co.uk/product/shelly-em-120a/). **WARNING: This device must be wired to mains electricity.**

If the "Today" entities do not exist, you may be able to create them as "Utility Meter" helpers.

[![Open your Home Assistant instance and show your helper entities.](https://my.home-assistant.io/badges/helpers.svg)](https://my.home-assistant.io/redirect/helpers/)

For the Home Energy Used Today, if you have values for solar produced and grid used/return, a template sensor can be created using the following simple formula:

```text
<energy used by home> = (<solar produced> + <grid used>) - <grid returned>
```

### ESPHome

The ESPHome code uses a custom component for the LED bar as native suppport is not available.

The code has been split into 3 packages:

- **`display_auto_off`:** Provides various Home Assistant components for turning the display on and off, turning the display on when the PIR sensor detects motion, setting the time without motion until the display turns off, etc.
- **`energy_today`:** Provides the components for the numeric displays showing energy values for today.
- **`energy_now`:** Provides the components for real-time power flow indicators.

In theory, you can omit the `energy_now` package if you don't have real-time sensors.

In addition to these packages, you will need:

- The API component
- A script with ID `update_status`. This should execute the scripts `update_energy_today` and `update_energy_now` (if the package is included)
- On boot, the `auto_off` and `update_status` scripts should be called.

An example configuration is included in this repository.
