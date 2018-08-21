#include "WiFiNetwork.h"

#include "../../espix-core/applications/Application.h"

WiFiNetwork::WiFiNetwork() {
}

bool WiFiNetwork::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

wl_status_t WiFiNetwork::getStatus() {
  return WiFi.status();
}

String WiFiNetwork::getLocalIP() {
  return WiFi.localIP().toString();
}

void WiFiNetwork::connect(WiFiConnectionSetting setting, bool showProgress,
                          NetworkConnectionCallback callback) {
  if (showProgress) {
    _getProgressView()->setMode(PROGRESS_MODE_INFINITY);
    _getProgressView()->setText("Connecting to WiFi...");
    Application::getInstance()->setRootView(_getProgressView());
  }
  static auto handler = WiFi.onStationModeGotIP([=](const WiFiEventStationModeGotIP &e) {
    if (showProgress) {
      _getProgressView()->setText("WiFi connected.");
    }
    WiFi.onStationModeGotIP(NULL);
    if (callback != NULL) {
      callback();
    }
  });
  WiFi.mode(WIFI_STA);
  WiFi.begin(setting.ssid.c_str(), setting.password.c_str());
}

void WiFiNetwork::disconnect(bool wifiOff) {
  WiFi.disconnect(wifiOff);
}

ProgressView *WiFiNetwork::_getProgressView() {
  if (_progressView == NULL) {
    _progressView = new ProgressView();
  }
  return _progressView;
}