#pragma once

namespace heroes::textures {
#define BEGIN_TEXTURES(TYPE) enum class TYPE {
#define TEXTURE(TYPE, NAME) NAME
#define NEXT_TEXTURE(TYPE, NAME1, NAME2) ,
#define END_TEXTURES(TYPE)                                                     \
  }                                                                            \
  ;
#include "Textures.inc"

} // namespace heroes::textures
