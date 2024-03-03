// Header
#include "view_bms.h"

// Includes
#include "main_window.h"

// UI Includes
#include "ui_view_bms.h"

// C Standard Libraries
#include <string.h>

// QT Libraries
#include <QLayout>
#include <QGridLayout>

ViewBms::ViewBms(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_) : View(parent_, mainWindow_, database_)
{
    // Create and apply the UI
    ui = new Ui::ViewBms;
    ui->setupUi(this);

    // Create center layout
    QGridLayout* layout = new QGridLayout(ui->frameCenter);
    layout->setSpacing(PADDING_CENTER_FRAME);

    cellStats        = new StatCell**[SEGMENT_COUNT];
    cellVoltages     = new double**  [SEGMENT_COUNT];
    cellTemperatures = new double**  [SEGMENT_COUNT];
    cellBalancings   = new bool**    [SEGMENT_COUNT];

    for(size_t segmentIndex = 0; segmentIndex < SEGMENT_COUNT; ++segmentIndex)
    {
        cellStats[segmentIndex]        = new StatCell*[CELLS_PER_SEGMENT];
        cellVoltages[segmentIndex]     = new double*[CELLS_PER_SEGMENT];
        cellTemperatures[segmentIndex] = new double*[CELLS_PER_SEGMENT / CELLS_PER_TEMPERATURE_SENSOR];
        cellBalancings[segmentIndex]   = new bool*[CELLS_PER_SEGMENT];

        for(size_t cellIndex = 0; cellIndex < CELLS_PER_SEGMENT; ++cellIndex)
        {
            char dataBuffer[DATABASE_KEY_LENGTH];

            cellStats[segmentIndex][cellIndex] = new StatCell(ui->frameCenter);

            sprintf(dataBuffer, CELL_VOLTAGE_N, segmentIndex * CELLS_PER_SEGMENT + cellIndex);
            cellVoltages[segmentIndex][cellIndex] = database->reference<double>(dataBuffer);

            sprintf(dataBuffer, CELL_BALANCING_N, segmentIndex * CELLS_PER_SEGMENT + cellIndex);
            cellBalancings[segmentIndex][cellIndex] = database->reference<bool>(dataBuffer);

            if(cellIndex % CELLS_PER_TEMPERATURE_SENSOR == 0)
            {
                size_t temperatureIndex = cellIndex / CELLS_PER_TEMPERATURE_SENSOR;
                sprintf(dataBuffer, CELL_TEMPERATURE_N, segmentIndex * CELLS_PER_SEGMENT / CELLS_PER_TEMPERATURE_SENSOR + temperatureIndex);
                cellTemperatures[segmentIndex][temperatureIndex] = database->reference<double>(dataBuffer);
            }

            layout->addWidget(cellStats[segmentIndex][cellIndex], segmentIndex, cellIndex);
        }
    }

    ui->frameCenter->setLayout(layout);

    // Connect GUI events
    connect(ui->buttonMenu, SIGNAL(clicked()), this, SLOT(handleButtonMenu()));
}

ViewBms::~ViewBms()
{
    // Delete UI
    delete ui;

    // TODO: Delete everything
}

void ViewBms::update()
{
    for(size_t segmentIndex = 0; segmentIndex < SEGMENT_COUNT; ++segmentIndex)
    {
        for(size_t cellIndex = 0; cellIndex < CELLS_PER_SEGMENT; ++cellIndex)
        {
            size_t temperatureIndex = cellIndex / CELLS_PER_TEMPERATURE_SENSOR;
            cellStats[segmentIndex][cellIndex]->set(*(cellVoltages[segmentIndex][cellIndex]), *(cellTemperatures[segmentIndex][temperatureIndex]), 0, *(cellBalancings[segmentIndex][cellIndex]));
        }
    }
}

void ViewBms::handleButtonMenu()
{
    mainWindow->setView(ID_VIEW_MENU);
}
