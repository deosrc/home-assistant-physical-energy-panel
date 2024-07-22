# Instructions

Below are the instructions for building your own energy panel. Note that these are intended as a guide only. You can and it is recommended to customise the build to your own preferences.

> :warning: The steps in these instructions may require you to wire components to mains electricity, or soldering of components to USB DC volatage. Performing steps incorrectly can result in short circuits, and fire. You should only continue if you are proficient in electronics. If you choose to continue, do so at your own risk.

## Materials

The physical components of the build are as follows:

- [IKEA SANNAHED Picture Frame](https://www.ikea.com/gb/en/p/sannahed-frame-black-60459118/)
- A piece of wood (~25cm square) for mounting components to.
- Circuit board stand-offs for mounting

The following electrical components are used:

- Double width solderboard
- Az-Delivery ESP32 Dev Board
- PIR Sensor
- 7-Segment Clock Display (x4)
- 10 LED Bar Display (x2)

Additionally, there are some 3D printed parts for mounting and to neaten the display.

## Physical Build

### Circuit Board

The LED displays I used were chosen due to them having backpacks for easier wiring and control.

Each 7-Segment or LED bar display has 4 wires:

- 3.3V (VCC)
- Ground (GND)
- Data (DIO)
- Clock (CLK)

> **TODO: Add example images**

> :warning: Always check the pins as the LED Bar and 7-Segment displays I used change the ordering of the pins.

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

> **TODO: Add images and link to 3D print files**

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

> **TODO**
