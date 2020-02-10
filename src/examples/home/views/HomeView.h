#pragma once

#include <Arduino.h>

#include "../../../espix-core.h"

class HomeView : public View {
public:
  HomeView();

  static HomeView *getInstance();

  bool shouldUpdate();
  void update();
  void didSelect();
  void willMount();
  void willUnmount();
  void render(CanvasContext *context);

private:
  String _timeString = "-- : -- : --";
};