#pragma once

#include <Arduino.h>

#include <OLEDDisplay.h>

class Screen;

#define TEXT_ALIGN OLEDDISPLAY_TEXT_ALIGNMENT

enum FONT_SIZE { FONT_SIZE_H1 = 1, FONT_SIZE_H2 = 2, FONT_SIZE_NORMAL = 3 };

class DrawingContext {
public:
  // Do NOT use this constructor directly.
  // Use Screen.createDrawingContext() instead.
  DrawingContext(int width = 0, int height = 0, int offsetX = 0, int offsetY = 0);

  OLEDDisplay *getCanvas();

  void setDirty();

  void setColor(OLEDDISPLAY_COLOR color);

  // Get width of context
  int getWidth();

  // Get height of context
  int getHeight();

  // Set width and height of context
  void setSize(int width, int height);

  // Get offsetX of context
  int getOffsetX();

  // Get offsetY of context
  int getOffsetY();

  // Set offsetY of context
  void setOffset(int x = 0, int y = 0);

  // Specifies relative to which anchor point
  // the text is rendered. Available constants:
  // TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
  void setTextAlign(TEXT_ALIGN textAlign);

  // Change font
  void setFont(const uint8_t *fontData);

  // Sets the current font. Available default fonts
  // ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
  void setFontSize(FONT_SIZE size);

  // Draw a pixel at given position
  void drawPixel(int x, int y);

  // Draw a line from position 0 to position 1
  void drawLine(int x0, int y0, int x1, int y1);

  // Draw the border of a rectangle at the given location
  void drawRect(int x, int y, int width, int height);

  // Fill the rectangle
  void fillRect(int x, int y, int width, int height);

  // Draw the border of a circle
  void drawCircle(int x, int y, int radius);

  // Draw all Quadrants specified in the quads bit mask
  void drawCircleQuads(int x, int y, int radius, int quads);

  // Fill circle
  void fillCircle(int x, int y, int radius);

  // Draw a line horizontally
  void drawHorizontalLine(int x, int y, int length = -1);

  // Draw a line vertically
  void drawVerticalLine(int x, int y, int length = -1);

  // Draw a XBM picture
  void drawXBM(const uint8_t *xbm, int width, int height, int x = 0, int y = 0);

  // Draws a string at the given position
  void drawString(String text, int x = -1, int y = -1);

  // Get width of string
  int getStringWidth(String text);

  // Draws a String with a maximum width at the given location.
  // If the given String is wider than the specified width
  // The text will be wrapped to the next line at a space or dash
  void drawMultilineString(String text, int x = -1, int y = -1, int maxLineWidth = -1);

  // Clear buffer
  void clear();

private:
  Screen *_screen;

  int _x(int value);
  int _y(int value);

  int _width;
  int _height;
  int _offsetX;
  int _offsetY;
  TEXT_ALIGN _textAlign = TEXT_ALIGN_LEFT;
};
