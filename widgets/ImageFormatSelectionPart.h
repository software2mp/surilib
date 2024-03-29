/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef IMAGEFORMATSELECTIONPART_H_
#define IMAGEFORMATSELECTIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"

// Includes Wx
#include "wx/filepicker.h"
// Includes App

/** namespace suri */
namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ImageFormatSelectionPartEvent);

/** Parte seleccionar nombre y formato de archivo salida de una imagen a exportar */
/**
 * Esta clase es la Pagina que permite seleccionar un nombre de archivo y
 * un formato para exportar una imagen.
 */
class ImageFormatSelectionPart : public Part, public ProcessAtributeProvider {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ImageFormatSelectionPartEvent);
   /** Ctor. */
   ImageFormatSelectionPart();
   /** Dtror. */
   virtual ~ImageFormatSelectionPart();

   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Carga los formatos disponibles para exportar */
   void ConfigureFormatList();
   /** Devuelve el estado del Part. */
   bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Carga los valores iniciales */
   void SetInitialValues();
   /** Devuelve el nombre del arvhivo seleccionado */
   std::string GetFileName();
   /** Devuelve el formato seleccionado */
   std::string GetFormat();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Evento que se ejecuta cuando cambia el filename */
   void OnFileSelection(wxFileDirPickerEvent &Event);
   /** Evento que se ejecuta cuando se selecciona formato de salida */
   void OnFormatSelection(wxCommandEvent &Event);
   /** Evento UIUpdate */
   void OnUiUpdate(wxUpdateUIEvent &Event);
   /** Obtiene la ruta seleccionada en la herramienta grafica */
   std::string GetInternalFilename();
   /** Obtiene el formato seleccionado en la herramienta grafica */
   std::string GetInternalFormat();
   /** Verifica que tenga datos validos */
   bool HasValidData();
   /** Verifica que el archivo no tenga caracteres invalidos */
   bool IsValidFileName(const wxString &Filename);
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   /** Abre un file dialog para configurar nombre y salida del archivo */
   void OnButtonClicked(wxCommandEvent &Event);
   EVENT_OBJECT_PROPERTY(ImageFormatSelectionPartEvent); /*! Objeto para conectar */
   /* los eventos */
   std::string fileName_; /*! Ruta y nombre del archivo */
   std::string format_; /*! Formato del archivo a exportar */
   std::vector<std::string> suportedFormats_; /*! Formatos soportados para */
   /* exportar */
};
}  // namespace suri
#endif /* IMAGEFORMATSELECTIONPART_H_ */
