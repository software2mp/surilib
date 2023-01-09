/*! \file CsvFormatSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CSVFORMATSELECTIONPART_H_
#define CSVFORMATSELECTIONPART_H_

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
DECLARE_EVENT_OBJECT(CsvFormatSelectionEventHandler);

/*!
 * Clase que permite definir el archivo y formato de
 * salida para un archivo csv
 */
class CsvFormatSelectionPart : public suri::Part, public ProcessAtributeProvider {
   /** ctor copia **/
   CsvFormatSelectionPart(const CsvFormatSelectionPart&);
public:
   /** Constructor */
   CsvFormatSelectionPart();
   /** Destructor **/
   virtual ~CsvFormatSelectionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Obtiene el formato elegido para el archivo de salida **/
   std::string GetFormat() const;
   /** Obtiene el nombre del archivo de salida **/
   std::string GetFileName() const;

private:
   /** Evento UIUpdate */
   void OnUiUpdate(wxUpdateUIEvent &Event);
   void OnButtonClicked(wxCommandEvent &Event);
   /** nombre del archivo de salida **/
   std::string fileName_;
   /** formato del archivo de salida **/
   std::string format_;
   FRIEND_EVENT_OBJECT(CsvFormatSelectionEventHandler);
   EVENT_OBJECT_PROPERTY(CsvFormatSelectionEventHandler);
};

} /** namespace suri */

#endif /* CSVFORMATSELECTIONPART_H_ */
