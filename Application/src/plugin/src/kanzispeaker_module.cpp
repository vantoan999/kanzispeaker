#include "kanzispeaker_module.hpp"
#include "kanzispeaker.hpp"

using namespace kanzi;

void KanziSpeakerModule::registerModule(Domain* domain)
{
    domain->registerModule<KanziSpeakerModule>("KanziSpeaker");
}

KanziSpeakerModule::MetaclassContainer KanziSpeakerModule::getMetaclassesOverride()
{
    MetaclassContainer metaclasses;
    metaclasses.push_back(KanziSpeaker::getStaticMetaclass());
    return metaclasses;
}

#if defined(KANZISPEAKER_API_EXPORT) || defined(ANDROID)

extern "C"
{

KANZISPEAKER_API Module* createModule(uint32_t /*kanziVersionMajor*/, uint32_t /*kanziVersionMinor*/)
{
    return new KanziSpeakerModule;
}

}

#endif
