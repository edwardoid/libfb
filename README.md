# libfb
**libfb** is a simple library to draw on linux framebuffer.
Following functionality is implemented
  - Draw single pixels
  - Draw png images (libpng)
  - Draw QR codes (libqrencode)
  - Draw text (libfreetype2)
  - Fill/clear

  Color interface is RGB888
# Important
This library is designed to be used without X11.
# Test environment
  - Orange PI Zero (any other device like Rapsberry PI should work as well)
  - 160*128 LCD SPI Display based on ST7735 16bit-depth color (should work on any other display)
  - Armbian Linux (shoudl work on any linux distro, no specific libs were used)
# Installation

Close the repo:
```sh
git clone https://github.com/edwardoid/libfb.git
cd libfb
mkdir tests
cd tests
cmake $PWD/..
make
make install
```
Now you can link fo libfb ```libfb.a``` and ```libfb.so```
To build examples:
```sh
cmake $PWD/.. -DBUILD_TEST=YES
make
```

# Example
```c++

// File: main.cpp

#include <thread>
#include <chrono>
#include "framebuffer.h"
#include "pngprovider.h"
#include "pngimage.h"
#include "sprite.h"
#include "log.h"

int main()
{
    IoT::FrameBuffer fb("/dev/fb0");
    IoT::PNGProvider provider;

    fb.open();
    fb.clear(0xFFFFFF);
    bg = provider.get("./bg_blue.png");
    auto sourceImg = provider.get("./normal_sprite.png");
    auto sp = std::make_shared<IoT::Sprite>(std::dynamic_pointer_cast<IoT::Pixmap>(sourceImpl),
                                            /* frame width: */ 160,
                                            /* frame height */ 128,
                                            /* frmaes count */ 166);
    while(1) {
        sp->jump(i % sp->framesCount());
        ++i;
        fb.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}
```

Compile with
```sh
g++ main.cpp -I<include dir path> -L<libraries directory> -lfb -lqrencode -lfreetype -I/usr/include/freetype2 -o testApp
```
Run:
```1sh
./testApp
```

