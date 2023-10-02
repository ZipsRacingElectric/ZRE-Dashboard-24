#ifndef VIEW_BMS_H
#define VIEW_BMS_H

// BMS View -------------------------------------------------------------------------------------------------------------------
//
// Description: BMS monitoring tool
//
// Created: 23.09.04
// Updated: 23.09.04

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "view.h"
#include "stat_cell.h"

// QT Libraries
#include <QWidget>

// Database Keys --------------------------------------------------------------------------------------------------------------

#define CELL_VOLTAGE_N     "Voltage_Cell_%lu"       // Double (0 <= n < SEGMENT_COUNT * CELLS_PER_SEGMENT)
#define CELL_BALANCING_N   "Balancing_Cell_%lu"     // Double (0 <= n < SEGMENT_COUNT * CELLS_PER_SEGMENT)
#define CELL_TEMPERATURE_N "Temperature_Sensor_%lu" // Double (0 <= n < SEGMENT_COUNT * CELLS_PER_SEGMENT / CELLS_PER_TEMPERATURE_SENSOR)

// Classes --------------------------------------------------------------------------------------------------------------------

// View UI forward declaration
// - This namespace and class are generated at compile-time using the form file
namespace Ui
{
    class ViewBms;
}

class ViewBms : public View
{
    Q_OBJECT // QT object boilerplate
    
    // Constants --------------------------------------------------------------------------------------------------------------

    #define SEGMENT_COUNT                5           // Number of cell stacks in battery, row count
    #define CELLS_PER_SEGMENT            18          // Number of cells per stack, column count
    #define CELLS_PER_TEMPERATURE_SENSOR 2           // Number of cells monitored by each temperature sensor

    #define PADDING_CENTER_FRAME 2

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    ViewBms(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_);

    ~ViewBms();

    // Public Funtions --------------------------------------------------------------------------------------------------------

    void update() override;

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void handleButtonMenu();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    Ui::ViewBms* ui;                     // View UI, object generated by QMake

    StatCell*** cellStats;               // Dynamically allocated matrix of cell stat object pointers. Size of
                                         // [SEGMENT_COUNT x CELLS_PER_SEGMENT]

    double*** cellVoltages;              // Dynamically allocated matrix of cell voltage pointers, same size as cell stats.
    bool***   cellBalancings;            // Dynamically allocated matrix of cell balancing pointers, same size as cell stats.
    double*** cellTemperatures;          // Dynamically allocated matrix of cell temperature pointers. Size of
                                         // [SEGMENT_COUNT x (CELLS_PER_SEGMENT / CELLS_PER_TEMPERATURE_SENSOR)]
};

#endif // VIEW_BMS_H