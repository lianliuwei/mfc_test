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
          <Tab Caption=\"ʾ����\" Id=")ID_BT_OK _T(">\
            <Groups>\
              <Group Id=\"2\" Caption=\"����\">\
              <ControlGroupPopup Caption=\"����\"/>\
                <Controls>\
                  <Control Class=\"CXTPControlButton\" Id=") ID_BT_START _T(" Icon=\"0\" Caption=\"����\" TooltipText=\"ʾ�����ɼ�����\" ToolBar=\"1\"/>\
                  <Control Class=\"CXTPControlButton\" Id=") ID_BT_STOP _T(" Icon=\"1\" Caption=\"ֹͣ\" TooltipText=\"ʾ�������ɼ�����\" ToolBar=\"1\"/>\
                </Controls>\
              </Group>\
            </Groups>\
          </Tab>\
        </Tabs>\
      </CommandBar>\
    </CommandBars>\
  </CommandBars>\
</Settings>");