#include "needs_system.hpp"
#include "components.hpp"
#include "raymath.h"
#include <iostream>

NeedsSystem::NeedsSystem(entt::basic_registry<>* EntityRegistry) { sRegistry = EntityRegistry; }

void NeedsSystem::clampDesires(NeedsC& need, GenesC& genes)
{
	// clamp values
	for (int desireType = 0; desireType < 5; desireType++) {
		need.desires[desireType] = Clamp(need.desires[desireType], 0.0f, genes.maxDesires[desireType]);
	}
}

void NeedsSystem::handleCurrentAction(NeedsC& need, GenesC& genes)
{

	switch (need.currentAction) {
	case RESTING:
		need.desires[ENERGY] += genes.desireIncrements[ENERGY];
		break;
	case EATING:
		need.desires[SATIATION] += genes.desireIncrements[SATIATION];
		need.desires[ENERGY] += genes.desireIncrements[ENERGY];

		if (need.currentSubAction == SOCIALIZING) {
			need.desires[SOCIAL] += genes.desireIncrements[SOCIAL];
		}

		break;
	case SOCIALIZING:
		need.desires[SOCIAL] += genes.desireIncrements[SOCIAL];
		break;
	default:
		break;
	}
}

// set desire state and return new current desire
void NeedsSystem::setCurrentDesire(NeedsC& need, GenesC& genes)
{
	if (need.desires[ENERGY] <= 1.0f) {
		need.currentDesire = ENERGY;
	}
	else {
		// get min desire type
		int minDesireType = need.currentDesire;
		for (int desireType = 0; desireType < ENTERTAINMENT; desireType++) {
			// if less than min desires found so far and desire is under minimum gene desire threshold
			if ((need.desires[desireType] < need.desires[minDesireType]) && need.desires[desireType] <= genes.minDesires[desireType]) {
				minDesireType = desireType;
			}
		}

		need.currentDesire = minDesireType;
	}
}

void NeedsSystem::updateDesires(NeedsC& need, GenesC& genes)
{
	for (int desireType = 0; desireType < ENTERTAINMENT; desireType++) {
		need.desires[desireType] -= genes.desireDecrements[desireType];
	}

	setCurrentDesire(need, genes);
	handleCurrentAction(need, genes);
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
