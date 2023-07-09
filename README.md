# PSPython
PSP Python Interpreter

## API Layout

All API calls are made through the `psp` module.
This module is the parent module for all submodules defined in the API.

### Color API

The `psp.color` module contains all color related functions.
This includes defines for 16 VGA colors, and a `create` function to create a color from RGB values.

The following are the constant defines for the 16 VGA colors:

```
BLACK
BLUE
GREEN
CYAN
RED
MAGENTA
YELLOW
LIGHT_GRAY
GRAY
LIGHT_BLUE
LIGHT_GREEN
LIGHT_CYAN
LIGHT_RED
LIGHT_MAGENTA
LIGHT_YELLOW
WHITE
```