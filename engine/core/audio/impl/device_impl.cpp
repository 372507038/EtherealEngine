#include "device_impl.h"
#include "../logger.h"
#include "check.h"
#include <AL/alc.h>
#include <AL/alext.h>
#include <cstdint>
#include <sstream>

namespace audio
{
namespace priv
{
static bool has_efx(ALCdevice* dev)
{
	return (alcIsExtensionPresent(dev, "ALC_EXT_EFX") == AL_TRUE); // ALC_TRUE
}

static std::string info()
{
	std::stringstream ss;
	if(alGetString(AL_VERSION))
		ss << "OpenAL version: " << alGetString(AL_VERSION) << std::endl;
	if(alGetString(AL_RENDERER))
		ss << "OpenAL renderer: " << alGetString(AL_RENDERER) << std::endl;
	if(alGetString(AL_VENDOR))
		ss << "OpenAL vendor: " << alGetString(AL_VENDOR) << std::endl;
	if(alGetString(AL_EXTENSIONS))
		ss << "OpenAL extensions: " << alGetString(AL_EXTENSIONS) << std::endl;

	return ss.str();
}

device_impl::device_impl(int devnum)
{
	// device name
	auto devices = enumerate();
    log_info("Supported audio devices:");    
	for(const auto& dev : devices)
	{
		log_info(dev);
	}

	if(devnum >= 0 && devnum < int(devices.size()))
		_device_id = devices[std::size_t(devnum)];

	// select device
	_device = alcOpenDevice(_device_id.empty() ? 0 : _device_id.c_str());

	if(!_device)
	{
		log_error("Cant open audio device: " + _device_id);
		return;
	}
	bool has_efx_ = has_efx(_device);

	ALint attribs[4] = {0};
	attribs[0] = ALC_MAX_AUXILIARY_SENDS;
	attribs[1] = 4;

	// select device
	_context = alcCreateContext(_device, has_efx_ ? attribs : nullptr);

	if(!_context)
	{
		log_error("Cant create audio context for device: " + _device_id);
		return;
	}
	enable();

	alCheck(alDistanceModel(AL_LINEAR_DISTANCE));

	_info = info();

	log_info("Using audio device: " + _device_id);
}

device_impl::~device_impl()
{
	if(_context)
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(_context);
		_context = nullptr;
	}

	if(_device)
	{
		alcCloseDevice(_device);
		_device = nullptr;
	}
}

void device_impl::enable()
{
	alCheck(alcMakeContextCurrent(_context));
}

void device_impl::disable()
{
	alCheck(alcMakeContextCurrent(nullptr));
}

bool device_impl::is_valid() const
{
	return _device && _context;
}

const std::string& device_impl::get_device_id() const
{
	return _device_id;
}

const std::string& device_impl::get_info() const
{
	return _info;
}

std::vector<std::string> device_impl::enumerate()
{
	std::vector<std::string> vs;

	if(alcIsExtensionPresent(NULL, reinterpret_cast<const ALCchar*>("ALC_ENUMERATION_EXT")) == AL_TRUE)
	{
		const char* devices = reinterpret_cast<const char*>(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));

		while(std::string(devices).size())
		{
			vs.push_back(devices);
			devices += vs.back().size() + 1;
		}
	}

	return vs;
}
}
}