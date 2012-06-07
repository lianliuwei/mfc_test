#include "stdafx.h"

#define DEFINE_COMMAND_ID_STRING
#include "app/command_bar_ids.h"


const TCHAR* kCommandBarXML = _T("\
 <?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<Settings CompactMode=\"1\">\
  <CommandBars>\
    <CommandBars>\
      <CommandBar Class=\"CXTPRibbonBar\">\
        <Tabs>\
          <Tab Caption=\"示波器\" Id=")ID_BT_OK _T(">\
            <Groups>\
              <Group Id=\"2\" Caption=\"控制\">\
              <ControlGroupPopup Caption=\"控制\"/>\
                <Controls>\
                  <Control Class=\"CXTPControlButton\" Id=") ID_BT_START _T(" Icon=\"0\" Caption=\"开启\" TooltipText=\"示波器采集数据\" ToolBar=\"1\"/>\
                  <Control Class=\"CXTPControlButton\" Id=") ID_BT_STOP _T(" Icon=\"1\" Caption=\"停止\" TooltipText=\"示波器不采集数据\" ToolBar=\"1\"/>\
                </Controls>\
              </Group>\
            </Groups>\
          </Tab>\
        </Tabs>\
      </CommandBar>\
    </CommandBars>\
  </CommandBars>\
</Settings>");