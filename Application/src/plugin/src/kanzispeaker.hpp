#ifndef KANZISPEAKER_HPP
#define KANZISPEAKER_HPP

// Use kanzi.hpp only when you are learning to develop Kanzi applications. 
// To improve compilation time in production projects, include only the header files of the Kanzi functionality you are using.
#include <kanzi/kanzi.hpp>
#include <string>
#include "ffmpeg_manager.hpp"


class KanziSpeaker;
typedef kanzi::shared_ptr<KanziSpeaker> KanziSpeakerSharedPtr;

// The template component.
class KANZISPEAKER_API KanziSpeaker : public kanzi::Node2D
{
public:
    enum PlayState {
		Playing=4,Paused, Stopped,
	};

    /// Message that allows one to play camera on 3D mesh.
	class PlayMessageArguments : public MessageArguments
	{
	public:
		KZ_MESSAGE_ARGUMENTS_METACLASS_BEGIN(PlayMessageArguments, MessageArguments, "KanziSpeaker.PlayMessageArguments")
			KZ_METACLASS_PROPERTY_TYPE(FilePath)
			KZ_METACLASS_END()

			static PropertyType<std::string> FilePath;
	};

    class StopMessageArguments : public MessageArguments
	{
	public:
		KZ_MESSAGE_ARGUMENTS_METACLASS_BEGIN(StopMessageArguments, MessageArguments, "Video navigation message arguments")
			KZ_METACLASS_END()
	};

    class PauseMessageArguments : public MessageArguments
	{
	public:
		KZ_MESSAGE_ARGUMENTS_METACLASS_BEGIN(PauseMessageArguments, MessageArguments, "Video paused message arguments")
			KZ_METACLASS_END()
	};

    class SeekMessageArguments : public MessageArguments
	{
	public:
		KZ_MESSAGE_ARGUMENTS_METACLASS_BEGIN(SeekMessageArguments, MessageArguments, "KanziSpeaker.SeekMessageArguments")
			KZ_METACLASS_PROPERTY_TYPE(Progress)
			KZ_METACLASS_END()

			static PropertyType<float> Progress;
	};

    class AudioMuteMessageArguments : public MessageArguments
	{
	public:
		KZ_MESSAGE_ARGUMENTS_METACLASS_BEGIN(AudioMuteMessageArguments, MessageArguments, "Audio  Control message arguments")
			KZ_METACLASS_PROPERTY_TYPE(IsMute)
			KZ_METACLASS_END()
		static PropertyType<bool> IsMute;
	};


    /// Message type for requesting video play.
	static MessageType<PlayMessageArguments> VideoPlayMessage;
	/// Message type for requesting video stop.
	static MessageType<StopMessageArguments> VideoStopMessage;
	/// Message type for requesting video pause.
	static MessageType<PauseMessageArguments> VideoPauseMessage;

	/// Message type for requesting video rewind.
	static MessageType<StopMessageArguments> onEndTriggerMessage;

	static MessageType<SeekMessageArguments> VideoSeekMessage;

	static MessageType<AudioMuteMessageArguments> AudioMuteMessage;

    /// Property type for filename of video file to play.
	/// Creates a video view node.
	/// \param domain The UI domain the new node belongs to.
	/// \param name Name of the trajectory layout.
	/// \return The created node.
	static KanziSpeakerSharedPtr create(Domain* domain, string_view name);
    static PropertyType<PlayState> StateProperty;
	static PropertyType<bool> AutoReplayProperty;
	static PropertyType<float> ProgressProperty;
	static PropertyType<int> DurationProperty;
	static PropertyType<bool> HWOpenProperty;

    KZ_METACLASS_BEGIN(KanziSpeaker, Node2D, "KanziSpeaker")
		KZ_METACLASS_MESSAGE_TYPE(VideoPlayMessage)
		KZ_METACLASS_MESSAGE_TYPE(VideoStopMessage)
		KZ_METACLASS_MESSAGE_TYPE(VideoPauseMessage)
		KZ_METACLASS_MESSAGE_TYPE(onEndTriggerMessage)
		KZ_METACLASS_MESSAGE_TYPE(VideoSeekMessage)
		KZ_METACLASS_MESSAGE_TYPE(AudioMuteMessage)
		KZ_METACLASS_PROPERTY_TYPE(StateProperty)
		KZ_METACLASS_PROPERTY_TYPE(AutoReplayProperty)
		KZ_METACLASS_PROPERTY_TYPE(ProgressProperty)
		KZ_METACLASS_PROPERTY_TYPE(DurationProperty)
		KZ_METACLASS_PROPERTY_TYPE(HWOpenProperty)
	KZ_METACLASS_END()

    static PropertyTypeEditorInfoSharedPtr makeEditorInfo();


protected:

    // Constructor.
    explicit KanziSpeaker(kanzi::Domain* domain, kanzi::string_view name):
        kanzi::Node2D(domain, name)
    {
        setRenderType(RenderTypeTexture);
    }

    // Initializes the created Kanzi Engine plugin.
    // Kanzi node classes typically have a static create() member function, which creates the instance of a node,
    // initializes it, and returns a shared pointer to the instance. To initialize an instance of a Kanzi Engine 
    // plugin, in the create() function call the initialize() function on the instance of that Kanzi Engine plugin. 
    // You must initialize a node in the initialize() function, not in the constructor.
    void initialize();

    /// Node::onAttached() implementation.
	virtual void onAttached() KZ_OVERRIDE;
	/// Node::onDetached() implementation.
	virtual void onDetached() KZ_OVERRIDE;

	/// Node2D::measureOverride() implementation.
	virtual Vector2 measureOverride(Vector2 /*availableSize*/) KZ_OVERRIDE;

	virtual void renderSelfOverride(Renderer3D& renderer, CompositionStack& compositionStack, const Matrix3x3& baseTransform);
	/// Handler of Message.KzVideoPlayer.Play message.
	void onVideoPlay(PlayMessageArguments& message);
	/// Handler of Message.KzVideoPlayer.Play message.
	void onVideoStop(StopMessageArguments& message);

	void onVideoPause(PauseMessageArguments& message);
	void onVideoSeek(SeekMessageArguments& message);

	void onVideoEnded(NodeSharedPtr receiver);

	void onAudioMute(AudioMuteMessageArguments& message);

private:
	string m_videoFilename;

	FFmpegManager *m_ffmpegManager = NULL;
};

#endif
