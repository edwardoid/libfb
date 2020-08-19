#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "framebuffer.h"
#include "pngprovider.h"
#include "pngimage.h"
#include "sprite.h"
#include "log.h"

<<<<<<< HEAD
using namespace LIBFB_NS;

FrameBuffer fb("/dev/fb1");
=======
using namespace aquabox;

FrameBuffer fb("/dev/fb0");
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
PNGProvider provider;

std::shared_ptr<Sprite> starting;
std::shared_ptr<Sprite> loading;
std::shared_ptr<Sprite> connecting;
std::shared_ptr<Sprite> normal;
std::shared_ptr<PNGImage> noInternet;
std::shared_ptr<PNGImage> apMode;
std::shared_ptr<PNGImage> bg;


<<<<<<< HEAD
std::shared_ptr<Drawable> current;
=======
std::shared_ptr<Pixmap> current;
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd


int main()
{
    fb.open();
    fb.clear(0xFFFFFF);
    bg = provider.get("./bg_blue.png");
<<<<<<< HEAD
    normal = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./normal_sprite.png")), 160, 128, 166); // 3ms
    starting = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./starting_up_89_sprite.png")), 160, 128, 89); // 3ms
    loading = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./loading_sprite.png")), 160, 128, 156); // 2ms
    connecting = std::make_shared<Sprite>(std::dynamic_pointer_cast<Drawable>(provider.get("./wifi_sprite.png")), 71, 100, 5); // 200ms
=======
    normal = std::make_shared<Sprite>(std::dynamic_pointer_cast<Pixmap>(provider.get("./normal_sprite.png")), 160, 128, 166); // 3ms
    starting = std::make_shared<Sprite>(std::dynamic_pointer_cast<Pixmap>(provider.get("./starting_up_89_sprite.png")), 160, 128, 89); // 3ms
    loading = std::make_shared<Sprite>(std::dynamic_pointer_cast<Pixmap>(provider.get("./loading_sprite.png")), 160, 128, 156); // 2ms
    connecting = std::make_shared<Sprite>(std::dynamic_pointer_cast<Pixmap>(provider.get("./wifi_sprite.png")), 71, 100, 5); // 200ms
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
    noInternet = provider.get("./no_internet.png");
    apMode = provider.get("./ap_mode.png");

    std::thread switcher([]() {

<<<<<<< HEAD
        std::vector<std::shared_ptr<Drawable>> images({
            std::dynamic_pointer_cast<Drawable>(starting),
            std::dynamic_pointer_cast<Drawable>(loading),
            std::dynamic_pointer_cast<Drawable>(connecting),
            std::dynamic_pointer_cast<Drawable>(normal),
            std::dynamic_pointer_cast<Drawable>(noInternet),
            std::dynamic_pointer_cast<Drawable>(apMode),
            std::dynamic_pointer_cast<Drawable>(bg)
=======
        std::vector<std::shared_ptr<Pixmap>> images({
            std::dynamic_pointer_cast<Pixmap>(starting),
            std::dynamic_pointer_cast<Pixmap>(loading),
            std::dynamic_pointer_cast<Pixmap>(connecting),
            std::dynamic_pointer_cast<Pixmap>(normal),
            std::dynamic_pointer_cast<Pixmap>(noInternet),
            std::dynamic_pointer_cast<Pixmap>(apMode),
            std::dynamic_pointer_cast<Pixmap>(bg)
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
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
<<<<<<< HEAD
                fb.draw(dynamic_cast<const Drawable*>(bg.get()));
            }
            if (current != nullptr) {
                fb.draw(current.get());
=======
                fb.drawPixmap(0, 0, dynamic_cast<Pixmap*>(bg.get()));
            }
            if (current != nullptr) {
                fb.drawPixmap(0, 0, current.get());
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
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