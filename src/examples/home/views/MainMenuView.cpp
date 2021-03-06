#include "MainMenuView.h"

#include "../../settings/Settings.h"

#include "../../alarm/views/AlarmView.h"
#include "../../display/views/DisplayView.h"
#include "../../network/views/NetworkView.h"

MainMenuView::MainMenuView()
    : MenuListView({MenuItem("NIGHT_MODE", "Enter Night Mode"), MenuItem("ALARM", "Alarm"),
                    MenuItem("DISPLAY", "Display"), MenuItem("NETWORK", "Network"),
                    MenuItem("RESTART", "Restart")}) {
}

MainMenuView *MainMenuView::getInstance() {
  static MainMenuView instance;
  return &instance;
}

void MainMenuView::willMount() {
  Application.showStatusBar();
}

void MainMenuView::willUnmount() {
  Application.hideStatusBar();
}

void MainMenuView::didSelect() {
  auto menuItem = getItem(getSelectedIndex());
  String key = menuItem->key;
  if (key.equals("NIGHT_MODE")) {
    auto displaySettings = Settings.getDisplaySettings();
    displaySettings->isNightMode = !displaySettings->isNightMode;
    getItem(0)->text = displaySettings->isNightMode ? "Exit Night Mode" : "Enter Night Mode";
    Application.popView();
  } else if (key.equals("ALARM")) {
    Application.pushView(AlarmView::getInstance());
  } else if (key.equals("DISPLAY")) {
    Application.pushView(DisplayView::getInstance());
  } else if (key.equals("NETWORK")) {
    Application.pushView(NetworkView::getInstance());
  } else if (key.equals("RESTART")) {
    ESP.restart();
  }
}
