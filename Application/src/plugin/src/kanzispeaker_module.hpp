#ifndef KANZISPEAKER_MODULE_HPP
#define KANZISPEAKER_MODULE_HPP

#include <kanzi/core/module/plugin.hpp>


class KANZISPEAKER_API KanziSpeakerModule : public kanzi::Plugin
{
public:

    static void registerModule(kanzi::Domain* domain);

protected:

    MetaclassContainer getMetaclassesOverride() override;
};

#endif
