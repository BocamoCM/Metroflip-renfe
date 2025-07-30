
# Metroflip
Metroflip is a multi-protocol metro card reader app for the Flipper Zero, inspired by the Metrodroid project. It enables the parsing and analysis of metro cards from transit systems around the world, providing a proof-of-concept for exploring transit card data in a portable format. 

# Author
[@luu176](https://github.com/luu176)


![Menu-Top-Screenshot](screenshots/Menu-Top.png)
# Metroflip Plugin: RENFE Suma 10
## Descripción general



Este módulo implementa el soporte para **lectura y análisis** de tarjetas **RENFE Suma 10** con Flipper Zero usando el sistema **Metroflip**.

---

## 📚 Estructura general

- **Archivo principal:** `renfe_sum10.c`
- **Protocolo NFC:** Mifare Classic 1K / 4K
- **Claves:** Sectoriales específicas (`renfe_sum10_keys`) + alternativas (`renfe_sum10_alt_keys`)
- **Escena principal:** `Metroflip:Scene:RenfeSum10`

---

## ⚙️ Flujo de trabajo

1. **Detección NFC:** Se detecta la tarjeta usando `MfClassicPoller`
2. **Autenticación:** Se intentan claves sectoriales en orden
3. **Lectura:** Se leen bloques definidos (viajes, zona, configuración)
4. **Parseo:** Los datos binarios se interpretan según patrones reales verificados

---

![Renfe-Screenshot](screenshots/Suma10.png)



## 🧩 Interpretación de datos

A continuación se detalla **cómo se extrae e interpreta cada campo**:

### 🆔 UID

- **Fuente:** `iso14443_3a_data->uid`
- **Formato:** Hexadecimal con bytes separados por espacios
- **Propósito:** Identifica de forma única la tarjeta

### 🎯 Zona tarifaria

- **Bloque:** 5
- **Bytes:** 5 y 6 (`block5[5] << 8 | block5[6]`)
- **Función extractora:** `renfe_sum10_extract_zone_code`
- **Interpretación:** `renfe_sum10_get_zone_name` mapea códigos a zonas
  - Zonas principales: `A`, `B`, `C`
  - Subzonas y líneas especiales
  - Combinaciones tarifarias
- **Ejemplo:** `0x6C16` → Zona `A`

### 🏠 Estación de origen

**Algoritmo de determinación:**
1. Busca el **primer top-up (recarga)** con `transaction_type = 0x33` o `0x3A`
2. Si no hay recarga, toma el primer viaje válido
3. Extrae `station_code` de bytes 9–10
4. Traduce usando `renfe_sum10_get_station_name_dynamic`

**Bloques de búsqueda:** 18, 22, 28–30, 44–46

### 📚 Historial de viajes

**Bloques analizados:** `[4–14, 16–22, 28–30, 44–46]`

**Criterios de validación** (`renfe_sum10_is_history_entry`):
- `transaction_type` válido
- `station_code` válido (no nulo ni patrón por defecto)
- Timestamp válido (bytes 2–4 ≠ `0x00` o `0xFF`)

**Procesamiento:**
- Ordenamiento temporal (`renfe_sum10_sort_history_entries`)
- Formato de salida: `N. [Tipo] - [Estación] [Timestamp] (Detalles)`

### 📋 Tipos de transacción

| Código | Descripción | Tipo |
|--------|-------------|------|
| `0x13` | Entrada | ENTRY |
| `0x1A` | Salida | EXIT |
| `0x1E` | Transbordo | TRANSFER |
| `0x16` | Validación | VALIDATION |
| `0x33` | Recarga | TOP-UP |
| `0x3A` | Cargo adicional | CHARGE |
| `0x17` | Inspección | INSPECTION |
| `0x23` | Descuento | DISCOUNT |
| `0x2A` | Sanción | PENALTY |
| `0x2B` | Operación especial | SPECIAL |
| Otros | Desconocido | Unknown |

### 🔢 Viajes restantes

- **Ubicación:** Bloque 5, Byte 4 (`block5[4]`)
- **Condición:** `block5[0] == 0x01` y `block5[1–3] == 0x00`
- **Formato de salida:** `🎫 Trips: [n]`

---

## 🗺️ Sistema de estaciones

### Traducción de códigos

Cada `station_code` se traduce usando archivos de mapeo locales ubicados en:

```
/ext/apps_assets/metroflip/renfe/stations/
├── valencia.txt
├── cercanias_valencia.txt
├── metro_valencia.txt
└── tranvia_valencia.txt
```

### Formato de archivo

```
0xCODE,Nombre de estación
```

**Ejemplos:**
```
0x6A12,València Nord
0x6A13,Estació del Nord
0x6A14,Xàtiva
```

### Sistema de caché

- **Capacidad:** Máximo 150 estaciones en memoria (`StationCache`)
- **Fallback:** Si no se encuentra el código → `Unknown`
- **Carga dinámica:** Intenta múltiples archivos si `valencia.txt` no está disponible

### Jerarquía de archivos

1. `valencia.txt` (principal)
2. `cercanias_valencia.txt` (fallback)
3. `metro_valencia.txt` (fallback)
4. `tranvia_valencia.txt` (fallback)

---

## 🔧 Funciones principales

| Función | Propósito |
|---------|-----------|
| `renfe_sum10_extract_zone_code` | Extrae código de zona tarifaria |
| `renfe_sum10_get_zone_name` | Mapea código a nombre de zona |
| `renfe_sum10_get_origin_station` | Determina estación de origen |
| `renfe_sum10_is_history_entry` | Valida entrada de historial |
| `renfe_sum10_parse_travel_history` | Procesa historial completo |
| `renfe_sum10_sort_history_entries` | Ordena entradas cronológicamente |
| `renfe_sum10_get_station_name_dynamic` | Traduce códigos de estación |

---

## 📝 Notas técnicas

- **Compatibilidad:** Mifare Classic 1K y 4K
- **Autenticación:** Sistema de claves múltiples con fallback
- **Robustez:** Validación estricta de datos para evitar falsos positivos
- **Escalabilidad:** Sistema de caché optimizado para múltiples consultas
- **Mantenimiento:** Archivos de estaciones actualizables independientemente del código
