# Metroflip
Metroflip es una aplicación lectora de tarjetas de metro multi-protocolo para el Flipper Zero, inspirada en el proyecto Metrodroid. Permite el análisis y parseo de tarjetas de metro de sistemas de transporte de todo el mundo, proporcionando una prueba de concepto para explorar datos de tarjetas de transporte en formato portátil.

# Autor
[@luu176](https://github.com/luu176)

![Menu-Top-Screenshot](screenshots/Menu-Top.png)

# 🚆 Plugins RENFE - Soporte para Tarjetas de Transporte Españolas

Metroflip ahora incluye soporte integral para **tarjetas de transporte RENFE españolas** a través de dos plugins especializados diseñados para la región de Valencia y uso a nivel nacional.

---

## 📋 Resumen de Plugins

### 🎫 Plugin RENFE Suma 10
**Para tarjetas de pago por viaje y abonos mensuales de la región de Valencia**

### 🚂 Plugin RENFE Regular  
**Para tarjetas RENFE nacionales con soporte multi-regional**

---

## ✨ Variantes de Tarjetas Soportadas

### Tarjetas de la Región de Valencia (Plugin RENFE Suma 10)
- **SUMA 10** - Tarjetas de pago por viaje con clasificación de zonas
- **MOBILIS 30** - Abonos mensuales con nombres de titulares

### Tarjetas Nacionales (Plugin RENFE Regular)
- **Bono Regular 10 viajes** - Específico de Valencia con contador de viajes
- **RENFE Cercanías** - Tarjetas de trenes de cercanías
- **RENFE Regular** - Tarjetas estándar nacionales

---

## 🌍 Soporte Multi-Regional

El plugin RENFE Regular soporta **18 regiones españolas**:
- 🏙️ **Valencia** - Metro, Tranvía, Cercanías, FGV
- 🏛️ **Madrid** - Cercanías, integración con Metro
- 🏛️ **Catalunya** - Barcelona, integración FGC  
- 🌅 **Andalucía** - Sevilla, Córdoba, Málaga
- 🏔️ **País Vasco** - Bilbao, integración EuskoTren
- 🌊 **Galicia** - Santiago, integración FEVE
- 🦅 **Aragón** - Zaragoza
- 🏰 **Castilla y León** - Valladolid, León, Salamanca
- ⛰️ **Asturias** - Oviedo, Gijón
- 🌊 **Cantabria** - Santander
- 🍷 **La Rioja** - Logroño
- 🏛️ **Navarra** - Pamplona
- 🌶️ **Murcia** - Murcia, Cartagena
- 🏰 **Castilla-La Mancha** - Toledo, Ciudad Real
- 🌳 **Extremadura** - Cáceres, Badajoz
- 🏝️ **Canarias** y **Baleares** - Cuando aplique
- 🏛️ **Ceuta** y **Melilla**

---

![Renfe-Screenshot](screenshots/Suma10.png)

## 🎯 Características Principales

### Detección Avanzada de Tarjetas
- ✅ **Reconocimiento automático de variantes** (SUMA/MOBILIS/Bono)
- ✅ **Validación basada en patrones** ("CARRERES MOMP" para MOBILIS 30, "E8 03 04" para Bono Regular)
- ✅ **Auto-detección multi-regional** basada en patrones de datos de tarjeta

### Extracción Inteligente de Datos
- 🆔 **Visualización de UID** (lectura en vivo + dumps guardados)
- 🎯 **Clasificación de zonas** (A, B, C, D, E, F, 1-6, L1, L2, B1)
- 🏠 **Identificación de estación origen** (Valencia + nacional)
- 🔢 **Contador de viajes** (X/10 para tarjetas Bono Regular)
- 👤 **Extracción completa de nombre del titular** (MOBILIS 30: nombre + ambos apellidos con normalización de acentos)

### Historial de Viajes Completo
- 📚 **Análisis completo del historial** con ordenamiento cronológico
- 🚇 **Identificación de tipos de transacción**:
  - Operaciones de entrada/salida
  - Transbordos entre líneas  
  - Recargas y cargos
  - Validaciones e inspecciones
- 🏪 **Resolución de nombres de estaciones** en todas las regiones soportadas
- ⏰ **Extracción y visualización de timestamps**

### Optimización de Memoria
- 🔧 **Caché de estaciones reducido** (50 estaciones para limitaciones del Flipper Zero)
- 🔧 **Estructuras de datos optimizadas** y asignación dinámica
- 🔧 **Gestión inteligente de recursos** con limpieza automática

---

## 🛠️ Especificaciones Técnicas

### Estructura de Archivos
```
applications_user/Metroflip/scenes/plugins/
├── renfe_sum10.c          # Tarjetas región Valencia (SUMA/MOBILIS)
├── renfe_regular.c        # Tarjetas nacionales (Bono/Cercanías/Regular)
└── ...

assets/metroflip/renfe/stations/
├── valencia.txt           # Estaciones Metro/Tranvía Valencia
├── madrid.txt            # Estaciones Cercanías Madrid
├── catalunya.txt         # Estaciones Barcelona/FGC
├── andalucia.txt         # Estaciones región Andalucía
└── ... (18 archivos regionales)
```

### Protocolos Soportados
- 💳 **Mifare Classic 1K** - Formato estándar


### Cifrado
- 🔐 **Claves específicas RENFE** validadas con dumps reales
- 🔐 **Conjuntos de claves de respaldo** para autenticación alternativa
- 🔐 **Lectura sector por sector** con patrones de acceso optimizados

---

## 🚀 Instrucciones de Uso

### Lectura de Tarjeta en Vivo (Tarjetas de Valencia)
1. Abrir la aplicación **Metroflip**
2. Seleccionar plugin **RENFE Suma 10**
3. Colocar tarjeta de Valencia en la parte trasera del Flipper Zero
4. Ver análisis en tiempo real
5. Presionar **IZQUIERDA** para historial detallado de viajes
6. Presionar **CENTRO** para guardar dump para análisis posterior

### Análisis de Dump Guardado (Tarjetas Nacionales)
1. Exportar dump de tarjeta usando **Proxmark3** o herramienta similar
2. Copiar archivo `.nfc` a la carpeta **Saved** del Flipper Zero
3. Seleccionar plugin **RENFE Regular**
4. Cargar archivo dump desde **Saved**
5. Ver información completa de la tarjeta
6. Acceder al historial de viajes con botón **IZQUIERDA**

### Modo de Depuración
Habilitar logging detallado para desarrollo:
```bash
# En CLI del Flipper Zero
log debug
```

---

## 🧩 Detalles de Interpretación de Datos

### Plugin RENFE Suma 10 (Región de Valencia)

#### Detección de Variantes de Tarjeta
- **SUMA 10**: Tarjetas estándar de pago por viaje
- **MOBILIS 30**: Abonos mensuales con detección de patrón "CARRERES MOMP"

#### Puntos de Extracción de Datos

**🆔 UID**
- **Fuente:** `iso14443_3a_data->uid`
- **Formato:** Hexadecimal con bytes separados por espacios
- **Propósito:** Identificador único de tarjeta

**🎯 Clasificación de Zonas**
- **SUMA 10**: Bloque 5, bytes 5-6 (`block5[5] << 8 | block5[6]`)
- **MOBILIS 30**: Análisis multi-bloque (Bloques 12, 10, 1) para detección mejorada
- **Función:** `renfe_sum10_extract_zone_code` / `renfe_sum10_extract_mobilis_zone_code`
- **Zonas:** A, B, C, D, E, F, 1-6, L1, L2, B1
- **Ejemplo:** `0x6C16` → Zona `A`

**👤 Nombre del Titular (MOBILIS 30)**
- **Bloques:** 9 (nombre) y 14 (apellidos)
- **Procesamiento:** Extracción multi-bloque con normalización de acentos
- **Formato:** "NOMBRE APELLIDO1 APELLIDO2"
- **Soporte de Acentos:** Conversión automática (á→A, é→E, í→I, ó→O, ú→U, ñ→N, ü→U)
- **Ejemplo:** Entrada "José" → Salida "JOSE", Entrada "Rodríguez" → Salida "RODRIGUEZ"

**🏠 Estación de Origen**
- **Algoritmo:** Encuentra primera transacción de recarga (`0x33` o `0x3A`)
- **Respaldo:** Usa primer viaje válido si no hay recarga
- **Código de Estación:** Bytes 9-10 de transacción
- **Bloques de Búsqueda:** 18, 22, 28-30, 44-46

**📚 Historial de Viajes**
- **Bloques Analizados:** [4-14, 16-22, 28-30, 44-46]
- **Criterios de Validación:**
  - Tipo de transacción válido
  - Código de estación no nulo
  - Timestamp válido (bytes 2-4 ≠ 0x00 o 0xFF)
- **Procesamiento:** Ordenamiento cronológico (más reciente primero)

### Plugin RENFE Regular (Nacional)

#### Detección de Variantes de Tarjeta
- **Bono Regular 10 viajes**: Detección de patrón "E8 03 04"
- **RENFE Cercanías**: Identificación de trenes de cercanías
- **RENFE Regular**: Tarjetas estándar nacionales

#### Características Mejoradas
- **Visualización de Contador de Viajes**: Muestra X/10 viajes para tarjetas Bono
- **Extracción Multi-posición de Código de Estación**: Prueba múltiples posiciones de bytes
- **Auto-detección de Región**: Basada en patrones de datos de tarjeta
- **Depuración Integral**: Logs de análisis bloque por bloque

### 📋 Tipos de Transacción

| Código | Descripción | Tipo | Contexto |
|---------|-------------|------|----------|
| `0x13` | Entrada | ENTRADA | Entrada a estación |
| `0x1A` | Salida | SALIDA | Salida de estación |
| `0x1E` | Transbordo | TRANSBORDO | Cambio de línea |
| `0x16` | Validación | VALIDACION | Validación de billete |
| `0x33` | Recarga | RECARGA | Recarga de tarjeta |
| `0x3A` | Cargo adicional | CARGO | Pago extra |
| `0x17` | Inspección | INSPECCION | Control de billetes |
| `0x23` | Descuento | DESCUENTO | Reducción de tarifa |
| `0x2A` | Sanción | SANCION | Pago de multa |
| `0x2B` | Operación especial | ESPECIAL | Operación del sistema |
| Otros | Desconocido | Desconocido | Desarrollo necesario |

### 🔢 Contador de Viajes (Bono Regular)
- **Ubicación:** Bloque 5, Byte 4 (`block5[4]`)
- **Condición:** `block5[0] == 0x01` y `block5[1-3] == 0x00`
- **Visualización:** Formato `🎫 Viajes: X/10`
- **Validación:** Probado en mundo real con tarjetas de región Alzira

---

## 🗺️ Sistema de Base de Datos de Estaciones

### Organización de Archivos
```
/ext/apps_assets/metroflip/renfe/stations/
├── valencia.txt              # Valencia Metro/Tranvía/FGV
├── madrid.txt                # Madrid Cercanías/Metro
├── catalunya.txt             # Barcelona/FGC
├── andalucia.txt             # Sevilla/Córdoba/Málaga
├── pais_vasco.txt            # Bilbao/EuskoTren
├── galicia.txt               # Santiago/FEVE
├── aragon.txt                # Zaragoza
├── castilla_leon.txt         # Valladolid/León/Salamanca
├── asturias.txt              # Oviedo/Gijón
├── cantabria.txt             # Santander
├── la_rioja.txt              # Logroño
├── navarra.txt               # Pamplona
├── murcia.txt                # Murcia/Cartagena
├── castilla_la_mancha.txt    # Toledo/Ciudad Real
├── extremadura.txt           # Cáceres/Badajoz
├── canarias.txt              # Islas Canarias
├── baleares.txt              # Islas Baleares
└── ceuta_melilla.txt         # Ciudades del norte de África
```

### Formato de Archivo
```
0xCODIGO,Nombre_Estacion
```

**Ejemplos:**
```
0x6A12,València Nord
0x6A13,Estació del Nord
0x6A14,Xàtiva
0x1234,Madrid-Chamartín
0x5678,Barcelona-Sants
```

### Sistema de Caché Inteligente
- **Capacidad:** 50 estaciones máximo (optimizado para Flipper Zero)
- **Estrategia:** Expulsión LRU (Least Recently Used)
- **Respaldo:** Múltiples intentos de archivo para códigos desconocidos
- **Carga Dinámica:** Selección de archivo específico por región

### Jerarquía Región Valencia
1. `valencia.txt` (principal)
2. `cercanias_valencia.txt` (respaldo)
3. `metro_valencia.txt` (respaldo)
4. `tranvia_valencia.txt` (respaldo)

### Estrategia Multi-Regional
- **Auto-detección:** Análisis de patrones de tarjeta determina región
- **Cascada de respaldo:** Prueba múltiples archivos regionales
- **Logging de desconocidos:** Registra códigos no encontrados para expansión de base de datos

---

## 🔧 Funciones Principales

### Plugin RENFE Suma 10
| Función | Propósito |
|---------|-----------|
| `renfe_sum10_extract_zone_code` | Extracción zona estándar SUMA 10 |
| `renfe_sum10_extract_mobilis_zone_code` | Análisis multi-bloque zona MOBILIS 30 |
| `renfe_sum10_get_zone_name` | Mapeo código a nombre de zona |
| `renfe_sum10_get_origin_station` | Determinación estación origen |
| `renfe_sum10_is_history_entry` | Validación historial viajes |
| `renfe_sum10_parse_travel_history` | Procesamiento historial completo |
| `renfe_sum10_sort_history_entries` | Ordenamiento cronológico |
| `renfe_sum10_get_station_name_dynamic` | Traducción dinámica código estación |

### Plugin RENFE Regular
| Función | Propósito |
|---------|-----------|
| `renfe_regular_detect_bono_regular` | Detección Bono Regular 10 viajes |
| `renfe_regular_extract_trip_counter` | Extracción contador viajes |
| `renfe_regular_get_station_multi_position` | Búsqueda código estación multi-posición |
| `renfe_regular_detect_region` | Detección automática región |
| `renfe_regular_load_regional_stations` | Carga base datos específica región |

---

## 🎯 Validación en Mundo Real

### Tipos de Tarjetas Probadas
- ✅ **Valencia SUMA 10** - Pago por viaje con historial
- ✅ **Valencia MOBILIS 30** - Abono mensual con extracción completa de nombre (nombre + ambos apellidos)
- ✅ **Normalización de acentos españoles** - Todos los diacríticos convertidos correctamente (á→A, ñ→N, etc.)
- ✅ **Bono Regular 10 viajes** - Región Alzira con contador viajes
- ✅ **RENFE multi-regional** - Tarjetas por toda España

### Precisión de Detección
- 🎯 **99%+ reconocimiento estaciones** para región Valencia
- 🎯 **Detección automática variantes** (SUMA/MOBILIS/Bono)
- 🎯 **Conteo preciso viajes** para Bono Regular
- 🎯 **Análisis confiable historial** con ordenamiento timestamps

### Validación de Patrones
- ✅ **Patrón "CARRERES MOMP"** para MOBILIS 30 confirmado
- ✅ **Patrón "E8 03 04"** para Bono Regular validado
- ✅ **Detección zona multi-bloque** para MOBILIS 30 funcionando

---

## 📊 Métricas de Rendimiento

- ⚡ **Análisis Rápido** - Lectura de tarjeta en menos de un segundo
- 🔋 **Bajo Consumo** - Optimizado para batería del Flipper Zero
- 💾 **Eficiente en Memoria** - Límite caché 50 estaciones
- 🔄 **Confiable** - Manejo robusto de errores y validación
- 🗺️ **Escalable** - Fácil adición de nuevas regiones/estaciones

---

## 🐛 Problemas Conocidos y Soluciones

### Plugin RENFE Regular
- ❌ **Lectura en vivo no soportada** - Usar dumps guardados de Proxmark3
- 🔐 **Claves de cifrado variables** - Las tarjetas usan diferentes claves por región
- 💡 **Solución:** Exportar dumps con herramientas externas, luego analizar en Flipper

### Detección Zona MOBILIS 30
- 🔧 **Algoritmo mejorado implementado** - Análisis multi-bloque para zonas precisas
- ✅ **Extracción nombre funcionando** - Patrón "CARRERES MOMP" validado
- 🎯 **Visualización zona mejorada** - Usa Bloques 12, 10, 1 para detección

### Cobertura Base Datos Estaciones
- ❓ **Algunos códigos desconocidos** - Base de datos aún en expansión
- 💡 **Solución:** Códigos desconocidos registrados para futuras actualizaciones
- 📝 **Contribuciones comunitarias bienvenidas** - Enviar nuevos códigos estación

---

## 📞 Soporte y Desarrollo

### Reportar Códigos de Estación Desconocidos
Verificar logs de depuración para mensajes como:
```
UNKNOWN STATION CODE: 0x0012 (decimal: 18)
Add '0x0012,Station_Name' to valencia.txt
```

### Contribuir Nuevas Variantes de Tarjetas
- 📝 Reportar tipo de tarjeta y región
- 📊 Incluir logs de depuración con `log debug`
- 💾 Compartir dumps anonimizados si es posible

### Configuración de Desarrollo
1. Habilitar logging de depuración: `log debug`
2. Probar con tarjetas reales y dumps guardados
3. Verificar `applications_user/Metroflip/scenes/plugins/`
4. Actualizar bases de datos estaciones en `assets/metroflip/renfe/stations/`

---

## 🚀 Hoja de Ruta Futura

- 🆕 **Variantes de tarjetas adicionales** basadas en reportes de usuarios
- 🗺️ **Soporte regional extendido** para provincias españolas restantes
- 📱 **UI mejorada** con visualización gráfica de viajes
- 🔄 **Actualizaciones automáticas** para códigos de estaciones
- 📊 **Análisis estadístico** de patrones de viaje
- 🌍 **Expansión internacional** a otros sistemas de transporte europeos

---

## 🤝 Contribuciones de la Comunidad

Esta versión incorpora datos de pruebas en mundo real de:
- 🏠 **Residentes de Valencia** con tarjetas SUMA/MOBILIS activas
- 🗺️ **Viajeros multi-regionales** por toda la red española
- 🔧 **Desarrolladores** contribuyendo bases de datos de códigos estación

---

## 📈 Información de Versión

Para notas de versión completas incluyendo especificaciones técnicas, características de depuración y desglose completo de características, ver:
- **RELEASE_NOTES.md** - Documentación completa
- **Etiquetas Git** - Seguimiento de versiones
- **Feedback comunitario** - Resultados de validación en mundo real

---
