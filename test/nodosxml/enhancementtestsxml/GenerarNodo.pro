pro GenerarNodo,dimension,realce

; parametros
dimension = 32
realce = "SquareRoot"
tipo= "creciente"

directorio = "z:\Temporal\imagenes-realce\"
sufijo = strtrim(dimension,1)+"x"+strtrim(dimension,1)

;Leo imagen entrada
bandaorigen=read_tiff(directorio+"banda"+tipo+"-"+sufijo+".tiff")

;Leo resultado enhancement
bandaenhancement=read_tiff(directorio+"banda"+tipo+"-"+realce+"-"+sufijo+".tif")

;Cargo comienzo de xml
openw, lun, directorio+'tabla'+tipo+'-'+realce+'-'+sufijo+'.xml', /get_lun
printf, lun, '<?xml version="1.0" encoding="UTF-8"?>'
printf, lun, '<tabla filas="2" columnas="'+strtrim(dimension*dimension,1)+'">'

;Cargo Valores imagen de entrada
str=''
n =n_elements(bandaorigen) 
for i=0,n-1 do begin
  str=str + ' ' + strtrim(string(bandaorigen[i],Format='(D0.8)'),2)
endfor
printf, lun, '<fila>' + str + '</fila>'

;Cargo Valores imagen de luego de aplicar enhancement
str=''
n =n_elements(bandaenhancement) 
for i=0,n-1 do begin
  str=str + ' ' + strtrim(string(bandaenhancement[i],FORMAT = '(I5)'),2)
endfor
printf, lun, '<fila>' + str + '</fila>'

;Cargo fin de xml
printf, lun, '</tabla>'

;Guardo en disco
free_lun, lun 



end