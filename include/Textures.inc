#ifndef BEGIN_TEXTURES
  #define BEGIN_TEXTURES(TYPE)
#endif
#ifndef TEXTURE
  #define TEXTURE(TYPE, NAME)
#endif
#ifndef NEXT_TEXTURE
  #define NEXT_TEXTURE(TYPE, NAME1, NAME2)
#endif
#ifndef END_TEXTURES
  #define END_TEXTURES(TYPE)
#endif

BEGIN_TEXTURES(Background)
TEXTURE(Background, Grass)
NEXT_TEXTURE(Background, Grass, Grass1)
TEXTURE(Background, Grass1)
END_TEXTURES(Background)

BEGIN_TEXTURES(LandscapeObjects)
TEXTURE(LandscapeObjects, WoodGainer)
END_TEXTURES(LandscapeObjects)

BEGIN_TEXTURES(Hero)
TEXTURE(Hero, HeroOnHorse)
END_TEXTURES(Hero)

BEGIN_TEXTURES(Arrow)
TEXTURE(Arrow, Straight)
NEXT_TEXTURE(Arrow, Straight, Stop)
TEXTURE(Arrow, Stop)
NEXT_TEXTURE(Arrow, Stop, Turn45Left)
TEXTURE(Arrow, Turn45Left)
NEXT_TEXTURE(Arrow, Turn45Left, Turn45Right)
TEXTURE(Arrow, Turn45Right)
NEXT_TEXTURE(Arrow, Turn45Right, Turn90Left)
TEXTURE(Arrow, Turn90Left)
NEXT_TEXTURE(Arrow, Turn90Left, Turn90Right)
TEXTURE(Arrow, Turn90Right)
NEXT_TEXTURE(Arrow, Turn90Right, Turn135Left)
TEXTURE(Arrow, Turn135Left)
NEXT_TEXTURE(Arrow, Turn135Left, Turn135Right)
TEXTURE(Arrow, Turn135Right)
END_TEXTURES(Arrow)


#undef BEGIN_TEXTURES
#undef TEXTURE
#undef NEXT_TEXTURE
#undef END_TEXTURES
