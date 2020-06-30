#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "framebuffer.h"
#include "pngprovider.h"
#include "pngimage.h"
#include "sprite.h"
#include "log.h"

using namespace LIBFB_NS;

FrameBuffer fb("/dev/fb1");
PNGProvider provider;

std::shared_ptr<Sprite> starting;
std::shared_ptr<Sprite> loading;
std::shared_ptr<Sprite> connecting;
std::shared_ptr<Sprite> normal;
std::shared_ptr<PNGImage> noInternet;
std::shared_ptr<PNGImage> apMode;
std::shared_ptr<PNGImage> bg;


std::shared_ptr<Drawable> current;


int main()
{
    fb.open();
    fb.clear(0xFFFFFF);
    bg = provider.get("./bg_blue.png");
    normal = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./normal_sprite.png")), 160, 128, 166); // 3ms
    starting = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./starting_up_89_sprite.png")), 160, 128, 89); // 3ms
    loading = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./loading_sprite.png")), 160, 128, 156); // 2ms
    connecting = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./wifi_sprite.png")), 71, 100, 5); // 200ms
    noInternet = provider.get("./no_internet.png");
    apMode = provider.get("./ap_mode.png");

    std::thread switcher([]() {

        std::vector<std::shared_ptr<Drawable>> images({
            std::dynamic_pointer_cast<Drawable>(starting),
            std::dynamic_pointer_cast<Drawable>(loading),
            std::dynamic_pointer_cast<Drawable>(connecting),
            std::dynamic_pointer_cast<Drawable>(normal),
            std::dynamic_pointer_cast<Drawable>(noInternet),
            std::dynamic_pointer_cast<Drawable>(apMode),
            std::dynamic_pointer_cast<Drawable>(bg)
        });
        size_t i = 0;
        while(1) {
            current = images[i % images.size()];
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    });


    std::thread t([]() {
        uint32_t i = 0;

        while(1) {
            if (bg != nullptr) {
                fb.draw(dynamic_cast<const Drawable*>(bg.get()));
            }
            if (current != nullptr) {
                fb.draw(current.get());
            }
            auto s = std::dynamic_pointer_cast<Sprite>(current);
            if (s != nullptr) {
                s->jump(i % s->framesCount());
            }
            ++i;
            fb.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
        }
    });
    switcher.join();
    t.join();
    return 0;
}