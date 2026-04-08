#ifndef RELIC_TYPES_H
#define RELIC_TYPES_H

#define MAX_RELIC_LEVEL 15
#define MAX_RELICS 4

typedef enum {
    RELIC_NONE = 0,
    RELIC_HEART,      // Health
    RELIC_SWORD,      // Damage
    RELIC_HOURGLASS,  // Attack Speed
    RELIC_LENS,       // Size
    RELIC_BOOTS,      // Movement
    RELIC_FANG        // LifeSteal
} RelicType;

typedef struct {
    RelicType type;
    int level;
} Relic;

#endif // RELIC_TYPES_H
