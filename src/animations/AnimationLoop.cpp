#include "AnimationLoop.h"

AnimationLoop::AnimationLoop() {
  begin();
}

void AnimationLoop::onTick(AnimationTickEventHandler handler) {
  _onTick = handler;
}

void AnimationLoop::begin() {
  _state.updateStart = millis();
  _state.lastUpdate = millis();
  _state.timeBudget = 0;
}

int AnimationLoop::update() {
  unsigned long updateStart = millis();
  unsigned long lastUpdate = _state.lastUpdate;
  int ticksSinceLastUpdate = updateStart - _state.lastUpdate;
  int timeBudget = _options.updateInterval - ticksSinceLastUpdate;
  if (timeBudget <= 0) {
    _state.lastUpdate = _state.updateStart;
    _state.updateStart = updateStart;
    _state.ticksSinceLastUpdate = ticksSinceLastUpdate;
    _state.timeBudget = timeBudget;
    _fireTickEvent();
    // Recalculate time after tick event has done.
    _state.ticksSinceLastUpdate = millis() - lastUpdate;
    _state.timeBudget = _options.updateInterval - _state.ticksSinceLastUpdate;
    return _state.timeBudget;
  }
  _state.timeBudget = timeBudget;
  return _state.timeBudget;
}

void AnimationLoop::_fireTickEvent() {
  if (_onTick != NULL) {
    _onTick(this);
  }
}
