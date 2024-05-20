#include "needs_system.hpp"
#include "components.hpp"
#include <iostream>

void HumanStates::findFood(NeedsC& need, GenesC& genes)
{

    if (need.search) {
        need.desires[ENERGY] -= genes.desireDecrements[ENERGY] * 3.0f;
    }
    else if (need.eating) {
        need.desires[SATIATION] += genes.desireIncrements[SATIATION];
        need.desires[ENERGY] += genes.desireIncrements[ENERGY] / 2.0f;
    }
}

void HumanStates::findFriends(NeedsC& need, GenesC& genes)
{
    if (need.search) {
        need.desires[ENERGY] -= genes.desireDecrements[ENERGY];
    }
    else if (need.social) {
        need.desires[SOCIAL] += genes.desireIncrements[SOCIAL];
    }
}

void HumanStates::rest(NeedsC& need, GenesC& genes)
{
    if (need.leisure) {
        need.desires[ENERGY] += genes.desireIncrements[ENERGY];
        need.desires[SATIATION] -= genes.desireIncrements[SATIATION] / 2.0f;
    }
}

NeedsSystem::NeedsSystem(entt::basic_registry<>* EntityRegistry) { sRegistry = EntityRegistry; }

void NeedsSystem::resetDesireStates(NeedsC& need)
{
    need.gather = false;
    need.eating = false;
    need.social = false;
    need.leisure = false;
}

void NeedsSystem::clampDesires(NeedsC& need, GenesC& genes)
{
    for (int desireType = 0; desireType < 5; desireType++) {
        // clamp values
        if (need.desires[desireType] <= 0.0f) {
            need.desires[desireType] = 0.0f;
        }
        else if (need.desires[desireType] >= genes.maxDesires[desireType]) {
            need.desires[desireType] = genes.maxDesires[desireType];
        }
    }
}

// set desire state and return new current desire
void NeedsSystem::setCurrentDesire(NeedsC& need, GenesC& genes)
{
    // get min desire type
    int minDesireType = ENERGY;
    if (need.desires[ENERGY] >= genes.minDesires[ENERGY]) { // if the npc has enough energy
        for (int desireType = 0; desireType < SAFETY; desireType++) {
            // if less than min desires found so far and desire is under minimum gene desire threshold
            if ((need.desires[desireType] < need.desires[minDesireType]) && need.desires[desireType] <= genes.minDesires[desireType]) {
                minDesireType = desireType;
            }
        }
    }

    need.currentDesire = minDesireType;
}

void NeedsSystem::updateDesires(NeedsC& need, GenesC& genes)
{

    setCurrentDesire(need, genes);

    switch (need.currentDesire) {
    case SATIATION:
        npcHuman.findFood(need, genes);
        break;
    case ENERGY:
        npcHuman.rest(need, genes);
        break;
    case SOCIAL:
        npcHuman.findFriends(need, genes);
        break;
    default:
        npcHuman.rest(need, genes);
        break;
    }

    clampDesires(need, genes);
}

void NeedsSystem::update()
{
    auto view = sRegistry->view<NeedsC, TimerC, GenesC>();

    for (auto id : view) {
        auto& need = view.get<NeedsC>(id);
        auto& gene = view.get<GenesC>(id);
        auto& timer = view.get<TimerC>(id);

        if (GetTime() - timer.lastTime >= 3.0f) {
            updateDesires(need, gene);
            timer.lastTime = GetTime();
        }
    }
}
