pro calc_histograma,banda,nombre,bincount,maximo,minimo,lun
  binsize = (maximo-minimo) / bincount
  histograma=histogram(banda,BINSIZE=binsize)
  n = n_elements(histograma)-1
  ;print,-1,string(n)+" "+string(bincount)
  ;iplot,histograma,TITLE=nombre
  histostro="    <fila>"
  for i=0,n do begin
    histostro = histostro + " " + strtrim(histograma[i],1)
  endfor
  histostro = histostro + "</fila>"
  
;  openu, lun, directorio+'datos-'+nombre+'-'+sufijo+'.xml', /get_lun
  printf, lun, '  <histograma bin="'+string(bincount,FORMAT="(I0)")+'" min="'+string(minimo,FORMAT="(D0)")+'" max="'+string(maximo,FORMAT="(D0)")+' filas="1" columnas="'+string(bincount,FORMAT="(I0)")+'">'
  printf, lun, histostro
  printf, lun, '  </histograma>'
;  free_lun, lun
end

pro calc_statistics,banda,nombre,lun
  image_statistics,banda,COUNT=bcount,DATA_SUM=bsum,MAXIMUM=bmax,MINIMUM=bmin,VARIANCE=bvar
;  openu, lun, directorio+'datos-'+nombre+'-'+sufijo+'.xml', /get_lun
  printf, lun, '  <estadisticas>'
  printf, lun, '   <muestras>'+string(bcount,FORMAT='(D0)')+'</muestras>'
  printf, lun, '   <media>'+string(bsum/bcount,FORMAT='(D0)')+'</media>'
  printf, lun, '   <varianza>'+string(bvar,FORMAT='(D0)')+'</varianza>'
  printf, lun, '   <maximo>'+string(bmax,FORMAT='(D0)')+'</maximo>'
  printf, lun, '   <minimo>'+string(bmin,FORMAT='(D0)')+'</minimo>'
  printf, lun, '  </estadisticas>'
;  free_lun, lun
end

pro make_xml,banda,nombre,maximo,minimo,lun
  printf, lun, ' <elemento tipo="raster">'
  printf, lun, '  <url>'+nombre+'</url>'
  calc_histograma,banda,nombre,256.0,maximo,minimo,lun
  calc_statistics,banda,nombre,lun
  printf,lun,' </elemento>'
end

pro programa,dimension,maximo,minimo

; parametros
dimension = 10
maximo = 1.0
minimo = -1.0

directorio = "z:\Temporal\imagenes-realce\"
sufijo = strtrim(dimension,1)+"x"+strtrim(dimension,1)

bandaaleatoria=make_array(dimension, dimension, /DOUBLE)
bandaaleatoria(0:dimension-1,0:dimension-1)=randomu(seed,dimension,dimension)*(maximo-minimo)+minimo
; garantizo que tengan el maximo y minimo
bandaaleatoria(0,0)=minimo
bandaaleatoria(dimension-1,dimension-1)=maximo
bandaaleatoria(where("bandaaleatoria GT maximo"))=maximo
bandaaleatoria(where("bandaaleatoria LT minimo"))=minimo

write_tiff,directorio+"bandaaleatorea-"+sufijo+".tiff",bandaaleatoria,/FLOAT 

bandacreciente=make_array(dimension, dimension, /DOUBLE)
value = minimo
n = n_elements(bandacreciente) 
for i=0,n-1 do begin
    bandacreciente(i)=value
    value = value + (maximo-minimo)/(dimension*dimension-1);
endfor
bandacreciente(n-1)=maximo
write_tiff,directorio+"bandacreciente-"+sufijo+".tiff",bandacreciente,/FLOAT

openw, lun, directorio+'datos-'+sufijo+'.xml', /get_lun
printf, lun, '<?xml version="1.0" encoding="UTF-8"?>'
printf, lun, '<datos>'

make_xml,bandacreciente,"bandacreciente",maximo,minimo,lun
make_xml,bandaaleatoria,"bandaaleatoria",maximo,minimo,lun

printf, lun, '</datos>'
free_lun, lun


end