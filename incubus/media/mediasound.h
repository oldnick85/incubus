#ifndef MEDIASOUND_H
#define MEDIASOUND_H

#include <map>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alu.h>
#include <AL/alut.h>
#include "../incubus_common.h"
#include "coordinates.h"
#include "../param/physics_param.h"

struct sCoreStatisticShSound
{
    std::string print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "buf=%u; src=%u;", buffers, souces);
        return s;
    }

    uint_32     buffers = 0;
    uint_32     souces = 0;
};

enum enSoundType : uint_32
{
    stNone,
    stCollisionBullet,
    stShotPistol,
};

struct sSoundParam
{
    std::string Print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "%u; %s;", type, snd_pos.print().c_str());
        return s;
    }

    enSoundType type = stNone;
    sCoord2     snd_pos;
};

class CMediaSound
{
public:
    CMediaSound(sGlobalPhysicsParam* global_physics_param);
    ~CMediaSound();
    void SetListenerPos(const sCoord2 &pos);
    bool PlaySound(const sSoundParam &snd_param);
    void ClearSources();
    void GetStatistic(sCoreStatisticShSound &stat);

private:
    bool InitializeOpenAL();
    void DestroyOpenAL();
    bool InitSoundData();
    ALboolean CheckALCError();
    ALboolean CheckALError();
    bool LoadSoundFromFile(const std::string fname, const enSoundType stype);

    ALCdevice*              pDevice;
    ALCcontext*             pContext;
    sGlobalPhysicsParam*    m_global_physics_param;

    struct SndInfo
    {
      ALuint                buf_id;
      //std::string           Filename;
      Real_t                duration = 0.0;
      uint                  Rate;
      uint                  Format;
      std::vector<int_16>   data;           ///< массив сэмплов
    };

   std::map<enSoundType, SndInfo>    m_buffers;    ///< звуковые буферы - структуры, хранящие звуковые данные для воспроизведения

   struct sSoundSource
   {
       Real_t               time_end = 0;
       ALuint               src_id;
   };

   std::map<ALuint, sSoundSource>   m_sources;

   struct sRiffChunkDescr
   {
       uint_8   fmt[4];
       uint_32  size;
       uint_8   type[4];
   };

   struct sRiffSubchunkFmt
   {
       uint_8   fmt[4];
       uint_32  size;
       uint_16  audio_format;
       uint_16  num_channels;
       uint_32  sample_rate;
       uint_32  byte_rate;
       uint_16  block_align;
       uint_16  bits_per_sample;
   };

   struct sRiffSubchunkData
   {
       uint_8   fmt[4];
       uint_32  size;
   };

   bool LoadFileWav(const std::string fname, SndInfo &sinfo);
};

using CMediaSoundPtr = std::shared_ptr<CMediaSound>;

#endif // MEDIASOUND_H
