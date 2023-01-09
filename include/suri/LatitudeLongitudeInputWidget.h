/*! \file LatitudeLongitudeInputWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LATITUDELONGITUDEINPUTWIDGET_H_
#define LATITUDELONGITUDEINPUTWIDGET_H_

// Includes suri
#include "suri/NavigationInputWidget.h"

// Includes wx
#include "wx/event.h"

/** namespace suri */
namespace suri {
class World;
/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(LatitudeLongitudeInputWidgetEvent);

/** Clase para el ingreso de coordenadas en Lat/Long */
/**
 * Permite ingresar coordenadas en Lat/Long. Antes de mostrar las coordenadas
 * verifica que todos los campos hallan sido cargados con datos validos.
 * Recibe en el constructor un flag validate, por defecto en true, pero si esta
 * en false omite validar que la longitud sea menor a 180 y que la latidut sea
 * menor a 90.
 * Cuando se agregan botones a la herramienta se hace visible y luego
 * de mostrar el ultimo se oculta.
 * \todo cambiar separador de decimales segun region
 */
class LatitudeLongitudeInputWidget : public NavigationInputWidget {
   /** Ctor. Copia */
   LatitudeLongitudeInputWidget(const LatitudeLongitudeInputWidget &);

public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(LatitudeLongitudeInputWidgetEvent);

   /** ctor */
   LatitudeLongitudeInputWidget(bool Validate = true);
   /** dtor */
   virtual ~LatitudeLongitudeInputWidget();

   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const;
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &LatLongCoord);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates LatLongCoord);

protected:
   /** Metodo auxiliar que se encarga de validar la nueva coordenada y actualizar
    * el valor de la coordenada guardada */
   virtual bool ValidateAndUpdateCoordinates(Coordinates& LatLongCoordinate,
                                     Coordinates& NewCoordinate,
                                     Coordinates& ReferenceCoordinate);
   /** transforma lat long en strings a double */
   bool GetCoordinateFromStrings(const std::string &Coordinate, const wxString &Degree,
                                 const wxString &Minute, const wxString &Second,
                                 double &Value);
   /** guarda el valor en Coordinate en los strings */
   bool GetStringFromCoordinate(std::string &Degree, std::string &Minute,
                                std::string &Second, double Coordinate);
   /** Obtiene el icono del navegador */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** cambia el valor de norte/sur cuando hace click sobre texto */
   virtual void OnLatitudeHyperlink(wxCommandEvent &Event);
   /** cambia el valor de este/oeste cuando hace click sobre texto */
   virtual void OnLongitudeHyperlink(wxCommandEvent &Event);
   /** cambia s/n e/o cuando se ingreso coordenada valida. */
   virtual void OnLostFocus(wxFocusEvent& Event);
   /** Cambia el signo y el simbolo(n/s o e/o) segun corresponda */
   virtual void UpdateControlSign(double Coord, bool &Simbol,
                                  const char *pControlIds[]);
   /** Despliega la coordenada interna en los controles */
   virtual void DisplayCoordinates();
   /** Analiza si la coordenada interna es valida */
   virtual bool ValidateCoordinates(const Coordinates &LatLongCoord);
   /** Verifica si las coordenadas son iguales en formato texto */
   virtual bool EqualInControlPrecision(double Coord1, double Coord2);

   Coordinates latLongCoordinate_; /*! Coordenada que guarda ultimo valor */
   /* valido de la herramienta */
   bool north_; /*! flag que indica coordenadas norte */
   bool east_; /*! flat que indica coordenadas sur */
   bool validate_; /*! Flag para validar coordenadas menor a 180 y a 90 */
   EVENT_OBJECT_PROPERTY(LatitudeLongitudeInputWidgetEvent); /*! Objeto que */
   /* conecta los eventos */
};
}

#endif /* LATITUDELONGITUDEINPUTWIDGET_H_ */

