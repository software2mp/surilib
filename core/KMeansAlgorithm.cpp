/*! \file KMeansAlgorithm.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "KMeansAlgorithm.h"

// Includes standard
#include <limits>
// Includes Suri
#include "suri/DataTypes.h"
#include "suri/xmlnames.h"
#include "suri/AuxiliaryFunctions.h"
#include "logmacros.h"
#include "suri/XmlFunctions.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Registra la clase entre los tipos de algoritmos de clasificacion */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, KMeansAlgorithm, 0)

/** Template KMeans */
/**
 * Recorre la imagen asignando cada pixel a la clase con media mas cercana y
 * guarda a la salida el id correspondiente.
 * @param[out] pDest imagen con ints donde se guardan los ids
 * @param[in] pSource imagen que se quiere clasificar
 * @param[in] Size cantidad de pixeles en la imagen a clasificar
 * @param[in] Classes vector con la informacion de las clases a clasificar
 * contiene el id y la media.
 */
template<typename T>
bool kmeans(int* pDest, std::vector<void*> &pSource, size_t Size,
            int NDVPixelValue, double Ndv, bool NdvAvailable,
            const std::vector<KMeansAlgorithm::KMeansAlgorithmClassData> &Classes) {
   int sourcesize = pSource.size();

   // Si la cantidad de capas es diferente al tamanio de la media falla operacion
   if ((int) Classes[0].mean_.size() != sourcesize) {
      REPORT_DEBUG(
            "D: Error, la cantidad de bandas no es igual al tamanio de la media");
      return false;
   }

   std::vector<T*> psrc;
   for (int i = 0; i < sourcesize; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   int classcount = Classes.size();
   double diference;
   double pixeldistance = 0;
   // Para cada pixel en la imagen
   for (int pixelposition = 0; pixelposition < (int) Size; pixelposition++) {
      // La clase default es sin clase
      int pixelclass = 0;

      bool invalidpixel = true;

      // Salida para pixel no valido.
      // (valor no valido en todas las bandas involucradas)
      // Ver TCK #7325
      if (NdvAvailable) {
         for (int i = 0; i < sourcesize && invalidpixel; ++i) {
            if (static_cast<double>(psrc[i][pixelposition]) != CLASSIFICATION_NDV)
               invalidpixel = false;
         }
      } else {
         invalidpixel = false;
      }

      // Si el pixel es valido hago el proc normal.
      if (invalidpixel == false) {
         double pixelmindistance = std::numeric_limits<double>::max();

         // Para cada clase
         for (int classpos = 0; classpos < classcount; classpos++) {
            pixeldistance = 0;
            // Le resto a cada pixel la media
            for (int i = 0; i < sourcesize; i++) {
               diference = (static_cast<double>(psrc[i][pixelposition])
                     - Classes[classpos].mean_[i]);
               pixeldistance += diference * diference;
            }

            // Comparo la ultima clase asignada y el threashold
            if (pixeldistance < pixelmindistance) {
               pixelclass = Classes[classpos].id_;
               pixelmindistance = pixeldistance;
            }
         }
      } else {
         // Sino le pongo un valor para la parte de la imagen que no
         // es valida.
         pixelclass = NDVPixelValue;
      }
      pDest[pixelposition] = pixelclass;
   }
   return true;
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(KMeansAlgorithm::KMeansFunctionType, kmeans);

/** Ctor */
KMeansAlgorithm::KMeansAlgorithm() {
}

/** Dtor */
KMeansAlgorithm::~KMeansAlgorithm() {
}

/**
 * Actualiza los datos internos del algoritmo con los del nodo.
 * @param[in] pAlgorithmNode nodo del algoritmo
 * @return informa si el nodo era del tipo correto.
 */
bool KMeansAlgorithm::Update(const wxXmlNode* pAlgorithmNode) {
   if (!pAlgorithmNode->GetName().IsSameAs(wxT(CLASSIFICATION_ALGORITHM_NODE)))
      return false;

   const wxXmlNode *pdatanode = GetNodeByName(wxT(ALGORITHM_NAME_NODE), pAlgorithmNode);
   if (!pdatanode)
      return false;

   wxXmlNode *pchildnode = pAlgorithmNode->GetChildren();
   while (pchildnode) {
      if (pchildnode->GetName().IsSameAs(wxT(CLASS_NODE))) {
         // Cargo id clase
         wxString idstring;
         if (pchildnode->GetPropVal(wxT(CLASS_ID_PROPERTY), &idstring)) {
            int id = StringToNumber<int>(idstring.c_str());
            KMeansAlgorithmClassData classdata(id);

            // Cargo propiedades clase
            wxXmlNode *pclassnode = pchildnode->GetChildren();
            while (pclassnode) {
               std::string nodecontent = pclassnode->GetNodeContent().c_str();
               // Obtiene el color de la clase
               if (pclassnode->GetName() == wxT(MEAN_NODE)) {
                  classdata.mean_ = split<double>(nodecontent);
               }

               pclassnode = pclassnode->GetNext();
            }

            // Agrego clase a lista de clases.
            classes_.push_back(classdata);
         }
      }

      pchildnode = pchildnode->GetNext();
   }
   return true;
}

/**
 * Genera nodo con los datos itnernos del algoritmo.
 * \attention el nodo generado es resposabilidad del metodo que invoca GetNode
 * @return nodo xml con el algoritmo
 */
wxXmlNode* KMeansAlgorithm::GetNode() {
   wxXmlNode *palgorithmnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                             wxT(CLASSIFICATION_ALGORITHM_NODE));

   // Creo nodo con tipo de algoritmo
   new wxXmlNode(new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
           wxT(ALGORITHM_NAME_NODE)), wxXML_TEXT_NODE, wxEmptyString,
           KMEANS_NODE);

   int classcount = classes_.size();
   for (int i = 0; i < classcount; i++) {
      // Cargo propiedad con id de clase
      wxString id = wxString::Format(wxT("%d"), classes_[i].id_);
      wxXmlNode *pclassnode = new wxXmlNode(palgorithmnode, wxXML_ELEMENT_NODE,
                                            wxT(CLASS_NODE));
      pclassnode->SetProperties(new wxXmlProperty(wxT(CLASS_ID_PROPERTY), id));

      // nodo de medias
      wxString content =
            join(classes_[i].mean_.begin(), classes_[i].mean_.end()).c_str();
      new wxXmlNode(new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE, wxT(MEAN_NODE)),
                    wxXML_TEXT_NODE, wxEmptyString, content);
   }

   return palgorithmnode;
}

/**
 * Utiliza la funcion de kmeans para asignar una clase a cada pixel.
 * @param[in] pDest destino de la clasificacion
 * @param[in] pSource fuente de datos para clasificar
 * @param[in] Size cantidad de pixeles
 * @return true si puede aplicar la funcion de clasificacion, false en otro caso
 */
bool KMeansAlgorithm::Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                               const std::string &DataType) {
   function_ = kmeansTypeMap[DataType];
   return function_(pDest, pSource, Size, GetNDVPixelValue(), GetNoDataValue(),
                    IsNoDataValueAvailable(), classes_);
}

/**
 * Verifico que todas las medias tengan la misma dimension.
 * @return informa si el algoritmo esta bien configurado
 */
bool KMeansAlgorithm::Check() const {
   bool consistentdimensions = true;
   std::vector<int>::iterator result;
   size_t dimension = 0;
   if (classes_.size() > 0) {
      dimension = classes_[0].mean_.size();
   }
   consistentdimensions = (dimension > 0);
   for (size_t i = 1; i < classes_.size() && consistentdimensions; i++)
      consistentdimensions &= (dimension == classes_[i].mean_.size());

   return consistentdimensions;
}
}

