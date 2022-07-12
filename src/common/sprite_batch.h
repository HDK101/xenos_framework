#ifndef SPRITE_BATCH
#define SPRITE_BATCH 

typedef struct SpriteRegionItem {
  float sx;
  float sy;
  float sw;
  float sh;
  float x;
  float y;
} SpriteRegionItem;
   
int sprite_batch_lua_init(Lstate);

#endif /* ifndef SPRITE_BATCH */
