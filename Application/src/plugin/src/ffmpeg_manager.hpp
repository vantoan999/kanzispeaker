#ifndef FFMPEGMANAGER_HPP
#define FFMPEGMANAGER_HPP


#include <kanzi/kanzi.hpp>
#include "vlc/vlc.h"
#include <mutex>
#include <thread>

using namespace kanzi;

typedef function<void(NodeSharedPtr)> onEndCallback;



class FFmpegManager {
public:

	struct context
	{
		uint64_t key;
		uint8_t* pixelBuffer;
	};
	FFmpegManager(NodeSharedPtr controller, onEndCallback callback, bool autoReplay);
	~FFmpegManager();
	
	void open(string file_path);

	void pause();

	void stop();

	void seek(float pos);

	void init();
	
	void initData();


	static void *lock(void *data, void **p_pixels);
	static void unlock(void *data, void *id, void *const *p_pixels);
	static void display(void *data, void *id);

	static map<uint64_t, FFmpegManager*> m_ffManagers;

	void updateAutoReplay(bool autoReplay);

	void updateHwState(bool isOpenHw);
	void setMuteState(bool isMute);
	void updataVideoInfo();
	void resetState();

	bool isEndRunning = false;
private:

	Domain* m_uiDomain;
	TextureSharedPtr m_videoTexture;
	NodeSharedPtr m_controller;
	onEndCallback m_onEndCallback;
	bool m_autoReplay;//自动重播

	context ctx;
	uint64_t m_key = 0;

	void initTexture(int width, int height);
	void initTextureInUI(int width, int height);
	void updateTexture(const kanzi::byte* data);
	void updateTextureInUI(const kanzi::byte* data);
	void resetTexture();
	TaskDispatcher *m_taskDispatcher;

	libvlc_instance_t *libvlc = NULL;
	libvlc_media_t *m = NULL;
	libvlc_media_player_t *mp = NULL;
	libvlc_media_list_t *ml = NULL;
	libvlc_media_list_player_t *mlp = NULL;

	int m_video_width = 0;
	int m_video_height = 0;
	std::mutex m_QueueLock;

	uint8_t* m_data = nullptr;

	float m_position = 0;
	int m_duration = 0;
	bool m_isOpenHw = false;
	bool m_isMute = false;
};
#endif
