#include <cmath>    // For std::round if we want to round percentages
#include <iomanip>  // For std::fixed, std::setprecision
#include <iostream> // For input/output
#include <limits>   // For std::numeric_limits
#include <map>      // For std::map
#include <string>   // For std::string
#include <vector>   // For std::vector (if needed later)

// Define constants for clarity
const double PERCENT_DIVISOR = 100.0;
const double BASE_MULTIPLIER = 1.0; // Represents 100% of something

// --- Function Prototypes ---
// Passed a flag to control detailed printout for calculateDamage
double calculateDamage(double baseDamage, double weaponDamagePercent,
                       double totalWeaponDamagePercent,
                       double criticalDamagePercent,
                       double headshotDamagePercent,
                       double damagesToArmorPercent,
                       double damagesToHealthPercent,
                       double damagesTargetOutOfCoverPercent,
                       const std::map<std::string, double> &activeAmplifiers,
                       bool isHeadshot, bool isCritical, bool isAgainstArmor,
                       bool printDetails // NEW: Flag to control detailed output
);

// Helper function to get a percentage input safely
double getPercentageInput(const std::string &prompt);
// Helper function to get a double input safely
double getDoubleInput(const std::string &prompt);
// Helper function to clear input buffer in case of bad input
void clearInputBuffer();

// --- Main Program ---
int main() {
  std::cout << "--- The Division 2 Damage Calculator ---" << std::endl;

  // Use helper functions for input to make main cleaner and safer
  double baseDamage =
      getDoubleInput("Please input Base Damage of the weapon: (e.g 123456) ");
  double weaponDamagePercent =
      getPercentageInput("Please input Weapon Damage %: (e.g 15 for 15%) ");
  double totalWeaponDamagePercent = getPercentageInput(
      "Please input Total Weapon Damage %: (e.g 10 for 10%) ");
  double criticalHitChancePercent = getPercentageInput(
      "Please input Critical Hit Chance %: (e.g. 60 for 60%) ");
  double criticalDamagePercent = getPercentageInput(
      "Please input Critical Hit Damage %: (e.g 80 for 80%) ");
  double headshotDamagePercent =
      getPercentageInput("Please input Headshot Damage %: (e.g 50 for 50%) ");
  double damagesToArmorPercent =
      getPercentageInput("Please input Damage To Armor %: (e.g 10 for 10%) ");
  double damagesToHealthPercent =
      getPercentageInput("Please input Damage To Health %: (e.g 10 for 10%) ");
  double damagesTargetOutOfCoverPercent = getPercentageInput(
      "Please input Damage To Target Out Of Cover %: (e.g 10 for 10%) ");
  int magazineSize = static_cast<int>(
      getDoubleInput("Please input Magazine Size: (e.g 200) "));

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
      case 1:
        amplifierName = "Bleeding";
        break;
      case 2:
        amplifierName = "Shocked";
        break;
      case 3:
        amplifierName = "Burning";
        break;
      case 4:
        amplifierName = "Poisoned";
        break;
      case 5:
        amplifierName = "Confused";
        break;
      case 6:
        amplifierName = "Ensnared";
        break;
      default:
        std::cout << "Invalid choice. Skipping amplifier.\n";
        continue; // Skip to next iteration of the while loop
      }

      double amplifiedDamageValue =
          getPercentageInput("How much amplified? (e.g 10 for 10%) ");
      activeAmplifiers[amplifierName] +=
          amplifiedDamageValue; // Add to existing value
      std::cout << amplifierName << " amplifier (" << amplifiedDamageValue
                << "%) added. Total for " << amplifierName << ": "
                << activeAmplifiers[amplifierName] << "%\n";

    } else {
      std::cout << "Invalid input. Please enter 'y' or 'n'.\n";
    }
  }

  std::cout << "\n--- Base Damage Scenarios ---" << std::endl;
  std::cout << std::fixed << std::setprecision(0); // Display as whole numbers

  // Scenario 1: Base Body Shot, Non-Critical, Vs Armor, Out of Cover
  // Print details for the first detailed scenario
  double bodyArmorOOCDamage = calculateDamage(
      baseDamage, weaponDamagePercent, totalWeaponDamagePercent,
      criticalDamagePercent, headshotDamagePercent, damagesToArmorPercent,
      damagesToHealthPercent, damagesTargetOutOfCoverPercent, activeAmplifiers,
      false, false, true, // isHeadshot, isCritical, isAgainstArmor
      true                // printDetails = true for this one
  );
  std::cout << "1. Base Body Shot (Vs Armor, OOC): " << bodyArmorOOCDamage
            << std::endl;

  // Scenario 2: Critical Headshot, Vs Armor, Out of Cover
  // Print details for another detailed scenario
  double critHeadshotArmorOOCDamage = calculateDamage(
      baseDamage, weaponDamagePercent, totalWeaponDamagePercent,
      criticalDamagePercent, headshotDamagePercent, damagesToArmorPercent,
      damagesToHealthPercent, damagesTargetOutOfCoverPercent, activeAmplifiers,
      true, true, true, // isHeadshot, isCritical, isAgainstArmor
      true              // printDetails = true for this one
  );
  std::cout << "2. Crit Headshot (Vs Armor, OOC):  "
            << critHeadshotArmorOOCDamage << std::endl;

  // --- Magazine Expected Damage Calculation ---
  std::cout << "\n--- Magazine Expected Damage ---" << std::endl;
  std::cout << "Magazine Size: " << magazineSize
            << ", Critical Hit Chance: " << criticalHitChancePercent << "%\n";
  std::cout << "Assumptions for Average Damage: Body Shots, Vs Armor, Out of "
               "Cover.\n";

  // Calculate damage for a non-critical body shot (for expected normal hits)
  double expectedNormalHitDamage = calculateDamage(
      baseDamage, weaponDamagePercent, totalWeaponDamagePercent,
      criticalDamagePercent, headshotDamagePercent, damagesToArmorPercent,
      damagesToHealthPercent, damagesTargetOutOfCoverPercent, activeAmplifiers,
      false, // isHeadshot
      false, // isCritical
      true,  // isAgainstArmor
      false  // printDetails = false
  );

  // Calculate damage for a critical body shot (for expected critical hits)
  double expectedCriticalHitDamage = calculateDamage(
      baseDamage, weaponDamagePercent, totalWeaponDamagePercent,
      criticalDamagePercent, headshotDamagePercent, damagesToArmorPercent,
      damagesToHealthPercent, damagesTargetOutOfCoverPercent, activeAmplifiers,
      false, // isHeadshot
      true,  // isCritical
      true,  // isAgainstArmor
      false  // printDetails = false
  );

  // Calculate expected number of hits
  double expectedCriticalHits =
      magazineSize * (criticalHitChancePercent / PERCENT_DIVISOR);
  double expectedNormalHits = magazineSize - expectedCriticalHits;

  // Calculate total expected damage
  double totalExpectedDamage =
      (expectedCriticalHits * expectedCriticalHitDamage) +
      (expectedNormalHits * expectedNormalHitDamage);

  std::cout << "  Expected Critical Hits: "
            << std::round(expectedCriticalHits * 10) / 10.0 << " (out of "
            << magazineSize << " bullets)\n"; // Round to 1 decimal
  std::cout << "  Expected Normal Hits:   "
            << std::round(expectedNormalHits * 10) / 10.0 << " (out of "
            << magazineSize << " bullets)\n";
  std::cout << "  Total Expected Damage per Magazine: " << totalExpectedDamage
            << std::endl;

  return 0;
}

// --- Helper Function Definitions (Unchanged from previous response) ---

double getPercentageInput(const std::string &prompt) {
  double value;
  while (true) {
    std::cout << prompt;
    std::cin >> value;
    if (std::cin.fail()) {
      std::cout << "Invalid input. Please enter a number.\n";
      clearInputBuffer();
    } else if (value < 0) {
      std::cout
          << "Percentage cannot be negative. Please enter a positive number.\n";
      clearInputBuffer();
    } else {
      clearInputBuffer();
      return value;
    }
  }
}

double getDoubleInput(const std::string &prompt) {
  double value;
  while (true) {
    std::cout << prompt;
    std::cin >> value;
    if (std::cin.fail()) {
      std::cout << "Invalid input. Please enter a number.\n";
      clearInputBuffer();
    } else if (value <= 0) { // Base damage should typically be positive
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

// --- The Core Damage Calculation Logic (Updated for controlled output) ---
double calculateDamage(double baseDamage, double weaponDamagePercent,
                       double totalWeaponDamagePercent,
                       double criticalDamagePercent,
                       double headshotDamagePercent,
                       double damagesToArmorPercent,
                       double damagesToHealthPercent,
                       double damagesTargetOutOfCoverPercent,
                       const std::map<std::string, double> &activeAmplifiers,
                       bool isHeadshot, bool isCritical, bool isAgainstArmor,
                       bool printDetails // NEW parameter
) {
  // Convert percentages to decimals for multiplication (e.g., 10% -> 0.10)
  double wd_decimal = (weaponDamagePercent / PERCENT_DIVISOR);
  double twd_decimal = (totalWeaponDamagePercent / PERCENT_DIVISOR);
  double cd_decimal = (criticalDamagePercent / PERCENT_DIVISOR);
  double hd_decimal = (headshotDamagePercent / PERCENT_DIVISOR);
  double dta_decimal = (damagesToArmorPercent / PERCENT_DIVISOR);
  double dth_decimal = (damagesToHealthPercent / PERCENT_DIVISOR);
  double dtoc_decimal = (damagesTargetOutOfCoverPercent / PERCENT_DIVISOR);

  if (printDetails) {
    std::cout << "\n--- Calculation Details for this Specific Hit ---\n";
    std::cout << "Base Damage: " << baseDamage << std::endl;
    std::cout << "Weapon Damage Bonus: " << weaponDamagePercent << "%"
              << std::endl;
    std::cout << "Total Weapon Damage Bonus: " << totalWeaponDamagePercent
              << "%" << std::endl;
    std::cout << "Critical Hit Damage Bonus: " << criticalDamagePercent << "%"
              << std::endl;
    std::cout << "Headshot Damage Bonus: " << headshotDamagePercent << "%"
              << std::endl;
    std::cout << "Damage To Armor Bonus: " << damagesToArmorPercent << "%"
              << std::endl;
    std::cout << "Damage To Health Bonus: " << damagesToHealthPercent << "%"
              << std::endl;
    std::cout << "Damage To Target Out of Cover Bonus: "
              << damagesTargetOutOfCoverPercent << "%" << std::endl;
    std::cout << "Is Headshot: " << (isHeadshot ? "Yes" : "No") << std::endl;
    std::cout << "Is Critical: " << (isCritical ? "Yes" : "No") << std::endl;
    std::cout << "Is Against Armor: " << (isAgainstArmor ? "Yes" : "No")
              << std::endl;

    std::cout << "Active Amplifiers:\n";
    if (activeAmplifiers.empty()) {
      std::cout << "  No amplifiers provided.\n";
    } else {
      for (const auto &pair : activeAmplifiers) {
        std::cout << "  " << pair.first << ": " << pair.second << "%"
                  << std::endl;
      }
    }
    std::cout << "------------------------------------------\n";
  }

  double additiveWeaponDamagePoolMultiplier =
      BASE_MULTIPLIER * wd_decimal * twd_decimal;
  double currentDamage = baseDamage * additiveWeaponDamagePoolMultiplier;

  // 2. Damage to Target Out of Cover
  currentDamage *= (BASE_MULTIPLIER + dtoc_decimal);

  // 3. Damage to Armor OR Damage to Health
  if (isAgainstArmor) {
    currentDamage *= (BASE_MULTIPLIER + dta_decimal);
  } else {
    currentDamage *= (BASE_MULTIPLIER + dth_decimal);
  }

  // 4. Critical Hit Damage
  if (isCritical) {
    currentDamage *= (BASE_MULTIPLIER + cd_decimal);
  }

  // 5. Headshot Damage
  if (isHeadshot) {
    currentDamage *= (BASE_MULTIPLIER + hd_decimal);
  }

  // 6. Amplifiers
  double totalAmplifierBonusDecimal = 0;
  for (const auto &pair : activeAmplifiers) {
    totalAmplifierBonusDecimal += (pair.second / PERCENT_DIVISOR);
  }
  currentDamage *= (BASE_MULTIPLIER + totalAmplifierBonusDecimal);

  return currentDamage;
}
