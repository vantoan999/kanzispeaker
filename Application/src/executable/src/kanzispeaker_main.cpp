// Use kanzi.hpp only when you are learning to develop Kanzi applications. 
// To improve compilation time in production projects, include only the header files of the Kanzi functionality you are using.
#include <kanzi/kanzi.hpp>
#include "vlc/vlc.h"

#if !defined(ANDROID) && !defined(KANZI_CORE_API_IMPORT)
#include <kanzispeaker_module.hpp>
// [CodeBehind libs inclusion]. Do not remove this identifier.

#if defined(KANZISPEAKER_CODE_BEHIND_API)
#include <KanziSpeaker_code_behind_module.hpp>
#endif

#endif

using namespace kanzi;

// Application class.
// Implements application logic.
class KanziSpeakerApplication : public ExampleApplication
{
protected:

    // Configures application.
    void onConfigure(ApplicationProperties& configuration) override
    {
        configuration.binaryName = "kanzispeaker.kzb.cfg";
    }

    // Initializes application after project has been loaded.
    void onProjectLoaded() override
    {
        libvlc_instance_t *inst;
        libvlc_media_player_t *mp;
        libvlc_media_t *m;

        char const* vlc_argv[] = {

            // "--no-audio", // Don't play audio.
             // "--quiet",
             "--no-xlib", // Don't use Xlib.
            // "--no-video",
            // Apply a video filter.
            //"--video-filter", "sepia",
            //"--sepia-intensity=200"
        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
        kzLogDebug(("vlc_argv = {}, vlc_argc = {}", vlc_argv, vlc_argc));
        // load the vlc engine
        inst = libvlc_new(vlc_argc, vlc_argv);

        if (inst == nullptr) {
            kzLogDebug(("ko khoi tao duoc instance"));
        }
        else {
            // create a new item
            //m = libvlc_media_new_path(inst, "christmas.mp3");
            m = libvlc_media_new_location(inst, "file:///ar.mp4");
        }


        if (!m) {
            kzLogDebug(("ko load duoc path file nhac"));
        }
        else {
            // create a media play playing environment
            mp = libvlc_media_player_new_from_media(m);

            // no need to keep the media now
            libvlc_media_release(m);

            // play the media_player
            libvlc_media_player_play(mp);

            // sleep(100);//play the audio 100s

            // stop playing
            //libvlc_media_player_stop(mp);

            //// free the media_player
            //libvlc_media_player_release(mp);

            //libvlc_release(inst);
        }
    }

    void registerMetadataOverride(ObjectFactory& factory) override
    {
        ExampleApplication::registerMetadataOverride(factory);

#if !defined(ANDROID) && !defined(KANZI_CORE_API_IMPORT)
        Domain* domain = getDomain();
        KanziSpeakerModule::registerModule(domain);

#if defined(KANZISPEAKER_CODE_BEHIND_API)
        KanziSpeakerCodeBehindModule::registerModule(domain);
#endif

        // [CodeBehind module inclusion]. Do not remove this identifier.
#endif
    }
};

// Creates application instance. Called by framework.
Application* createApplication()
{
    return new KanziSpeakerApplication();
}
