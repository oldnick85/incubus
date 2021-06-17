#ifndef CORESSTATISTIC_H
#define CORESSTATISTIC_H

#include "incubus_common.h"
#include "cores/messages/messages.h"
#include "media/mediasound.h"

struct sCoresStatistic
{
    sCoreStatisticGp    gp_stat;
    sCoreStatisticPh    ph_stat;
    sCoreStatisticSh    sh_stat;
    sCoreStatisticBh    bh_stat;
};

#endif // CORESSTATISTIC_H
