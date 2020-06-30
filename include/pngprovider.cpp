#include "pngprovider.h"
#include "pngimage.h"

using namespace fb;

std::shared_ptr<PNGImage> PNGProvider::get(std::string path)
{
    auto pos = m_cache.find(path);
    if (pos != m_cache.end()) {
        return pos->second;
    }

    std::shared_ptr<PNGImage> p = std::make_shared<PNGImage>();
    if (!p->open(path)) {
        return nullptr;
    }
    m_cache.emplace(path, p);

    return m_cache[path];
}
