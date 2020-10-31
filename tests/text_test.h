
#pragma once

#include "text.h"
#include "rotate.h"

using namespace LIBFB_NS;

void text_test()
{
    for(int i = 0; i < 360; i += 36) {
        std::cout << "Angle " << i << std::endl;
        Text text;
        text.setDPI(96 * 2);
        text.setFont("./../tests/test_data/TimesNewRoman.ttf");
        text.setSize(24);
        text.mapTo(100, 100);
        text.setText("23·30");
        text.setAngle(double(90) * 0.01745329252);
        text.setColor(0xFF0000FF);
        std::cout << "\nDone " <<text.width() << "x" << text.height() << std::endl;
        break;
        text.dumpToImage(std::to_string(i) + ".png");
    }
}