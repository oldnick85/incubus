#ifndef CGUIELEMENTPROGRESSBAR_H
#define CGUIELEMENTPROGRESSBAR_H

#include "cores/common_show.h"
#include "cores/draw/drawprogressbar.h"

class CGuiElementProgressBar
        : public CDrawProgressBar
{
public:
    CGuiElementProgressBar(IDrawEnginePtr draw);
private:
};
using CGuiElementProgressBarPtr = std::shared_ptr<CGuiElementProgressBar>;

#endif // CGUIELEMENTPROGRESSBAR_H
