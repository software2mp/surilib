/*! \file PixelInformationWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PIXELINFORMATIONWIDGET_H_
#define PIXELINFORMATIONWIDGET_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/NotebookWidget.h"
#include "suri/DatasourceContainerInterface.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
// fordwars
class LibraryManager;
class World;

/** suri::PixelInformationPart: genera el dialogo de informacion de pixel */
/*
 * PixelInformacionPart contiene informacion general, y un notebook con un
 * tab por capa, con la informacion del valor de banda para cada elemento
 * raster y las features que caen en la zona, para cada capa vectorial
 */
class PixelInformationWidget : public NotebookWidget {
public:
   /** Ctor */
   explicit PixelInformationWidget(DatasourceContainerInterface *pDatasourceList,
                             const Coordinates &Point, const std::string &PointWkt,
                             const Coordinates &MClick, const Coordinates &ProcessedPl,
                             World* pViewerWorld = NULL, LibraryManager* pLibraryManager = NULL);
   /** Dtor */
   virtual ~PixelInformationWidget();

   virtual bool CreateToolWindow();
   virtual wxWindow *GetToolWindow();
   /** Retorna la el icono de la ventana */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   DatasourceContainerInterface *pDatasourceList_; /**! Lista de FDs consultadas */
   Coordinates point_;  /**! Punto de consulta */
   std::string pointWkt_;  /**! SR del punto de consulta */
   Coordinates mclick_; /** coordenadas reales del mouse */
   Coordinates processedPl_;  /** coordenadas procesadas */
   World* pViewerWorld_;  /** mundo utilizado para el elemento */
   LibraryManager* pLibraryManager_; /** Adminsitrador de bibliotecas  */

   /**
    * Crea el part que muestra info general sobre el pixel.
    */
   Part* CreateGeneralPixelInformationPart();

   /**
    * Metodo que obtiene y configura el part para pagina de consulta de pixel para una
    * fuente de datos vectorial*/
   Part* GetVectorTablePart(DatasourceInterface* pVectorDatasource);

   /**
    * Carga la informacion de coordenadas.
    */
   void LoadCoordinatesInfo(const Coordinates& Coords, const std::string& CoordsWkt);
};

}  // namespace suri

#endif /* PIXELINFORMATIONPART_H_ */
