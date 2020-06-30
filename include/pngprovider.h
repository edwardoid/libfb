#ifndef PNG_PROVIDER_H
#define PNG_POVIDER_H

#include <string>
#include <unordered_map>
#include <memory>

namespace aquabox
{
    class PNGProvider
    {
    public:
        std::shared_ptr<class PNGImage> get(std::string path);
    private:
        std::unordered_map<std::string, std::shared_ptr<class PNGImage>> m_cache;
    };
}

#endif // PNG_PROVIDER_H