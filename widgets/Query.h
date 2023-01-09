/*! \file Query.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERY_H_
#define QUERY_H_


// Includes Estandar
#include <string>

// Includes Suri
#include "suri/ViewportEvent.h"

// Includes Wx

// Defines

// forwards
class OGRFeature;

namespace suri {

// forwards
class VectorElement;

/*!
 * Clase encargada de realizar la consulta
 * de pixel sobre una geometria en base un punto clickeado
 */
class Query : public ViewportEvent {
public:
   /** Ctor */
   Query(ViewportManagerInterface *pViewportManager,
         ViewcontextManagerInterface *pViewcontextManager,
         LibraryManager *pLibraryManager = NULL);
   virtual ~Query();

   virtual void Reset();
   virtual void GetCursor(wxCursor &Cursor);
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
    virtual void OnDrag(const Coordinates & InitialCoordinate,
                          const Coordinates & CurrentCoordinate, wxMouseEvent &Event);
    virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
    virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

protected:
    /** Metodo privado que genera un polinomio en base al punto
     * que se pasa por parametro transformado en el Sr de llegada (SrWktOut). **/
    OGRGeometry *GetQueryPoly(const Coordinates& Point, const std::string& SrWkt,
                              double Tolerance, const std::string& SrWktOut);
    /** Genera el vector de seleccion. **/
    VectorElement *CreateSelectionVector(OGRFeature *pModelFeature, const std::string &SrWkr,
                                         Vector::VectorType Type) const;
    /** Agrega la geometria que forma parte de resultado de la consulta. **/
    void AddFeatureToVector(VectorElement *pVector, OGRFeature *pFeature)const;
    /** Administrador de bibliotecas */
    LibraryManager *pLibraryManager_;
};

} /** namespace suri */

#endif /* QUERY_H_ */
