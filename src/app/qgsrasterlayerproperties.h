
/**
 * \brief The qgsrasterlayerproperties class is used to set up how raster layers are displayed.
 */
/* **************************************************************************
                          qgsrasterlayerproperties.h  -  description
                             -------------------
    begin                : Sun Aug 11 2002
    copyright            : (C) 2002 by Tim Sutton
    email                : tim@linfiniti.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSRASTERLAYERPROPERTIES_H
#define QGSRASTERLAYERPROPERTIES_H

#include "qgsoptionsdialogbase.h"
#include "ui_qgsrasterlayerpropertiesbase.h"
#include "qgsguiutils.h"
#include "qgshelp.h"
#include "qgsmaplayerstylemanager.h"
#include "qgsmaptoolemitpoint.h"
#include "qgis_app.h"

class QgsPointXY;
class QgsMapLayer;
class QgsMapCanvas;
class QgsRasterLayer;
class QgsMetadataWidget;
class QgsRasterRenderer;
class QgsRasterRendererWidget;
class QgsRasterHistogramWidget;

/**
 * Property sheet for a raster map layer
  */
class APP_EXPORT QgsRasterLayerProperties : public QgsOptionsDialogBase, private Ui::QgsRasterLayerPropertiesBase
{
    Q_OBJECT

  public:

    enum StyleType
    {
      QML,
      SLD
    };
    Q_ENUM( StyleType )

    /**
     * \brief Constructor
     * \param ml Map layer for which properties will be displayed
     */
    QgsRasterLayerProperties( QgsMapLayer *lyr, QgsMapCanvas *canvas, QWidget *parent = nullptr, Qt::WindowFlags = QgsGuiUtils::ModalDialogFlags );

    //! Synchronize state with associated raster layer
    void sync();

  public slots:
    //TODO: Verify that these all need to be public
    //! \brief Applies the settings made in the dialog without closing the box
    void apply();
    //! Called when cancel button is pressed
    void onCancel();
    //! \brief Slot to update layer display name as original is edited.
    void mLayerOrigNameLineEd_textEdited( const QString &text );
    //! \brief this slot asks the rasterlayer to construct pyramids
    void buttonBuildPyramids_clicked();
    //! \brief slot executed when user presses "Add Values From Display" button on the transparency page
    void pbnAddValuesFromDisplay_clicked();
    //! \brief slot executed when user presses "Add Values Manually" button on the transparency page
    void pbnAddValuesManually_clicked();
    //! \brief slot executed when user changes the layer's CRS
    void mCrsSelector_crsChanged( const QgsCoordinateReferenceSystem &crs );
    //! \brief slot executed when user wishes to reset noNoDataValue and transparencyTable to default value
    void pbnDefaultValues_clicked();
    //! \brief slot executed when user wishes to export transparency values
    void pbnExportTransparentPixelValues_clicked();
    //! \brief auto slot executed when the active page in the main widget stack is changed
    void optionsStackedWidget_CurrentChanged( int index ) override;
    //! \brief slow executed when user wishes to import transparency values
    void pbnImportTransparentPixelValues_clicked();
    //! \brief slot executed when user presses "Remove Selected Row" button on the transparency page
    void pbnRemoveSelectedRow_clicked();

    /**
     * \brief slot executed when the single band radio button is pressed.
     * \brief slot executed when the reset null value to file default icon is selected
     */
    //void on_btnResetNull_clicked();

    void pixelSelected( const QgsPointXY &, const Qt::MouseButton & );

  private slots:
    void mRenderTypeComboBox_currentIndexChanged( int index );
    //! Load the default style when appropriate button is pressed.
    void loadDefaultStyle_clicked();
    //! Save the default style when appropriate button is pressed.
    void saveDefaultStyle_clicked();
    //! Load a saved style when appropriate button is pressed.
    void loadStyle_clicked();
    //! Save a style when appriate button is pressed.
    void saveStyleAs_clicked();
    //! Restore dialog modality and focus, usually after a pixel clicked to pick transparency color
    void restoreWindowModality();


    //! Load a saved metadata file.
    void loadMetadata();
    //! Save a metadata.
    void saveMetadataAs();
    //! Save the default metadata.
    void saveDefaultMetadata();
    //! Load the default metadata.
    void loadDefaultMetadata();

    //! Help button
    void showHelp();

    //! Slot to reset all color rendering options to default
    void mResetColorRenderingBtn_clicked();

    //! Enable or disable Build pyramids button depending on selection in pyramids list
    void toggleBuildPyramidsButton();

    //! Enable or disable saturation controls depending on choice of grayscale mode
    void toggleSaturationControls( int grayscaleMode );

    //! Enable or disable colorize controls depending on checkbox
    void toggleColorizeControls( bool colorizeEnabled );

    //! Transparency cell changed
    void transparencyCellTextEdited( const QString &text );

    void aboutToShowStyleMenu();

    //! Make GUI reflect the layer's state
    void syncToLayer();

    void urlClicked( const QUrl &url );

  private:
    QPushButton *mBtnStyle = nullptr;
    QPushButton *mBtnMetadata = nullptr;
    QAction *mActionLoadMetadata = nullptr;
    QAction *mActionSaveMetadataAs = nullptr;

    //! \brief  A constant that signals property not used
    const QString TRSTRING_NOT_SET;

    //! \brief Default contrast enhancement algorithm
    QString mDefaultContrastEnhancementAlgorithm;

    //! \brief default standard deviation
    double mDefaultStandardDeviation;

    //! \brief Default band combination
    int mDefaultRedBand;
    int mDefaultGreenBand;
    int mDefaultBlueBand;

    //! \brief Flag to indicate if Gray minimum maximum values are actual minimum maximum values
    bool mGrayMinimumMaximumEstimated;

    //! \brief Flag to indicate if RGB minimum maximum values are actual minimum maximum values
    bool mRGBMinimumMaximumEstimated;

    //! \brief Pointer to the raster layer that this property dilog changes the behavior of.
    QgsRasterLayer *mRasterLayer = nullptr;

    /**
     * \brief If the underlying raster layer doesn't have a provider

        This variable is used to determine if various parts of the Properties UI are
        included or not
     */
    //bool mRasterLayerIsInternal;

    QgsRasterRendererWidget *mRendererWidget = nullptr;
    QgsMetadataWidget *mMetadataWidget = nullptr;

    bool rasterIsMultiBandColor();

    void setupTransparencyTable( int nBands );

    //! \brief Clear the current transparency table and populate the table with the correct types for current drawing mode and data type
    void populateTransparencyTable( QgsRasterRenderer *renderer );

    void setTransparencyCell( int row, int column, double value );
    void setTransparencyCellValue( int row, int column, double value );
    double transparencyCellValue( int row, int column );
    void setTransparencyToEdited( int row );
    void adjustTransparencyCellWidth( int row, int column );

    void setRendererWidget( const QString &rendererName );

    //TODO: we should move these gradient generators somewhere more generic
    //so they can be used generically throughout the app
    QLinearGradient greenGradient();
    QLinearGradient redGradient();
    QLinearGradient blueGradient();
    QLinearGradient grayGradient();
    QLinearGradient highlightGradient();
    qreal mGradientHeight;
    qreal mGradientWidth;

    QgsMapCanvas *mMapCanvas = nullptr;
    std::unique_ptr<QgsMapToolEmitPoint> mPixelSelectorTool;

    QgsRasterHistogramWidget *mHistogramWidget = nullptr;

    QVector<bool> mTransparencyToEdited;

    /**
     * Previous layer style. Used to reset style to previous state if new style
     * was loaded but dialog is canceled */
    QgsMapLayerStyle mOldStyle;

    bool mDisableRenderTypeComboBoxCurrentIndexChanged = false;

    bool mMetadataFilled;

    friend class QgsAppScreenShots;
};
#endif
