#include "ffmpeg_manager.hpp"
#include "kanzispeaker.hpp"

map<uint64_t, FFmpegManager*> FFmpegManager::m_ffManagers;


FFmpegManager::FFmpegManager(NodeSharedPtr controller, onEndCallback callback, bool autoReplay)
{
	m_uiDomain = controller->getDomain();
	m_autoReplay = autoReplay;
	m_controller = controller;
	m_onEndCallback = callback;
	m_key = (uint64_t)this;
	m_ffManagers[m_key] = this;
	//kzLogDebug(("init ffmpegmanager:{}", m_key));
	m_taskDispatcher = m_uiDomain->getTaskDispatcher();
	init();
}

FFmpegManager::~FFmpegManager()
{
	if (mlp) {
		
		libvlc_media_list_player_stop(mlp);
		libvlc_media_list_player_release(mlp);
		if (mp)
		{
			libvlc_media_player_release(mp);
			mp = NULL;
		}
		mlp = NULL;
	}
	if (libvlc) 
	{
		libvlc_release(libvlc);
		libvlc = NULL;
	}

	if (m_data) 
	{
		delete m_data;
		m_data = nullptr;
	}

	if (m_ffManagers.find(m_key) != m_ffManagers.end()) {
		m_ffManagers.erase(m_key);
	}
}

void FFmpegManager::open(string file_path)
{
	if (NULL == libvlc) return;
	if (mlp && ( libvlc_media_list_player_is_playing(mlp) 
		|| (libvlc_media_list_player_get_state(mlp) == libvlc_state_t::libvlc_Paused))) return;

	ml = libvlc_media_list_new(libvlc);

	//libvlc_media_add_option(m, ":avcodec-hw=any");

	m_controller->setProperty(StandardMaterial::TextureProperty, m_videoTexture);
#if defined(ANDROID)
	file_path = "/sdcard/" + file_path;
#endif
	kzLogDebug(("file_path:{}", file_path));
	m = libvlc_media_new_path(libvlc, file_path.c_str());

	libvlc_media_list_add_media(ml, m);
	mlp = libvlc_media_list_player_new(libvlc);

	libvlc_media_release(m);
	
	if (m_isOpenHw) 
	{
		libvlc_media_add_option(m, ":avcodec-hw=any");
		libvlc_media_add_option(m, ":avcodec-threads=1");
	}
	else 
	{
		libvlc_media_add_option(m, ":avcodec-hw=none");
	}
	mp = libvlc_media_player_new(libvlc);

	libvlc_media_list_player_set_media_list(mlp, ml);
	libvlc_media_list_player_set_media_player(mlp, mp);
	if (m_autoReplay) 
	{
		libvlc_media_list_player_set_playback_mode(mlp, libvlc_playback_mode_loop);
	}
	else 
	{
		libvlc_media_list_player_set_playback_mode(mlp, libvlc_playback_mode_default);
	}

	ctx.pixelBuffer = new uint8_t[m_video_width * m_video_height * 4];
	memset(ctx.pixelBuffer, 0, m_video_height* m_video_width * 4);
	ctx.key = m_key;
	libvlc_video_set_callbacks(mp, lock, unlock, display, &ctx);
	
	libvlc_video_set_format(mp, "RGBA", m_video_width, m_video_height, m_video_width * 4);
	setMuteState(m_isMute);
	libvlc_media_list_player_play(mlp);

	isEndRunning = false;
}

void FFmpegManager::pause()
{
	if (mlp) {
		libvlc_media_list_player_pause(mlp);
	}
}
	

void FFmpegManager::stop()
{
	if (mlp) {

		libvlc_media_list_player_stop(mlp);
		libvlc_media_list_player_release(mlp);
		if (mp)
		{
			libvlc_media_player_release(mp);
			mp = NULL;
		}
		mlp = NULL;
	}
	
	resetTexture();
	resetState();
}


void FFmpegManager::resetState()
{
	if (m_controller) {
		KanziSpeakerSharedPtr kzVideoPlayer = dynamic_pointer_cast<KanziSpeaker>(m_controller);
		if (kzVideoPlayer) {
			kzVideoPlayer->setProperty(KanziSpeaker::StateProperty, KanziSpeaker::PlayState::Stopped);
			kzVideoPlayer->setProperty(KanziSpeaker::ProgressProperty, 0.0f);
			kzVideoPlayer->setProperty(KanziSpeaker::DurationProperty, 0);
		}
	}
}

void FFmpegManager::seek(float pos)
{
	if (mp) {
		libvlc_media_player_set_position(mp, pos);
	}
}

void FFmpegManager::init()
{
	char const *vlc_argv[] = {
		
		// "--no-audio", // Don't play audio.
		 // "--quiet",
		 "--no-xlib", // Don't use Xlib.
		// "--no-video",
		// Apply a video filter.
		//"--video-filter", "sepia",
		//"--sepia-intensity=200"
	};

	int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

	libvlc = libvlc_new(vlc_argc, vlc_argv);
	if (NULL == libvlc) {
		printf("LibVLC initialization failure.\n");
		return;
	}

	initData();
	// kzsError res = kzsThreadLockCreate(&m_QueueLock); // chắc là khởi tạo mutex
	// if (res != KZS_SUCCESS) {
	// 	kzLogDebug(("LaneDisplay::LaneDisplay create lock failed!"));
	// }
}


void FFmpegManager::initData()
{
	m_video_width = m_controller->getWidth();
	m_video_height = m_controller->getHeight();

	m_data = new uint8_t[m_video_width * m_video_height * 4];

	memset(m_data, 0, m_video_width * m_video_height * 4);
	initTextureInUI(m_video_width, m_video_height);
}

void FFmpegManager::updateAutoReplay(bool autoReplay)
{
	if (m_autoReplay == autoReplay) return;
	m_autoReplay = autoReplay;
	if (m_autoReplay)
	{
		libvlc_media_list_player_set_playback_mode(mlp, libvlc_playback_mode_loop);
	}
	else
	{
		libvlc_media_list_player_set_playback_mode(mlp, libvlc_playback_mode_default);
	}
}

void FFmpegManager::updateHwState(bool isOpenHw)
{
	//TODO  hw has some exception
	if (m_isOpenHw == isOpenHw) return;
	m_isOpenHw = isOpenHw;
	if (mlp) {

		if(m)
		{
			libvlc_media_list_player_stop(mlp);
	
			if (m_isOpenHw)
			{
				libvlc_media_add_option(m, ":avcodec-hw=any");
				libvlc_media_add_option(m, ":avcodec-threads=1");
			}
			else
			{
				libvlc_media_add_option(m, ":avcodec-hw=none");
			}
			
			libvlc_media_list_player_play(mlp);
			seek(m_position);
		}
		
		
	}
	
}

void FFmpegManager::setMuteState(bool isMute) {
	//if (m_isMute == isMute) return;
	m_isMute = isMute;
	//kzLogDebug(("mute :{}", libvlc_audio_get_mute(mp)));
	if(mp) {
		libvlc_audio_set_mute(mp, isMute);
	}

	/*map<uint64_t, FFmpegManager*>::iterator iter;
	for (iter = m_ffManagers.begin(); iter != m_ffManagers.end(); iter++) {
		if (iter->second->mp) {
			kzLogDebug(("setMuteState:{}", isMute));
			libvlc_audio_set_mute(iter->second->mp, isMute);
			return;
		}
	}*/

}

void FFmpegManager::updataVideoInfo()
{
	if (mlp) {
		libvlc_state_t state = libvlc_media_list_player_get_state(mlp);
		if (m_controller) {
			KanziSpeakerSharedPtr kzVideoPlayer = dynamic_pointer_cast<KanziSpeaker>(m_controller);
			KanziSpeaker::PlayState playState = KanziSpeaker::PlayState::Stopped;
			if (kzVideoPlayer)
				switch (state)
				{
				case libvlc_Playing:
				case libvlc_Paused:
				case libvlc_Stopped:
					playState = (KanziSpeaker::PlayState)state;
					break;
				default:
					break;
				}
				kzVideoPlayer->setProperty(KanziSpeaker::StateProperty, playState);
		}
		//kzLogDebug(("state:{}", state));
		if (state == libvlc_state_t::libvlc_Ended && !isEndRunning && !m_autoReplay) {
			m_onEndCallback(m_controller);
			resetTexture();
			isEndRunning = true;
		}
	}

	if (mp) {

		float position = libvlc_media_player_get_position(mp); // 0 - 1
		//kzLogDebug(("position:{}", m_position)); 
		if (m_controller) {
			KanziSpeakerSharedPtr kzVideoPlayer =  dynamic_pointer_cast<KanziSpeaker>(m_controller);
			if(kzVideoPlayer)
			kzVideoPlayer->setProperty(KanziSpeaker::ProgressProperty, position);

		}
		m_position = position;
		int duration = libvlc_media_get_duration(m);
		if (m_duration != duration) {
			m_duration = duration;
			if (m_controller) {
				KanziSpeakerSharedPtr kzVideoPlayer = dynamic_pointer_cast<KanziSpeaker>(m_controller);
				if (kzVideoPlayer)
					kzVideoPlayer->setProperty(KanziSpeaker::DurationProperty, m_duration);
			}
		}
		//kzLogDebug(("duration:{}", m_duration));
	}
}


void FFmpegManager::initTexture(int width, int height)
{
	if (m_taskDispatcher)
	{
		m_taskDispatcher->submit([this, width, height] { initTextureInUI(width, height); });
	}
}

void FFmpegManager::initTextureInUI(int width, int height)
{
	Texture::CreateInfo2D info(width, height, GraphicsFormatR8G8B8A8_UNORM);
	info.memoryType = GPUResource::GpuAndRam;
	m_videoTexture.reset();
	m_videoTexture = Texture::create(m_uiDomain, info, "videotexture");
	m_controller->setProperty(StandardMaterial::TextureProperty, m_videoTexture);
}

void FFmpegManager::updateTexture(const kanzi::byte * data)
{
	if (m_taskDispatcher)
		m_taskDispatcher->submit(std::bind(&FFmpegManager::updateTextureInUI, this, data));
}

void FFmpegManager::updateTextureInUI(const kanzi::byte * data)
{
	if (m_videoTexture) {
		if (data ) {
			m_videoTexture->setData(data);
		}
	}
}

void FFmpegManager::resetTexture()
{
	Texture::CreateInfo2D info(1, 1, GraphicsFormatR8G8B8_UNORM);
	info.memoryType = GPUResource::GpuAndRam;

	TextureSharedPtr videoTexture = Texture::create(m_uiDomain, info, "videotexture");
	kzU8 color[3] = { 0 };
	videoTexture->setData(color);
	m_controller->setProperty(StandardMaterial::TextureProperty, videoTexture);
}



// VLC prepares to render a video frame.
void *FFmpegManager::lock(void *data, void **p_pixels) {

	
	struct context* c = static_cast<context*>(data);
	// kzsThreadLockAcquire(m_ffManagers[c->key]->m_QueueLock);
	m_ffManagers[c->key]->m_QueueLock.lock();
	*p_pixels = c->pixelBuffer;

	return NULL; // Picture identifier, not needed here.
}



// VLC just rendered a video frame.
void FFmpegManager::unlock(void *data, void *id, void *const *p_pixels) {
	struct context* c = static_cast<context*>(data);
	// kzsThreadLockRelease(m_ffManagers[c->key]->m_QueueLock);
	m_ffManagers[c->key]->m_QueueLock.unlock();
}

// VLC wants to display a video frame.
void FFmpegManager::display(void *data, void *id) {
	//kzLogDebug(("width:{} height:{}", libvlc_video_get_width(m_ffManager->mp), libvlc_video_get_height(m_ffManager->mp)));

	struct context* c = static_cast<context*>(data);
	const kanzi::byte* b = c->pixelBuffer;
	//kzLogDebug(("keyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy::{}", c->key));
	m_ffManagers[c->key]->updateTexture(b);
}