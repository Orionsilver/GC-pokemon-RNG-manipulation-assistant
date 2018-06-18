#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVector>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../GUICommon.h"

class PredictorWidget : public QWidget
{
public:
  PredictorWidget(QWidget* parent = nullptr);
  void
  setStartersPrediction(const std::vector<BaseRNGSystem::StartersPrediction> startersPrediction,
                        const GUICommon::gameSelection game);
  void resetPredictor(const GUICommon::gameSelection currentGame);
  void filterUnwanted(const bool filterUnwanted);

private:
  const QBrush greenBrush = QBrush(QColor("#32CD32"));
  const QBrush redBrush = QBrush(QColor("#B22222"));

  void clearLabels();
  void initialiseWidgets();
  void makeLayouts();
  void switchGame(const GUICommon::gameSelection game);

  BaseRNGSystem::StartersPrediction m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QStringList m_tblHeaderLabels;
  QTableWidget* m_tblStartersPrediction;
};
