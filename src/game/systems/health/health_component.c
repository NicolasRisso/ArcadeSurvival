#include "game/systems/health/health_component.h"

void HealthComponent_Init(HealthComponent* healthComp, int maxHealth) {
    if (!healthComp) return;
    healthComp->maxHealth = maxHealth;
    healthComp->currentHealth = maxHealth;
    healthComp->bIsDead = false;
}

void HealthComponent_TakeDamage(HealthComponent* healthComp, int amount) {
    if (!healthComp || healthComp->bIsDead) return;
    
    healthComp->currentHealth -= amount;
    if (healthComp->currentHealth <= 0) {
        healthComp->currentHealth = 0;
        healthComp->bIsDead = true;
    }
}

void HealthComponent_Heal(HealthComponent* healthComp, int amount) {
    if (!healthComp || healthComp->bIsDead) return;
    
    healthComp->currentHealth += amount;
    if (healthComp->currentHealth > healthComp->maxHealth) {
        healthComp->currentHealth = healthComp->maxHealth;
    }
}
