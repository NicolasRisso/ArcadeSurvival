#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <stdbool.h>

typedef struct {
    int maxHealth;
    int currentHealth;
    bool bIsDead;
} HealthComponent;

void HealthComponent_Init(HealthComponent* healthComp, int maxHealth);
void HealthComponent_TakeDamage(HealthComponent* healthComp, int amount);
void HealthComponent_Heal(HealthComponent* healthComp, int amount);

#endif // HEALTH_COMPONENT_H
