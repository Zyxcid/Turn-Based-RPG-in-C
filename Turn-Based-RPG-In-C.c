// Created by Sha (Zyxcid)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INVENTORY_SIZE 10
#define HP_MAX 100
#define MP_MAX 20

// Player struct
struct storage {
    int gold;
    char inventory[INVENTORY_SIZE][39];
    int HP;
    int MP;
    char equipment[39];
} Player = {25, {}, 100, 20};

// Weapons struct
struct weapon {
    // Sword
    int sword_price;
    int sword_damage;
    // Bow
    int bow_price;
    int bow_damage;
    // Spear
    int spear_price;
    int spear_damage;
    // Staff
    int staff_price;
    int staff_damage;
    int mp_staff;
} Weapons = {20, 10, 20, 10, 40, 15, 50, 22, 2};

struct item { // There are HP-potion and MP-potion items in this game
    int hp_potion_price;
    int mp_potion_price;
} Item = {3, 2};

// Enemy struct, random data will be filled in this struct in the init_enemy function
struct enemy {
    char name[20];
    int HP;
    int damage;
    int reward;
} Enemy;

// Add a new enemy randomly
void init_enemy(struct enemy *enemy) {
    int random_enemy = rand() % 1000 + 1;  // Random number between 1 - 1000

    // Insert enemy data from here into the enemy struct
    if (random_enemy >= 1 && random_enemy <= 200) {
        strcpy(enemy->name, "Goblin");
        enemy->HP = 60;
        enemy->damage = 10;
        enemy->reward = 10;
    } else if (random_enemy >= 201 && random_enemy <= 400) {
        strcpy(enemy->name, "Slime");
        enemy->HP = 30;
        enemy->damage = 6;
        enemy->reward = 8;
    } else if (random_enemy >= 401 && random_enemy <= 600) {
        strcpy(enemy->name, "Undead");
        enemy->HP = 55;
        enemy->damage = 10;
        enemy->reward = 10;
    } else if (random_enemy >= 601 && random_enemy <= 800) {
        strcpy(enemy->name, "Skeleton");
        enemy->HP = 50;
        enemy->damage = 9;
        enemy->reward = 10;
    } else if (random_enemy >= 801 && random_enemy <= 950) {
        strcpy(enemy->name, "Golem");
        enemy->HP = 125;
        enemy->damage = 18;
        enemy->reward = 30;
    } else {
        strcpy(enemy->name, "Mimic");
        enemy->HP = 50;
        enemy->damage = 15;
        enemy->reward = 50;
    }
}

// Function to add an item to the inventory
void add_to_inventory(struct storage *Player, const char *item_name) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (strlen(Player->inventory[i]) == 0) {
            strcpy(Player->inventory[i], item_name);
            break;
        }
    }
}

int calculate_weapon_damage(struct weapon *Weapons) {
    int base_damage = 0;
    int random_number = rand() % 4 - 1; // Random number between -1 and 2

    // Basic damage adjusts according to the player's weapon choice
    if (strcmp(Player.equipment, "Sword") == 0) {
        base_damage = Weapons->sword_damage;
    } else if (strcmp(Player.equipment, "Bow") == 0) {
        base_damage = Weapons->bow_damage;
    } else if (strcmp(Player.equipment, "Spear") == 0) {
        base_damage = Weapons->spear_damage;
    } else if (strcmp(Player.equipment, "Staff") == 0) {
        base_damage = Weapons->staff_damage;
        if (Player.MP > 0) {
            printf("\tUsing %d mana\n", Weapons->mp_staff);
            Player.MP -= Weapons->mp_staff;
        } else {
            printf("Not enough mana!\n");
            return 0;
        }
    } else {
        strcpy(Player.equipment, "Fist");
        base_damage = 2; // Base damage for Fist
    }

    // Formula for total damage to make it more interesting with a random factor
    int total_damage = base_damage + random_number;
    if (total_damage < 0) {
        total_damage = 0; // Make damage non-negative
    }

    return total_damage;
}

int calculate_enemy_damage(struct enemy *enemy) {
    int base_damage = enemy->damage;
    int random_number = rand() % 4 - 1; // Random number between -1 and 2

    int total_damage = base_damage + random_number;
    if (total_damage < 0) {
        total_damage = 0;
    }

    return total_damage;
}

// Fight the enemy to avoid continuous gambling;)
void adventure(struct storage *Player, struct weapon *Weapons, struct enemy *enemy) {
    bool in_battle = true;
    printf("\n\tEncountered a %s! HP: %d\n", enemy->name, enemy->HP);

    while (in_battle) {
        printf("\n-- Battle Menu --\n");
        printf("HP: %d MP: %d\n", Player->HP, Player->MP);
        printf("1. Attack\n");
        printf("2. Use Item\n");
        printf("3. Flee\n");

        int battle_choice;
        printf("-> ");
        scanf("%d", &battle_choice);
        fflush(stdin);

        int player_damage = calculate_weapon_damage(Weapons);
        int enemy_damage = calculate_enemy_damage(enemy);

        switch (battle_choice) {
            // Attack
            case 1: {
                printf("\tYou attack %s with %s and deal %d damage!\n", enemy->name, Player->equipment, player_damage);
                enemy->HP -= player_damage;

                if (enemy->HP > 0) {
                    printf("\t%s attacks you and deals %d damage!\n", enemy->name, enemy_damage);
                    Player->HP -= enemy_damage;

                    if (Player->HP <= 0) {
                        printf("\n\n\t!! You lost the battle !!\n");
                        printf("\n\n\tCredit: Made by Sha\n");
                        printf("\n\n\n\tThanks for playing!\n\n");
                        exit(0);
                    }
                } else {
                    printf("\tYou have defeated %s!\n", enemy->name);
                    printf("\t+%d gold\n", enemy->reward);
                    Player->gold += enemy->reward; // Add reward gold to player's gold
                    in_battle = false;
                }
            } break;

            // Use Item
            case 2: {
                int item_count = 0;
                printf("Your items:\n");
                for (int i = 0; i < INVENTORY_SIZE; ++i) {
                    if (strlen(Player->inventory[i]) > 0) {
                        printf("%d. %s\n", i + 1, Player->inventory[i]);
                        item_count++;
                    }
                }
                if (item_count == 0) {
                    printf("Empty\n");
                    break;
                }

                printf("Choose an item to use\n");
                int item_choice;
                printf("-> ");
                scanf("%d", &item_choice);
                fflush(stdin);

                int use_hp_potion = 0;
                int use_mp_potion = 0;

                if (item_choice >= 1 && item_choice <= INVENTORY_SIZE &&
                    strlen(Player->inventory[item_choice - 1]) > 0) {
                    if (strcmp(Player->inventory[item_choice - 1], "HP-Potion") == 0) {
                        // Handling HP-Potion
                        if (HP_MAX == Player->HP) {
                            printf("You use an HP-potion even though your HP is already full!\n");
                        } else if (HP_MAX - Player->HP < 20) {
                            use_hp_potion = HP_MAX - Player->HP;
                            Player->HP += use_hp_potion;
                            printf("You use an HP-Potion and gain %d HP!\n", use_hp_potion);
                        } else {
                            Player->HP += 20;
                            printf("You use an HP-Potion and gain 20 HP!\n");
                        }

                        printf("Your HP: %d\n", Player->HP);
                        strcpy(Player->inventory[item_choice - 1], ""); // Remove the item from inventory after using
                    } else if (strcmp(Player->inventory[item_choice - 1], "MP-Potion") == 0) {
                        // Handling MP-Potion
                        if (MP_MAX == Player->MP) {
                            printf("Your MP is already full!\n");
                        } else if (MP_MAX - Player->MP < 5) {
                            use_mp_potion = MP_MAX - Player->MP;
                            Player->MP += use_mp_potion;
                            printf("You use an MP-Potion and gain %d MP!\n", use_mp_potion);
                        } else {
                            Player->MP += 5;
                            printf("You use an MP-Potion and gain 20 MP!\n");
                        }

                        printf("Your MP: %d\n", Player->MP);
                        strcpy(Player->inventory[item_choice - 1], ""); // Remove the item from inventory after using
                    } else {
                        printf("Item cannot be used!\n");
                    }
                } else {
                    printf("Invalid item choice!\n");
                }
            } break;

            // Flee
            case 3:
                // Another random number. Now I realize... life is full of gacha
                int random_number = rand() % 100 + 1;  // Random number between 1 - 1000

                if (random_number >= 1 && random_number <= 40) {
                    printf("\tYou successfully flee from the battle!\n");
                    in_battle = false; // Exit the loop
                } else if (random_number >= 41 && random_number <= 50) {
                    printf("\tSomehow lightning strikes %s!\n", enemy->name);
                    printf("\t+%d gold\n", enemy->reward + 2);
                    Player->gold += enemy->reward + 2;
                    in_battle = false;
                } else {
                    printf("\tFailed to flee!\n\n");
                    printf("\t%s attacks you and deals %d damage!\n", enemy->name, enemy_damage);
                    Player->HP -= enemy_damage;

                    if (Player->HP <= 0) {
                        printf("\n\n\t!! You lost the battle !!\n");
                        printf("\n\n\tCredit: Made by Sha\n");
                        printf("\n\n\n\tThanks for playing!\n");
                        exit(0);
                    }
                }
                break;

            default:
                printf("Enter a valid input\n");
        }
    }
}

void buy_weapon(struct storage *Player, struct weapon *weapons) {

	if (strlen(Player->equipment) != 0) {
		printf("You already have a %s!\n", Player->equipment);
		return;
	}

    printf("What weapon would you like to buy?\n");
    printf("1. Sword\tprice: %d gold\t\tbase damage: %d\n", weapons->sword_price, weapons->sword_damage);
    printf("2. Bow\t\tprice: %d gold\t\tbase damage: %d\n", weapons->bow_price, weapons->bow_damage);
    printf("3. Spear\tprice: %d gold\t\tbase damage: %d\n", weapons->spear_price, weapons->spear_damage);
    printf("4. Staff\tprice: %d gold\t\tbase damage: %d\t\t MP consumption: %d\n", weapons->staff_price, weapons->staff_damage, weapons->mp_staff);
    printf("5. Info\n");
    printf("9. Back\n");

    int weapon_choice;
    printf("-> ");
    scanf("%d", &weapon_choice);
    fflush(stdin);

    switch (weapon_choice) {
        case 1:
            if (Player->gold >= weapons->sword_price) {
                Player->gold -= weapons->sword_price;
                strcpy(Player->equipment, "Sword");
                printf("You have bought a SWORD\n");
            } else {
                printf("Not enough gold!\n");
            }
            break;
        case 2:
            if (Player->gold >= weapons->bow_price) {
                Player->gold -= weapons->bow_price;
                strcpy(Player->equipment, "Bow");
                printf("You have bought a BOW\n");
            } else {
                printf("Not enough gold!\n");
            }
            break;
        case 3:
            if (Player->gold >= weapons->spear_price) {
                Player->gold -= weapons->spear_price;
                strcpy(Player->equipment, "Spear");
                printf("You have bought a SPEAR\n");
            } else {
                printf("Not enough gold!\n");
            }
            break;
        case 4:
            if (Player->gold >= weapons->staff_price) {
                Player->gold -= weapons->staff_price;
                strcpy(Player->equipment, "Staff");
                printf("You have bought a STAFF\n");
            } else {
                printf("Not enough gold!\n");
            }
            break;
        case 5:
            printf("Total damage = base damage + random number (between -1 and 2)\n");
            break;
        case 9:
            break;
        default:
            printf("Enter a valid input\n");
    }
}

void buy_item(struct storage *Player, struct item *Item) {
    printf("What item would you like to buy?\n");
    printf("1. HP-Potion\tprice: %d gold\n", Item->hp_potion_price);
    printf("2. MP-Potion\tprice: %d gold\n", Item->mp_potion_price);
    printf("9. Back\n");

    int item_choice;
    printf("-> ");
    scanf("%d", &item_choice);
    fflush(stdin);

    switch (item_choice) {
        case 1:
            if (Player->gold >= Item->hp_potion_price) {
                Player->gold -= Item->hp_potion_price;
                add_to_inventory(Player, "HP-Potion");
                printf("You have bought an HP-Potion\n");
            } else {
                printf("Not enough gold!\n");
            }
            break;
        case 2:
            if (Player->gold >= Item->mp_potion_price) {
                Player->gold -= Item->mp_potion_price;
                add_to_inventory(Player, "MP-Potion");
                printf("You have bought an MP-Potion\n");
            } else {
                printf("Not enough gold!\n");
            }
            break;
        case 9:
            printf("Back\n");
            break;
        default:
            printf("Enter a valid input\n");
    }
}

// Stay away from gambling, embrace GACHA;)
void gacha(struct storage *Player) {
    int random_number = rand() % 1000 + 1;  // Random number from 1 to 1000

    // printf("Random number = %d\n", random_number); // cheat-proof GACHA
    
    printf("Gacha?\n");
    printf("1. Low-Risk Gacha\n");
    printf("2. High-Risk Gacha\n");
    printf("3. Info\n");
    printf("9. Back\n");

    int gacha_choice;
    printf("-> ");    
    scanf("%d", &gacha_choice);
    fflush(stdin);

    switch (gacha_choice) {
        case 1:
            if (Player->gold < 10) {
                printf("Not enough gold!\n");
                break;
            }
            printf("The wheel of fate is turning...\n");

            if (random_number >= 1 && random_number <= 500) {
                printf("Bad luck! -10 gold.\n");
                Player->gold -= 10;
            } else if (random_number >= 501 && random_number <= 900) {
                printf("+20 gold!\n");
                Player->gold += 20;
            } else {
                printf("Jackpot! +40 gold!\n");
                Player->gold += 40;
            }
            break;

        case 2:
            if (Player->gold < 40) {
                printf("Not enough gold!\n");
                break;
            }
            printf("The wheel of fate is turning...\n");

            if (random_number >= 1 && random_number <= 500) {
                printf("Bad luck! -40 gold.\n");
                Player->gold -= 40;
            } 
            else if (random_number >= 501 && random_number <= 900) {
                printf("+50 gold!\n");
                Player->gold += 50;
            } 
            else {
                printf("Jackpot! +100 gold!\n");
                Player->gold += 100;
            }
            break;

        case 3:
            printf("Information about gacha possibilities\n");
            printf("1. Low-Risk Gacha, cost: 10 gold, prize: 0-40 gold\n");
            printf("2. High-Risk Gacha, cost: 40 gold, prize: 0-100 gold\n");
            break;

        case 9:
            printf("Back\n");
            break;

        default:
            printf("Enter a valid input\n");
            break;
    }
}

void display_inventory(struct storage *Player) {
    printf("\n> Inventory <\n");
    printf("Weapons: %s\n", Player->equipment);
    printf("HP: %d\n", Player->HP);
    printf("MP: %d\n", Player->MP);
    printf("%d gold\n", Player->gold);
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (strlen(Player->inventory[i]) > 0) {
            printf("%d. %s\n", i + 1, Player->inventory[i]);
        }
    }

    char inventory_choice;
    printf("\n(Input 'a' to unequip weapon)\n(Press any button to continue)\n");
    printf("-> ");
    scanf("%c", &inventory_choice);
    fflush(stdin);

    switch (inventory_choice) {
        case 'a':
            strcpy(Player->equipment, "");
            printf("You unequip the weapon\n");
            break;
        default:
            printf("Back\n");
    }
}

// Save the player struct data as a form of progress in the game
void save_progress(struct storage *Player) {
    FILE *file = fopen("progress.txt", "w");
    if (file == NULL) {
        printf("Failed to create progress file.\n");
        return;
    }

    fprintf(file, "%d\n", Player->gold);
    fprintf(file, "%d\n%d\n", Player->HP, Player->MP);
    fprintf(file, "%s\n", Player->equipment);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fprintf(file, "%s\n", Player->inventory[i]);
    }

    printf("Progress saved successfully\n");
    
    fclose(file);
}

void load_progress(struct storage *Player) {
    FILE *file = fopen("progress.txt", "r");
    if (file == NULL) {
        printf("Failed to open progress file.\n");
        return;
    }

    fscanf(file, "%d\n", &Player->gold);
    fscanf(file, "%d\n%d\n", &Player->HP, &Player->MP);
    fscanf(file, "%s\n", Player->equipment);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fscanf(file, "%s\n", Player->inventory[i]);
    }

    printf("Progress loaded successfully\n");

    fclose(file);
}

int main() {
    int main_menu_choice, shop_choice;
    
    while (main_menu_choice != 9) {
        printf("\n- Main Menu -\n");
        printf("1. Shop\n");
        printf("2. Gacha\n");
        printf("3. Adventure\n");
        printf("4. Inventory\n");
        printf("5. Save\n");
        printf("6. Load\n");
        printf("9. Exit\n");

        printf("-> ");
        scanf("%d", &main_menu_choice);
        fflush(stdin);

        switch (main_menu_choice) {
            case 1:
                printf("\n- Shop Menu -\n");
                printf("1. Buy Weapons\n");
                printf("2. Buy Item\n");
                printf("9. Back\n");
                printf("-> ");
                scanf("%d", &shop_choice);
                fflush(stdin);

                switch (shop_choice) {
                    case 1:
                        buy_weapon(&Player, &Weapons);
                        break;
                    case 2:
                        buy_item(&Player, &Item);
                        break;
                    case 9:
                        break;
                    default:
                        printf("Enter a valid input!\n");
                }
                break;
            case 2:
                gacha(&Player);
                break;
            case 3:
                init_enemy(&Enemy); // Add a new enemy
                adventure(&Player, &Weapons, &Enemy);
                break;
            case 4:
                display_inventory(&Player);
                break;
            case 5:
                save_progress(&Player);
                break;
            case 6:
                load_progress(&Player);
                break;
            case 9:
                return 0;
            default:
                printf("Enter a valid input!\n");
        }
    }

    return 0;
}
