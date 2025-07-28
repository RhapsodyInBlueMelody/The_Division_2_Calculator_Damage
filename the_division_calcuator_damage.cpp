#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

const double PERCENT_DIVISOR = 100.0;
const double BASE_MULTIPLIER = 1.0;

double calculateDamage(
    double baseDamage,
    double weaponDamagePercent,
    double totalWeaponDamagePercent,
    double criticalDamagePercent,
    double headshotDamagePercent,
    double damagesToArmorPercent,
    double damagesToHealthPercent,
    double damagesTargetOutOfCoverPercent,
    const std::map<std::string, double>& activeAmplifiers,
    bool isHeadshot,
    bool isCritical,
    bool isAgainstArmor,
    bool printDetails
);

double getPercentageInput(const std::string& prompt);
double getDoubleInput(const std::string& prompt);
void clearInputBuffer();

int main() {
    std::cout << "--- The Division 2 Damage Calculator ---" << std::endl;

    double baseDamage = getDoubleInput("Please input Base Damage of the weapon: (e.g 123456) ");
    double weaponDamagePercent = getPercentageInput("Please input Weapon Damage %: (e.g 15 for 15%) ");
    double totalWeaponDamagePercent = getPercentageInput("Please input Total Weapon Damage %: (e.g 10 for 10%) ");
    double criticalHitChancePercent = getPercentageInput("Please input Critical Hit Chance %: (e.g. 60 for 60%) ");
    double criticalDamagePercent = getPercentageInput("Please input Critical Hit Damage %: (e.g 80 for 80%) ");
    double headshotDamagePercent = getPercentageInput("Please input Headshot Damage %: (e.g 50 for 50%) ");
    double damagesToArmorPercent = getPercentageInput("Please input Damage To Armor %: (e.g 10 for 10%) ");
    double damagesToHealthPercent = getPercentageInput("Please input Damage To Health %: (e.g 10 for 10%) ");
    double damagesTargetOutOfCoverPercent = getPercentageInput("Please input Damage To Target Out Of Cover %: (e.g 10 for 10%) ");
    int magazineSize = static_cast<int>(getDoubleInput("Please input Magazine Size: (e.g 200) "));


    std::map<std::string, double> activeAmplifiers;

    std::string amplifierStatusChoice;
    while (true) {
        std::cout << "Add an Amplifier? (y/n): ";
        std::cin >> amplifierStatusChoice;
        clearInputBuffer();

        if (amplifierStatusChoice == "n" || amplifierStatusChoice == "N") {
            break;
        } else if (amplifierStatusChoice == "y" || amplifierStatusChoice == "Y") {
            std::cout << "What type of amplifier? "
                      << "\n [1] Bleeding" << "\n [2] Shocked" << "\n [3] Burning"
                      << "\n [4] Poisoned" << "\n [5] Confused" << "\n [6] Ensnared"
                      << "\nEnter number: ";
            int statusEffectChoice;
            std::cin >> statusEffectChoice;
            clearInputBuffer();

            std::string amplifierName;
            switch (statusEffectChoice) {
                case 1: amplifierName = "Bleeding"; break;
                case 2: amplifierName = "Shocked"; break;
                case 3: amplifierName = "Burning"; break;
                case 4: amplifierName = "Poisoned"; break;
                case 5: amplifierName = "Confused"; break;
                case 6: amplifierName = "Ensnared"; break;
                default:
                    std::cout << "Invalid choice. Skipping amplifier.\n";
                    continue;
            }

            double amplifiedDamageValue = getPercentageInput("How much amplified? (e.g 10 for 10%) ");
            activeAmplifiers[amplifierName] += amplifiedDamageValue;
            std::cout << amplifierName << " amplifier (" << amplifiedDamageValue << "%) added. Total for " << amplifierName << ": " << activeAmplifiers[amplifierName] << "%\n";

        } else {
            std::cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    }

    std::cout << std::fixed << std::setprecision(0);

    std::cout << "\n--- Damage Per Single Shot (Target Out of Cover) ---\n";

    double bodyArmorOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        false,
        false,
        true,
        true
    );
    std::cout << "1. Body Shot (Vs Armor):     " << bodyArmorOOCDamage << std::endl;

    double critBodyArmorOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        false,
        true,
        true,
        false
    );
    std::cout << "2. Critical Body Shot (Vs Armor): " << critBodyArmorOOCDamage << std::endl;

    double headshotArmorOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        true,
        false,
        true,
        false
    );
    std::cout << "3. Headshot (Vs Armor):      " << headshotArmorOOCDamage << std::endl;

    double critHeadshotArmorOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        true,
        true,
        true,
        false
    );
    std::cout << "4. Critical Headshot (Vs Armor): " << critHeadshotArmorOOCDamage << std::endl;

    std::cout << "\n--- Damage Per Single Shot (Target Out of Cover, Vs Health) ---\n";

    double bodyHealthOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        false,
        false,
        false,
        false
    );
    std::cout << "5. Body Shot (Vs Health):    " << bodyHealthOOCDamage << std::endl;

    double critBodyHealthOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        false,
        true,
        false,
        false
    );
    std::cout << "6. Critical Body Shot (Vs Health): " << critBodyHealthOOCDamage << std::endl;

    double headshotHealthOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        true,
        false,
        false,
        false
    );
    std::cout << "7. Headshot (Vs Health):     " << headshotHealthOOCDamage << std::endl;

    double critHeadshotHealthOOCDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        true,
        true,
        false,
        false
    );
    std::cout << "8. Critical Headshot (Vs Health): " << critHeadshotHealthOOCDamage << std::endl;


    std::cout << "\n--- Magazine Expected Damage ---" << std::endl;
    std::cout << "Magazine Size: " << magazineSize
              << ", Critical Hit Chance: " << criticalHitChancePercent << "%\n";
    std::cout << "Assumptions for Average Damage: Body Shots, Vs Armored Target, Out of Cover.\n";

    double expectedNormalHitDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        false,
        false,
        true,
        false
    );

    double expectedCriticalHitDamage = calculateDamage(
        baseDamage, weaponDamagePercent, totalWeaponDamagePercent, criticalDamagePercent,
        headshotDamagePercent, damagesToArmorPercent, damagesToHealthPercent,
        damagesTargetOutOfCoverPercent, activeAmplifiers,
        false,
        true,
        true,
        false
    );

    double expectedCriticalHits = magazineSize * (criticalHitChancePercent / PERCENT_DIVISOR);
    double expectedNormalHits = magazineSize - expectedCriticalHits;

    double totalExpectedDamage = (expectedCriticalHits * expectedCriticalHitDamage) +
                                 (expectedNormalHits * expectedNormalHitDamage);

    std::cout << std::setprecision(1);

    std::cout << "  Expected Critical Hits: "
              << expectedCriticalHits << " (out of "
              << magazineSize << " bullets)\n";
    std::cout << "  Expected Normal Hits:   "
              << expectedNormalHits << " (out of "
              << magazineSize << " bullets)\n";

    std::cout << std::setprecision(0);
    std::cout << "  Total Expected Damage per Magazine: " << totalExpectedDamage
              << std::endl;


    return 0;
}

double getPercentageInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
        } else if (value < 0) {
            std::cout << "Percentage cannot be negative. Please enter a positive number.\n";
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
        } else if (value <= 0) {
            std::cout << "Value must be positive. Please enter a positive number.\n";
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
}

double calculateDamage(
    double baseDamage,
    double weaponDamagePercent,
    double totalWeaponDamagePercent,
    double criticalDamagePercent,
    double headshotDamagePercent,
    double damagesToArmorPercent,
    double damagesToHealthPercent,
    double damagesTargetOutOfCoverPercent,
    const std::map<std::string, double>& activeAmplifiers,
    bool isHeadshot,
    bool isCritical,
    bool isAgainstArmor,
    bool printDetails
) {
    double wd_decimal = (weaponDamagePercent / PERCENT_DIVISOR);
    double twd_decimal = (totalWeaponDamagePercent / PERCENT_DIVISOR);
    double cd_decimal = (criticalDamagePercent / PERCENT_DIVISOR);
    double hd_decimal = (headshotDamagePercent / PERCENT_DIVISOR);
    double dta_decimal = (damagesToArmorPercent / PERCENT_DIVISOR);
    double dth_decimal = (damagesToHealthPercent / PERCENT_DIVISOR);
    double dtoc_decimal = (damagesTargetOutOfCoverPercent / PERCENT_DIVISOR);

    if (printDetails) {
        std::cout << "\n--- Calculation Details for this Specific Hit ---\n";
        std::cout << "Base Weapon Damage: " << baseDamage << std::endl;
        std::cout << "Weapon Damage Bonus (Gear/Watch/etc.): " << weaponDamagePercent << "%" << std::endl;
        std::cout << "Total Weapon Damage Bonus (Talents/Stats): " << totalWeaponDamagePercent << "%" << std::endl;
        std::cout << "Critical Hit Damage Bonus: " << criticalDamagePercent << "%" << std::endl;
        std::cout << "Headshot Damage Bonus: " << headshotDamagePercent << "%" << std::endl;
        std::cout << "Damage To Armor Bonus: " << damagesToArmorPercent << "%" << std::endl;
        std::cout << "Damage To Health Bonus: " << damagesToHealthPercent << "%" << std::endl;
        std::cout << "Damage To Target Out of Cover Bonus: " << damagesTargetOutOfCoverPercent << "%" << std::endl;
        std::cout << "Is Headshot (for this calculation): " << (isHeadshot ? "Yes" : "No") << std::endl;
        std::cout << "Is Critical (for this calculation): " << (isCritical ? "Yes" : "No") << std::endl;
        std::cout << "Is Against Armor (for this calculation): " << (isAgainstArmor ? "Yes" : "No") << std::endl;

        std::cout << "Active Amplifiers:\n";
        if (activeAmplifiers.empty()) {
            std::cout << "  No amplifiers provided.\n";
        } else {
            for (const auto& pair : activeAmplifiers) {
                std::cout << "  " << pair.first << ": " << pair.second << "%" << std::endl;
            }
        }
        std::cout << "------------------------------------------\n";
    }

    double currentDamage = baseDamage;

    currentDamage *= (BASE_MULTIPLIER + wd_decimal);

    currentDamage *= (BASE_MULTIPLIER + twd_decimal);

    currentDamage *= (BASE_MULTIPLIER + dtoc_decimal);

    if (isAgainstArmor) {
        currentDamage *= (BASE_MULTIPLIER + dta_decimal);
    } else {
        currentDamage *= (BASE_MULTIPLIER + dth_decimal);
    }

    if (isCritical) {
        currentDamage *= (BASE_MULTIPLIER + cd_decimal);
    }

    if (isHeadshot) {
        currentDamage *= (BASE_MULTIPLIER + hd_decimal);
    }

    double totalAmplifierBonusDecimal = 0;
    for (const auto& pair : activeAmplifiers) {
        totalAmplifierBonusDecimal += (pair.second / PERCENT_DIVISOR);
    }
    currentDamage *= (BASE_MULTIPLIER + totalAmplifierBonusDecimal);

    return currentDamage;
}
