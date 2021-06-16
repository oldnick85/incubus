#include <iostream>
#include <fstream>

#include "mediasound.h"

CMediaSound::CMediaSound(sGlobalPhysicsParam *global_physics_param)
    : m_global_physics_param(global_physics_param)
{
    InitializeOpenAL();

    if (not InitSoundData())
        assert(false);
}

CMediaSound::~CMediaSound()
{
    DestroyOpenAL();
}

void CMediaSound::GetStatistic(sCoreStatisticShSound &stat)
{
    stat.buffers = static_cast<uint_32>(m_buffers.size());
    stat.souces = static_cast<uint_32>(m_sources.size());
}

bool CMediaSound::InitSoundData()
{
    LoadSoundFromFile("./resources/sound/shot_revolver.wav", stShotPistol);
    LoadSoundFromFile("./resources/sound/collision_bullet_flesh.wav", stCollisionBullet);
    return true;
}

bool CMediaSound::LoadSoundFromFile(const std::string fname, const enSoundType stype)
{
    SndInfo cl_buf;
    alGenBuffers(1, &cl_buf.buf_id);
    if (!CheckALError()) return false;

    LoadFileWav(fname, cl_buf);

    alBufferData(cl_buf.buf_id, AL_FORMAT_MONO16, cl_buf.data.data(),
                 static_cast<ALsizei>(cl_buf.data.size()*2), static_cast<ALsizei>(cl_buf.Rate));

    if (!CheckALError()) return false;

    m_buffers[stype] = cl_buf;
    return true;
}

bool CMediaSound::InitializeOpenAL()
{
    // Позиция слушателя.
    ALfloat ListenerPos[] = {0.0, 0.0, 0.0};
    // Скорость слушателя.
    ALfloat ListenerVel[] = {0.0, 0.0, 0.0};
    // Ориентация слушателя. (Первые 3 элемента – направление «на», последние 3 – «вверх»)
    ALfloat ListenerOri[] = {0.0, 0.0, -1.0,  0.0, -1.0, 0.0};

    // Открываем заданное по умолчанию устройство
    pDevice = alcOpenDevice(nullptr);
    // Проверка на ошибки
    if (!pDevice)
    {
        //ERRMSG("Default sound device not present");
        return false;
    }
    // Создаем контекст рендеринга
    pContext = alcCreateContext(pDevice, nullptr);
    if (!CheckALCError()) return false;

    // Делаем контекст текущим
    alcMakeContextCurrent(pContext);

    // Устанавливаем параметры слушателя
    // Позиция
    alListenerfv(AL_POSITION,    ListenerPos);
    // Скорость
    alListenerfv(AL_VELOCITY,    ListenerVel);
    // Ориентация
    alListenerfv(AL_ORIENTATION, ListenerOri);
    return true;
}

void CMediaSound::SetListenerPos(const sCoord2 &pos)
{
    ALfloat ListenerPos[] = {static_cast<ALfloat>(pos.x.Val()), static_cast<ALfloat>(pos.y.Val()), 0.0};
    alListenerfv(AL_POSITION, ListenerPos);
}

void CMediaSound::DestroyOpenAL()
{
    // Очищаем все буффера
    for (auto i = m_buffers.begin(); i != m_buffers.end(); i++)
        alDeleteBuffers(1, &i->second.buf_id);
    // Выключаем текущий контекст
    alcMakeContextCurrent(nullptr);
    // Уничтожаем контекст
    alcDestroyContext(pContext);
    // Закрываем звуковое устройство
    alcCloseDevice(pDevice);
}

ALboolean CMediaSound::CheckALCError()
{
    ALenum ErrCode;
    std::string Err = "ALC error: ";
    if ((ErrCode = alcGetError(pDevice)) != ALC_NO_ERROR)
    {
        Err += static_cast<const char*>(alcGetString(pDevice, ErrCode));
        //ERRMSG(Err.data());
        return AL_FALSE;
    }
    return AL_TRUE;
}

ALboolean CMediaSound::CheckALError()
{
    ALenum ErrCode;
    std::string Err = "OpenAL error: ";
    if ((ErrCode = alGetError()) != AL_NO_ERROR)
    {
        const char* e = static_cast<const char*>(alGetString(ErrCode));
        Err += e;
        //ERRMSG(Err.data());
        return AL_FALSE;
    }
    return AL_TRUE;
}

bool CMediaSound::PlaySound(const sSoundParam &snd_param)
{
    if (snd_param.type == stNone)
        return true;

    auto sd_it = m_buffers.find(snd_param.type);
    if (sd_it == m_buffers.end())
        return false;

    sSoundSource snd_src;
    alGenSources(1, &snd_src.src_id);
    if (not CheckALError())
        assert(false);

    ALfloat mVel[3] = {0.0, 0.0, 0.0};
    ALfloat mPos[3] = {static_cast<ALfloat>(snd_param.snd_pos.x.Val()), static_cast<ALfloat>(snd_param.snd_pos.y.Val()), 0.0};

    alSourcef (snd_src.src_id, AL_PITCH,    1.0f);      // тон звука (масштабирование звука по времени)
    alSourcef (snd_src.src_id, AL_GAIN,     5.0f);      // усиление звука. Этот параметр влияет на то, как будет изменяться сила звука, по мере изменения расстояния от источника до слушателя.
    alSourcefv(snd_src.src_id, AL_POSITION,  mPos);
    alSourcefv(snd_src.src_id, AL_VELOCITY,  mVel);
    alSourcei (snd_src.src_id, AL_LOOPING,  false);

    snd_src.time_end = m_global_physics_param->physics_time + sd_it->second.duration;
    m_sources[snd_src.src_id] = snd_src;

    alSourcei(snd_src.src_id, AL_BUFFER, static_cast<ALint>(sd_it->second.buf_id));

    alSourcePlay(snd_src.src_id);

    return true;
}

void CMediaSound::ClearSources()
{
    auto src_it = m_sources.begin();
    while (src_it != m_sources.end())
    {
        if (src_it->second.time_end <= m_global_physics_param->physics_time)
        {
            auto src_id = src_it->second.src_id;
            alSourceStop(src_id);
            if (alIsSource(src_id))
                alDeleteSources(1, &src_id);
            src_it = m_sources.erase(src_it);
            continue;
        }
        src_it++;
    }
}

bool CMediaSound::LoadFileWav(const std::string fname, SndInfo &sinfo)
{
    std::ifstream file;
    file.open(fname);
    if (not file.is_open())
        return false;
    // ================
    sRiffChunkDescr rcd;
    file.read(reinterpret_cast<char*>(&rcd), sizeof(rcd));
    if ((rcd.fmt[0] != 'R') or (rcd.fmt[1] != 'I') or (rcd.fmt[2] != 'F') or (rcd.fmt[3] != 'F'))
        return false;

    if ((rcd.type[0] != 'W') or (rcd.type[1] != 'A') or (rcd.type[2] != 'V') or (rcd.type[3] != 'E'))
        return false;
    // ================
    sRiffSubchunkFmt rsf;
    file.read(reinterpret_cast<char*>(&rsf), sizeof(rsf));

    if ((rsf.fmt[0] != 'f') or (rsf.fmt[1] != 'm') or (rsf.fmt[2] != 't') or (rsf.fmt[3] != ' '))
        return false;

    if (rsf.num_channels != 1)
        return false;

    if (rsf.bits_per_sample != 16)
        return false;

    if (rsf.audio_format != 1)
        return false;

    sinfo.Format = rsf.audio_format;
    sinfo.Rate = rsf.sample_rate;
    // ================
    sRiffSubchunkData rsd;
    file.read(reinterpret_cast<char*>(&rsd), sizeof(rsd));

    if ((rsd.fmt[0] != 'd') or (rsd.fmt[1] != 'a') or (rsd.fmt[2] != 't') or (rsd.fmt[3] != 'a'))
        return false;

    uint samples = rsd.size/2;

    sinfo.duration = Real_t(samples)/Real_t(sinfo.Rate);

    sinfo.data.resize(samples);

    file.read(reinterpret_cast<char*>(sinfo.data.data()), rsd.size);

    file.close();

    return true;
}
