#include "common/EnumBinderEx.h"

#include "resources/resource.h"

enum ChnlWaveCoupling {
  AC,
  DC,
};

BIND_ENUM_RES(ChnlWaveCoupling, EnumChnlWaveCoupling)
{
  ENUM_RES(AC, ID_STR_AC),
  ENUM_RES(DC, ID_STR_DC),
};