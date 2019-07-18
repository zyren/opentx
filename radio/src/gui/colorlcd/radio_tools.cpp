/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <algorithm>
#include "radio_tools.h"
#include "opentx.h"
#include "libopenui.h"

#define TOOL_NAME_MAXLEN  16

#if 1 //defined(LUA)
bool readToolName(const char * filename, char * name)
{
  FIL file;
  char buffer[1024];
  UINT count;

  if (f_open(&file, filename, FA_READ) != FR_OK) {
    return "Error opening file";
  }

  if (f_read(&file, &buffer, sizeof(buffer), &count) != FR_OK) {
    f_close(&file);
    return false;
  }

  const char * tns = "TNS|";
  auto * start = std::search(buffer, buffer + sizeof(buffer), tns, tns + 4);
  if (start >= buffer + sizeof(buffer))
    return false;

  start += 4;

  const char * tne = "|TNE";
  auto * end = std::search(buffer, buffer + sizeof(buffer), tne, tne + 4);
  if (end >= buffer + sizeof(buffer) || end <= start)
    return false;

  uint8_t len = end - start;
  if (len > TOOL_NAME_MAXLEN)
    return false;

  strncpy(name, start, len);
  memclear(name + len, TOOL_NAME_MAXLEN + 1 - len);

  return true;
}

bool isRadioScriptTool(const char * filename)
{
  const char * ext = getFileExtension(filename);
  return ext && !strcasecmp(ext, SCRIPT_EXT);
}
#endif

class ToolsButton : public Button {
  public:
    ToolsButton(Window * parent, const rect_t &rect, const char * toolname) :
      Button(parent, rect),
      toolname(toolname)
    {
      strcpy(msg, toolname);
    }

    virtual void paint(BitmapBuffer * dc) override
    {
      dc->drawText( 20, 0, msg, 0);

      // bounding rect
      drawSolidRect(dc, 0, 0, rect.w, rect.h, 2, hasFocus() ? SCROLLBOX_COLOR : CURVE_AXIS_COLOR);
    }

  protected:
    const char * toolname;
    char msg[20];
};

RadioToolsPage::RadioToolsPage() :
  PageTab(STR_MENUTOOLS, ICON_MODEL_OUTPUTS)
{
}

void RadioToolsPage::rebuild(FormWindow * window, int8_t focusChannel)
{
  coord_t scrollPosition = window->getScrollPositionY();
  window->clear();
  build(window, focusChannel);
  window->setScrollPositionY(scrollPosition);
}

void RadioToolsPage::build(FormWindow * window, int8_t focusChannel)
{
  FormGridLayout grid;
  grid.spacer(8);
  grid.setLabelWidth(66);
  uint8_t index=1;

#if 1 //defined(LUA)
  FILINFO fno;
  DIR dir;

#if defined(CROSSFIRE)
  if(isFileAvailable(SCRIPTS_TOOLS_PATH "/CROSSFIRE/crossfire.lua"))
    addRadioScriptTool(index++, SCRIPTS_TOOLS_PATH "/CROSSFIRE/crossfire.lua");
#endif

  FRESULT res = f_opendir(&dir, SCRIPTS_TOOLS_PATH);
  if (res == FR_OK) {
    for (;;) {
      TCHAR path[_MAX_LFN+1] = SCRIPTS_TOOLS_PATH "/";
      res = f_readdir(&dir, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fattrib & AM_DIR) continue;            /* Skip subfolders */
      if (fno.fattrib & AM_HID) continue;            /* Skip hidden files */
      if (fno.fattrib & AM_SYS) continue;            /* Skip system files */

      strcat(path, fno.fname);
      if (isRadioScriptTool(fno.fname)) {
        char toolName[TOOL_NAME_MAXLEN + 1];
        const char * label;
        char * ext = (char *)getFileExtension(path);
        if (readToolName(path, toolName)) {
          label = toolName;
        }
        else {
          *ext = '\0';
          label = getBasename(path);
        }
        strAppendUnsigned(reusableBuffer.radioTools.msg, index++);
        new StaticText(window, grid.getLabelSlot(), reusableBuffer.radioTools.msg, BUTTON_BACKGROUND | CENTERED);
        auto button = new ToolsButton(window, grid.getFieldSlot(1), label);
        if (!window->getFirstField())
          window->setFirstField(button);
        grid.nextLine();
      }
    }
  }
#endif

  grid.nextLine();

  window->setInnerHeight(grid.getWindowHeight());
}