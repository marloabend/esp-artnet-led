# ESP Art-Net LED

## Modes

### Mode 0: Individual pixel control

| CH | Function        |
|---:|:----------------|
|  1 | Pixel 1 red     |
|  2 | Pixel 1 green   |
|  3 | Pixel 1 blue    |
|  4 | (Pixel 1 white) |
|  5 | Pixel 2 red     |
|  6 | ...             |

### Mode 1: Single uniform color

| CH | Function                                                                  |
|---:|:--------------------------------------------------------------------------|
|  1 | Red                                                                       |
|  2 | Green                                                                     |
|  3 | Blue                                                                      |
|  4 | White                                                                     |
|  5 | Intensity (this allows scaling a preset RGBW color with a single channel) |

### Mode 2: Two color mixing

| CH | Function                                             |
|---:|:-----------------------------------------------------|
|  1 | Color 1 red                                          |
|  2 | Color 1 green                                        |
|  3 | Color 1 blue                                         |
|  4 | Color 1 white                                        |
|  5 | Color 2 red                                          |
|  6 | Color 2 green                                        |
|  7 | Color 2 blue                                         |
|  8 | Color 2 white                                        |
|  9 | Intensity (this also allows to black out the colors) |
| 10 | Balance (between color 1 and color2)                 |

### Mode 3: Single uniform color blink

Blinking between the color and black

| CH | Function                                                   |
|---:|:-----------------------------------------------------------|
|  1 | Red                                                        |
|  2 | Green                                                      |
|  3 | Blue                                                       |
|  4 | White                                                      |
|  5 | Intensity                                                  |
|  6 | Speed (number of flashes per unit of time)                 |
|  7 | Ramp (whether there is a abrubt or more smooth transition) |
|  8 | Duty cycle (the time ratio between the color and black)    |

### Mode 4: Dual color blinking

Blinking between color 1 and color 2

| CH | Function      |
|---:|:--------------|
|  1 | Color 1 red   |
|  2 | Color 1 green |
|  3 | Color 1 blue  |
|  4 | Color 1 white |
|  5 | Color 2 red   |
|  6 | Color 2 green |
|  7 | Color 2 blue  |
|  8 | Color 2 white |
|  9 | Intensity     |
| 10 | Speed         |
| 11 | Ramp          |
| 12 | Duty cycle    |

### Mode 5: Single color slider

Segment can be moved along the array (between the edges)

| CH | Function                                                                    |
|---:|:----------------------------------------------------------------------------|
|  1 | Red                                                                         |
|  2 | Green                                                                       |
|  3 | Blue                                                                        |
|  4 | White                                                                       |
|  5 | Intensity                                                                   |
|  6 | Position (from 0-255 or 0-360 degrees, relative to the length of the array) |
|  7 | Width    (from 0-255 or 0-360 degrees, relative to the length of the array) |

### Mode 6: Dual color slider

Segment can be moved along the array (between the edges)

| CH | Function                                                                    |
|---:|:----------------------------------------------------------------------------|
|  1 | Color 1 red                                                                 |
|  2 | Color 1 green                                                               |
|  3 | Color 1 blue                                                                |
|  4 | Color 1 white                                                               |
|  5 | Color 2 red                                                                 |
|  6 | Color 2 green                                                               |
|  7 | Color 2 blue                                                                |
|  8 | Color 2 white                                                               |
|  9 | Intensity                                                                   |
| 10 | Position (from 0-255 or 0-360 degrees, relative to the length of the array) |
| 11 | Width    (from 0-255 or 0-360 degrees, relative to the length of the array) |

### Mode 7: Single color smooth slider

Segment can be moved along the array (continuous over the edge)

| CH | Function                                                                    |
|---:|:----------------------------------------------------------------------------|
|  1 | Red                                                                         |
|  2 | Green                                                                       |
|  3 | Blue                                                                        |
|  4 | White                                                                       |
|  5 | Intensity                                                                   |
|  6 | Position (from 0-255 or 0-360 degrees, relative to the length of the array) |
|  7 | Width    (from 0-255 or 0-360 degrees, relative to the length of the array) |
|  8 | Ramp     (from 0-255 or 0-360 degrees, relative to the length of the array) |

### Mode 8: Dual color smooth slider

Segment can be moved along the array (continuous over the edge)

| CH | Function                                                                    |
|---:|:----------------------------------------------------------------------------|
|  1 | Color 1 red                                                                 |
|  2 | Color 1 green                                                               |
|  3 | Color 1 blue                                                                |
|  4 | Color 1 white                                                               |
|  5 | Color 2 red                                                                 |
|  6 | Color 2 green                                                               |
|  7 | Color 2 blue                                                                |
|  8 | Color 2 white                                                               |
|  9 | Intensity                                                                   |
| 10 | Position (from 0-255 or 0-360 degrees, relative to the length of the array) |
| 11 | Width    (from 0-255 or 0-360 degrees, relative to the length of the array) |
| 12 | Ramp     (from 0-255 or 0-360 degrees, relative to the length of the array) |

### Mode 9: Spinning color wheel

| CH | Function  |
|---:|:----------|
|  1 | Red       |
|  2 | Green     |
|  3 | Blue      |
|  4 | White     |
|  5 | Intensity |
|  6 | Speed     |
|  7 | Width     |
|  8 | Ramp      |

### Mode 10: Spinning color wheel with color background

| CH | Function      |
|---:|:--------------|
|  1 | Color 1 red   |
|  2 | Color 1 green |
|  3 | Color 1 blue  |
|  4 | Color 1 white |
|  5 | Color 2 red   |
|  6 | Color 2 green |
|  7 | Color 2 blue  |
|  8 | Color 2 white |
|  9 | Intensity     |
| 10 | Speed         |
| 11 | Width         |
| 12 | Ramp          |

### Mode 11: Rainbow slider

| CH | Function   |
|---:|:-----------|
|  1 | Saturation |
|  2 | Value      |
|  3 | Position   |

### Mode 12: Rainbow spinner

| CH | Function   |
|---:|:-----------|
|  1 | Saturation |
|  2 | Value      |
|  3 | Speed      |

### Mode 13: Dual color letter

for 8x8 RGBW neopixel array

| CH | Function      |
|---:|:--------------|
|  1 | Color 1 red   |
|  2 | Color 1 green |
|  3 | Color 1 blue  |
|  4 | Color 1 white |
|  5 | Color 2 red   |
|  6 | Color 2 green |
|  7 | Color 2 blue  |
|  8 | Color 2 white |
|  9 | Intensity     |
| 10 | ASCII code    |

> May be removed in the future to be replaced by external visualizer

## Web API

The device settings can be updated via web interface or API endpoints 

- GET ```/discovery``` Read device discovery information
- GET ```/status``` Monitor device status
- GET ```/dir``` Index device filesystem
- GET ```/mode``` Mode JSON
- GET ```/channel``` Art-Net config JSON
- GET ```/config``` Device config JSON
- GET ```/config.json``` Original config.json file on device FS
- POST ```/mode```
  ```
  {
    mode: 0 // int (0..16)
  }
  ```
- POST ```/channel``` Update Art-Net config
  ```
  {
    universe: 1, // int
    offset:   0  // int
  }
  ```
- POST ```/config ``` Update device config
  ```
  {
    pixels:       24,    // int
    colorMapping: "GRB", // string
    white:        false, // boolean
    brightness:   255,   // int
    hsv:          false, // boolean
    reverse:      false, // boolean
    speed:        8,     // int
    split:        0      // int
  }
  ```
- POST ```/reconnect``` Reset WiFi config
- POST ```/default``` Restore factory defaults
- POST ```/reset``` Reboot device
