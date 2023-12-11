#include "kanzispeaker.hpp"

using namespace kanzi;
const unsigned int FRAMERATE = 25;

// Creates the property editor info for the KanziSpeaker.
PropertyTypeEditorInfoSharedPtr KanziSpeaker::makeEditorInfo()
{
	return PropertyTypeEditorInfoSharedPtr(
		KZ_PROPERTY_TYPE_EDITOR_INFO(
			[]() -> PropertyTypeEditorInfo::AttributeDictionary {
				PropertyTypeEditorInfo::AttributeDictionary dict;
				dict.displayName = "KanziSpeaker";
				return dict;
			}()));
}

MessageType<KanziSpeaker::SeekMessageArguments> KanziSpeaker::VideoSeekMessage(kzMakeFixedString("KanziSpeaker.SeekMessage"),
	KZ_DECLARE_EDITOR_METADATA
	(
		metadata.displayName = "Seek Progress";
metadata["sendable"] = "True";
metadata["listenable"] = "False";
));

MessageType<KanziSpeaker::PlayMessageArguments> KanziSpeaker::VideoPlayMessage(kzMakeFixedString("KanziSpeaker.PlayMessage"),
	KZ_DECLARE_EDITOR_METADATA
	(
		metadata.displayName = "Play Video";
metadata["sendable"] = "True";
metadata["listenable"] = "False";
));
MessageType<KanziSpeaker::StopMessageArguments> KanziSpeaker::VideoStopMessage(kzMakeFixedString("KanziSpeaker.StopMessage"),
	KZ_DECLARE_EDITOR_METADATA
	(
		metadata.displayName = "Stop Video";
metadata["sendable"] = "True";
metadata["listenable"] = "False";
));
MessageType<KanziSpeaker::PauseMessageArguments> KanziSpeaker::VideoPauseMessage(kzMakeFixedString("KanziSpeaker.PauseMessage"),
	KZ_DECLARE_EDITOR_METADATA
	(
		metadata.displayName = "Pause Video";
metadata["sendable"] = "True";
metadata["listenable"] = "False";
));
MessageType<KanziSpeaker::StopMessageArguments> KanziSpeaker::onEndTriggerMessage(kzMakeFixedString("KanziSpeaker.onEndTrigger"),
	KZ_DECLARE_EDITOR_METADATA
	(
		metadata.displayName = "On Video Ended";
metadata["sendable"] = "False";
metadata["listenable"] = "True";
));

MessageType<KanziSpeaker::AudioMuteMessageArguments> KanziSpeaker::AudioMuteMessage(kzMakeFixedString("KanziSpeaker.AudioMuteMessage"),
	KZ_DECLARE_EDITOR_METADATA
	(
		metadata.displayName = "Mute Audio";
metadata["sendable"] = "True";
metadata["listenable"] = "False";
));

PropertyType<bool> KanziSpeaker::AudioMuteMessageArguments::IsMute(kzMakeFixedString("Message.KanziSpeaker.AudioMuteMessageArguments.IsMute"), false, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "This property is global and can only be set once for any plug-in.";
));

PropertyType<float> KanziSpeaker::SeekMessageArguments::Progress(kzMakeFixedString("Message.KanziSpeaker.SeekMessageArguments.Progress"), 0, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "Target progress which starts from 0 to 1.";
));

PropertyType<string> KanziSpeaker::PlayMessageArguments::FilePath(kzMakeFixedString("Message.KanziSpeaker.PlayMessageArguments.FilePath"), "", 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "Message video file playback file's path";
metadata.tooltip = "Video file playback file's path";
metadata.host = "Node:user";
));

PropertyType<KanziSpeaker::PlayState> KanziSpeaker::StateProperty(kzMakeFixedString("KanziSpeaker.StateProperty"), KanziSpeaker::PlayState::Stopped, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "State";
metadata.tooltip = "Player state";
metadata.host = "KanziSpeaker:automatic";
metadata.valueProvider = "Enum:Playing|4;Paused|5;Stopped|6";
));
PropertyType<bool> KanziSpeaker::AutoReplayProperty(kzMakeFixedString("KanziSpeaker.AutoReplayProperty"), false, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "Auto Replay";
metadata.tooltip = "Whether auto replay when ended";
metadata.host = "KanziSpeaker:auto";
));

PropertyType<float> KanziSpeaker::ProgressProperty(kzMakeFixedString("KanziSpeaker.ProgressProperty"), 0, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "Progress";
metadata.tooltip = "Current video/audio playback progress(0.0-1.0)";
metadata.host = "KanziSpeaker:auto";
));

PropertyType<int> KanziSpeaker::DurationProperty(kzMakeFixedString("KanziSpeaker.DurationProperty"), 0, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "Duration";
metadata.tooltip = "Video/Audio duration(ms)";
metadata.host = "KanziSpeaker:auto";
));


PropertyType<bool> KanziSpeaker::HWOpenProperty(kzMakeFixedString("KanziSpeaker.HWOpenProperty"), false, 0, false,
	KZ_DECLARE_EDITOR_METADATA(
		metadata.displayName = "HWOpen";
metadata.tooltip = "Open or Close Hardware Decoder";
metadata.host = "KanziSpeaker:auto";
));

// Creates a KanziSpeaker.
KanziSpeakerSharedPtr KanziSpeaker::create(Domain* domain, string_view name)
{
   auto enginePlugin = KanziSpeakerSharedPtr(new KanziSpeaker(domain, name));

   enginePlugin->initialize();

   return enginePlugin;

   	// KzVideoPlayerSharedPtr videoView2D = make_polymorphic_shared_ptr<Node>(new KzVideoPlayer(domain, name));

	// videoView2D->initialize();

	// return videoView2D;
}

// Initializes the created Kanzi Engine plugin.
// Kanzi node classes typically have a static create() member function, which creates the instance of a node,
// initializes it, and returns a shared pointer to the instance. To initialize an instance of a Kanzi Engine 
// plugin, in the create() function call the initialize() function on the instance of that Kanzi Engine plugin. 
// You must initialize a node in the initialize() function, not in the constructor.
void KanziSpeaker::initialize()
{
    // Initialize base class.
    Node2D::initialize();
	//setRenderable(false);

	addMessageHandler(VideoPlayMessage, this, &KanziSpeaker::onVideoPlay);
	addMessageHandler(VideoStopMessage, this, &KanziSpeaker::onVideoStop);
	addMessageHandler(VideoPauseMessage, this, &KanziSpeaker::onVideoPause);
	addMessageHandler(VideoSeekMessage, this, &KanziSpeaker::onVideoSeek);
	addMessageHandler(AudioMuteMessage, this, &KanziSpeaker::onAudioMute);
}

// KanziSpeaker::KanziSpeaker(Domain* domain, string_view name) :
// 	Node2D(domain, name)
// {


// }

void KanziSpeaker::onAttached()
{
	using std::swap;

	Domain* domain = getDomain();

	// Call parent class.
	Node2D::onAttached();

}

void KanziSpeaker::onDetached()
{
	// Call base class.
	Node2D::onDetached();
	delete m_ffmpegManager;
	m_ffmpegManager = nullptr;
}

void KanziSpeaker::renderSelfOverride(Renderer3D& renderer, CompositionStack& compositionStack, const Matrix3x3& baseTransform)
{
	Node2D::renderSelfOverride(renderer, compositionStack, baseTransform);
	//kzLogDebug(("renderSelfOverride"));
	if (m_ffmpegManager) {
		m_ffmpegManager->updateAutoReplay(getProperty(AutoReplayProperty));
		m_ffmpegManager->updateHwState(getProperty(HWOpenProperty));
		m_ffmpegManager->updataVideoInfo();
	}
}


Vector2 KanziSpeaker::measureOverride(Vector2 /*availableSize*/)
{
	Vector2 nodeSize;


	return nodeSize;
}


void KanziSpeaker::onVideoEnded(NodeSharedPtr receiver)
{
	//setProperty(StateProperty, PlayState::Stopped);
	KanziSpeaker::StopMessageArguments arg;
	receiver->dispatchMessage(KanziSpeaker::onEndTriggerMessage, arg);
}



void KanziSpeaker::onVideoPlay(PlayMessageArguments& message)
{

	if (!m_ffmpegManager) {
		m_ffmpegManager = new FFmpegManager(shared_from_this(), bind(&KanziSpeaker::onVideoEnded, this, std::placeholders::_1), getProperty(AutoReplayProperty));
	
	}
	string videoFilename = message.getArgument(message.FilePath);

	m_ffmpegManager->open(videoFilename);

}

void KanziSpeaker::onVideoStop(StopMessageArguments& /*message*/)
{

	if (m_ffmpegManager) {
		m_ffmpegManager->stop();
	}

}

void KanziSpeaker::onVideoPause(PauseMessageArguments& /*message*/)
{
	if (m_ffmpegManager)
	{
		m_ffmpegManager->pause();
	}
	//setProperty(StateProperty, PlayState::Paused);

}

void KanziSpeaker::onVideoSeek(SeekMessageArguments& message)
{
	float progress = message.getArgument(message.Progress);
	if (m_ffmpegManager) {
		m_ffmpegManager->seek(progress);
	}
}


void KanziSpeaker::onAudioMute(AudioMuteMessageArguments& message)
{
	bool isMute = message.getArgument(message.IsMute);
	if (m_ffmpegManager) {
		m_ffmpegManager->setMuteState(isMute);
	}
}