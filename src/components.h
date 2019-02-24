#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include <string>

#include "critterFactory.h"
#include "shapes.h"
#include "turretFactory.h"

#include "ecs.h"

#include "components/Bank.h"
#include "components/Color.h"
#include "components/ColorAnim.h"
#include "components/Critter.h"
#include "components/Cooldown.h"
#include "components/Dead.h"
#include "components/DeathTimer.h"
#include "components/EndWaveChecker.h"
#include "components/EndWaveDelay.h"
#include "components/FindTarget.h"
#include "components/Health.h"
#include "components/Investor.h"
#include "components/LineAnim.h"
#include "components/LineRenderer.h"
#include "components/Name.h"
#include "components/Physics.h"
#include "components/Placer.h"
#include "components/PointRenderer.h"
#include "components/Position.h"
#include "components/Resources.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"
#include "components/SizePulseAnim.h"
#include "components/Spawner.h"
#include "components/StaticLinesRenderer.h"
#include "components/Target.h"
#include "components/Targeter.h"
#include "components/TexCoord.h"
#include "components/TextRenderer.h"
#include "components/Traveler.h"
#include "components/Upgradable.h"
#include "components/Wave.h"
#include "components/WavesController.h"

#include "data.h"

static const float WAVE_START_TIME = -0.1f;

// Global/Player crap (FIX)
struct Player
{
    Entity cashLabel;
};

// Button crap (FIX)
struct CashThresholdButtonToggler
{
    float threshold;
};

typedef void(*ON_BUTTON_CLICK)(Entity entity);

struct Button {};

#endif
