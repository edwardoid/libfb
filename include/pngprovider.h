#ifndef PNG_PROVIDER_H
#define PNG_PROVIDER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "pngimage.h"

namespace fb
{
    class PNGProvider
    {
    public:
        std::shared_ptr<PNGImage> get(std::string path);
    private:
        std::unordered_map<std::string, std::shared_ptr<PNGImage>> m_cache;
    };
}

#endif // PNG_PROVIDER_H