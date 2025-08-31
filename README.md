# Metroflip

Aplicación lectora de tarjetas de metro para Flipper Zero que soporta sistemas de transporte españoles, especialmente RENFE.

**Autor:** [@luu176](https://github.com/luu176)

![Menu-Top-Screenshot](screenshots/Menu-Top.png)

## 🚆 Plugins RENFE

### Plugin RENFE Suma 10 (Valencia)
- **SUMA 10** - Tarjetas de pago por viaje
- **MOBILIS 30** - Abonos mensuales

### Plugin RENFE Regular (Nacional)
- **Bono Regular 10** - Tarjetas con contador de viajes
- **RENFE Cercanías** - Trenes de cercanías
- **RENFE Regular** - Tarjetas estándar

![Renfe-Screenshot](screenshots/Suma10.png)

## 🌍 Regiones Soportadas

Valencia, Madrid, Catalunya, Andalucía, País Vasco, Galicia, Aragón, Castilla y León, Asturias, Cantabria, La Rioja, Navarra, Murcia, Castilla-La Mancha, Extremadura, Canarias, Baleares, Ceuta y Melilla.

## ✨ Características

- **Lectura en tiempo real** de tarjetas de Valencia
- **Análisis de dumps guardados** para tarjetas nacionales
- **Historial completo de viajes** con timestamps
- **Identificación de estaciones** y tipos de transacción
- **Extracción de datos del titular** (MOBILIS 30)
- **Contador de viajes** (Bono Regular)

## 🚀 Uso Rápido

### Tarjetas de Valencia (Lectura en Vivo)
1. Abrir **Metroflip**
2. Seleccionar **RENFE Suma 10**
3. Acercar tarjeta al Flipper Zero
4. Ver información instantánea

### Tarjetas Nacionales (Dumps)
1. Exportar dump con **Proxmark3**
2. Copiar archivo `.nfc` a carpeta **Saved**
3. Seleccionar **RENFE Regular**
4. Cargar dump desde **Saved**

## 🎯 Datos Extraídos

- 🆔 **UID** de la tarjeta
- 🎯 **Zona tarifaria** (A, B, C, D, E, F)
- 🏠 **Estación de origen**
- 👤 **Nombre completo** (MOBILIS 30)
- 🔢 **Contador viajes** (X/10 para Bono)
- 📚 **Historial de viajes** ordenado cronológicamente

## 📋 Tipos de Transacción

- **Entrada/Salida** - Acceso a estaciones
- **Transbordo** - Cambio de línea
- **Recarga** - Añadir crédito
- **Validación** - Control de billete
- **Inspección** - Control oficial

## 🔧 Problemas Conocidos

- Plugin RENFE Regular requiere dumps (no lectura en vivo)
- Algunas estaciones pueden aparecer como "Desconocida"
- Base de datos en constante actualización

## 🤝 Contribuir

Para reportar códigos de estación desconocidos o nuevas variantes de tarjetas, habilitar logging con `log debug` y reportar los códigos encontrados.

---

**Nota:** Esta aplicación es solo para análisis educativo de tarjetas de transporte público.