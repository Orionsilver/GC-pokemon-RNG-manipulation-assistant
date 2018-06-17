#include "MainWindow.h"

#include <vector>

#include <QHBoxLayout>
#include <QSettings>
#include <QVBoxLayout>

#include "GUICommon.h"
#include "SPokemonRNG.h"
#include "SeedFinder/SeedFinderWizard.h"
#include "Settings/DlgSettings.h"

MainWindow::MainWindow()
{
  initialiseWidgets();
  makeLayouts();
}

MainWindow::~MainWindow()
{
  delete m_system;
}

void MainWindow::initialiseWidgets()
{
  m_cmbGame = new QComboBox;
  m_cmbGame->addItems(GUICommon::gamesStr);
  m_cmbGame->addItem(tr("--Select your game--"));
  m_cmbGame->setCurrentIndex(static_cast<int>(GUICommon::gameSelection::Unselected));
  connect(m_cmbGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::gameChanged);

  m_btnSettings = new QPushButton(tr("Settings"));
  connect(m_btnSettings, &QPushButton::clicked, this, &MainWindow::openSettings);

  m_btnStartSeedFinder = new QPushButton(tr("Find your seed"));
  connect(m_btnStartSeedFinder, &QPushButton::clicked, this, &MainWindow::startSeedFinder);
  m_btnStartSeedFinder->setEnabled(false);

  m_btnReset = new QPushButton(tr("Reset"));
  connect(m_btnReset, &QPushButton::clicked, this, &MainWindow::resetPredictor);
  m_btnReset->setEnabled(false);

  m_chkFilterUnwantedPredictions = new QCheckBox(tr("Hide unwanted predictions"));
  m_chkFilterUnwantedPredictions->setChecked(false);
  connect(m_chkFilterUnwantedPredictions, &QCheckBox::stateChanged, this,
          [=](int state) { m_predictorWidget->filterUnwanted(state == Qt::Checked); });

  m_btnRerollPrediciton = new QPushButton(tr("Reroll"));
  connect(m_btnRerollPrediciton, &QPushButton::clicked, this, &MainWindow::rerollPredictor);
  m_btnRerollPrediciton->setEnabled(false);

  m_predictorWidget = new PredictorWidget(this);
}

void MainWindow::makeLayouts()
{
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(m_btnSettings);
  buttonsLayout->addWidget(m_btnStartSeedFinder);
  buttonsLayout->addWidget(m_btnReset);

  QHBoxLayout* filterUnwantedLayout = new QHBoxLayout;
  filterUnwantedLayout->addStretch();
  filterUnwantedLayout->addWidget(m_chkFilterUnwantedPredictions);
  filterUnwantedLayout->addStretch();

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_cmbGame);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addLayout(filterUnwantedLayout);
  mainLayout->addWidget(m_predictorWidget);
  mainLayout->addWidget(m_btnRerollPrediciton);

  QWidget* mainWidget = new QWidget;
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);
}

void MainWindow::gameChanged()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  if (selection == GUICommon::gameSelection::Colosseum)
    SPokemonRNG::getInstance()->switchGame(SPokemonRNG::GCPokemonGame::Colosseum);
  else if (selection = GUICommon::gameSelection::XD)
    SPokemonRNG::getInstance()->switchGame(SPokemonRNG::GCPokemonGame::XD);

  if (m_cmbGame->count() == static_cast<int>(GUICommon::gameSelection::Unselected) + 1)
  {
    m_cmbGame->removeItem(static_cast<int>(GUICommon::gameSelection::Unselected));
    m_btnStartSeedFinder->setEnabled(true);
  }
  m_btnReset->setEnabled(false);
  m_btnRerollPrediciton->setEnabled(false);
}

void MainWindow::startSeedFinder()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  QSettings settings("settings.ini", QSettings::IniFormat);
  int rtcMarginError = settings.value("generalSettings/finder/rtcMarginError", 5).toInt();
  bool useWii =
      settings
          .value("generalSettings/finder/platform", static_cast<int>(GUICommon::platform::GameCube))
          .toInt() == static_cast<int>(GUICommon::platform::Wii);
  SeedFinderWizard* wizard = new SeedFinderWizard(this, selection, rtcMarginError, useWii);
  if (wizard->exec() == QDialog::Accepted)
  {
    m_currentSeed = wizard->getSeeds()[0];
    std::vector<BaseRNGSystem::StartersPrediction> predictions =
        SPokemonRNG::getInstance()->getSystem()->predictStartersForNbrSeconds(
            m_currentSeed, settings.value("generalSettings/predictor/time", 10).toInt());
    m_predictorWidget->setStartersPrediction(predictions, selection);
    m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
    m_btnReset->setEnabled(true);
    m_btnRerollPrediciton->setEnabled(true);
  }
}

void MainWindow::resetPredictor()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  m_predictorWidget->resetPredictor(selection);
  m_btnReset->setEnabled(false);
  m_btnRerollPrediciton->setEnabled(false);
}

void MainWindow::rerollPredictor()
{
  std::vector<int> dummyCriteria;
  for (int i = 0; i < 6; i++)
    dummyCriteria.push_back(-1);

  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  QSettings settings("settings.ini", QSettings::IniFormat);
  SPokemonRNG::getInstance()->getSystem()->generateBattleTeam(m_currentSeed, dummyCriteria);
  std::vector<BaseRNGSystem::StartersPrediction> predictions =
      SPokemonRNG::getInstance()->getSystem()->predictStartersForNbrSeconds(
          m_currentSeed, settings.value("generalSettings/predictor/time", 10).toInt());
  m_predictorWidget->setStartersPrediction(predictions, selection);
  m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
}

void MainWindow::openSettings()
{
  DlgSettings* dlg = new DlgSettings(this);
  dlg->exec();
}
