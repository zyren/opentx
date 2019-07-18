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

RadioToolsPage::RadioToolsPage():
  PageTab(STR_MENUTOOLS, ICON_RADIO_VERSION)  //TODO : need a new icon
{
}


void RadioToolsPage::build(FormWindow * window)
{
  FormGridLayout grid;
  grid.setLabelWidth(120);
  grid.spacer(PAGE_PADDING);

}
