#pragma once

#include "../BaseRNGSystem.h"

class ColosseumRNGSystem final : public BaseRNGSystem
{
public:
  enum QuickBattleTeamLeader
  {
    Blaziken = 0,
    Entei,
    Swampert,
    Raikou,
    Meganium,
    Suicune,
    Metagross,
    Heracross
  };

  enum QuickBattlePlayerName
  {
    Wes = 0,
    Seth,
    Thomas
  };

  enum SecondaryPokemonIndex
  {
    Quilava = 0,
    Croconaw,
    Bayleef,
    SECONDARY_COUNT
  };

  std::string getPrecalcFilename() final override;

private:
  u32 rollRNGToBattleMenu(u32 seed, u16* counter = nullptr) final override;
  bool generateBattleTeam(u32& seed, const std::vector<int> criteria) final override;
  std::vector<int> obtainTeamGenerationCritera(u32& seed) final override;
  int getNbrCombinationsFirstTwoCriteria() final override;
  int firstTwoCriteriaToIndex(const std::vector<int> criteria) final override;
  int getMinFramesAmountNamingScreen() final override;
  int getNbrStartersPrediction() final override;
  std::vector<std::string> getStartersName() final override;
  u32 rollRNGNamingScreenInit(u32 seed) final override;
  u32 rollRNGNamingScreenNext(u32 seed) final override;
  StartersPrediction generateStarterPokemons(u32 seed) final override;
  void generateAllSecondaryPokemonsInSearchRange(u32 postStarterSeed,
                                                 int secondaryIndex) final override;
  u32 generatePokemonPID(u32& seed, const u32 hTrainerId, const u32 lTrainerId, const u32 dummyId,
                         u16* counter = nullptr, const s8 wantedGender = -1,
                         const u32 genderRatio = 257, const s8 wantedNature = -1);
  u32 rollRNGToPokemonCompanyLogo(u32 seed, u16* counter = nullptr);
  u32 rollRNGEnteringBattleMenu(u32 seed, u16* counter = nullptr);

  const int secondaryRngAdvanceSearchStart = 1000000;
  const int secondarySearchSeedsAmount = 1000000;
  const int secondaryLevel = 30;
  const u8 secondaryGenderRatio = 31;

  Stats secondaryStats[3] = {{58, 64, 58, 80, 65, 80},  // Quilava
                             {65, 80, 80, 59, 63, 58},  // Croconaw
                             {60, 62, 80, 63, 80, 60}}; // Bayleef
};
